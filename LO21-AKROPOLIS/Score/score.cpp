#include "score_registry.h"
#include <algorithm>
#include <queue>
#include <set>
#include <map>
#include "../Joueur/joueur.h"


// classe pour lire la grille
class ScoreReader {
    const std::map<Coord, Hexagone>& grille;
public:
    ScoreReader(const std::map<Coord, Hexagone>& g) : grille(g) {}

    std::pair<std::string, int> parseHex(const Hexagone& h) const {
        std::string s = h.getContenu();
        int niv = 1;
        size_t pos = s.find('#');
        if (pos != std::string::npos) niv = std::stoi(s.substr(pos + 1));

        std::string type = "Autre";
        if (s.find("[Q]") != std::string::npos) {
            if (h.getCouleur() == Couleur::Bleu) type = "Hab";
            else if (h.getCouleur() == Couleur::Jaune) type = "Mar";
            else if (h.getCouleur() == Couleur::Rouge) type = "Cas";
            else if (h.getCouleur() == Couleur::Violet) type = "Tem";
            else if (h.getCouleur() == Couleur::Vert) type = "Jar";
        }
        else if (s.find('*') != std::string::npos) type = "Place";
        return { type, niv };
    }

    std::vector<VisibleCell> getMap() const {
        std::vector<VisibleCell> cells;
        for (auto const& [coord, hex] : grille) {
            auto res = parseHex(hex);
            cells.push_back({ coord, res.second, res.first });
        }
        return cells;
    }

    Stars getStars() const {
        Stars st;
        for (auto const& [coord, hex] : grille) {
            TypeHexagone s = hex.getType();
            int count;
            switch (s) {
            case TypeHexagone::Place1: count = 1;
            case TypeHexagone::Place2: count = 2;
            case TypeHexagone::Place3: count = 3;
            default: count = 0;
            }
            if (hex.getCouleur() == Couleur::Bleu) st.hab += count;
            else if (hex.getCouleur() == Couleur::Jaune) st.mar += count;
            else if (hex.getCouleur() == Couleur::Rouge) st.cas += count;
            else if (hex.getCouleur() == Couleur::Violet) st.tem += count;
            else if (hex.getCouleur() == Couleur::Vert) st.jar += count;
        }
        return st;
    }

    static std::vector<Coord> getNeighbors(const Coord& c) {
        static const std::vector<std::pair<int, int>> dirs = { {0, -2}, {1, -1}, {1, 1}, {0, 2}, {-1, 1}, {-1, -1} };
        std::vector<Coord> res;
        for (auto const& d : dirs) res.push_back({ c.q + d.first, c.r + d.second });
        return res;
    }

    bool isOccupied(const Coord& c) const { return grille.count(c) > 0; }

    bool isMarketSatisfied(const Coord& pos) const {
        for (const auto& nb : getNeighbors(pos)) {
            auto it = grille.find(nb);
            if (it != grille.end() && it->second.getContenu().find('*') != std::string::npos &&
                it->second.getCouleur() == Couleur::Jaune) return true;
        }
        return false;
    }

    bool isBarrackSatisfied(const Coord& pos) const {
        int empty = 0;
        for (const auto& nb : getNeighbors(pos)) if (!isOccupied(nb)) empty++;
        return (empty == 3 || empty == 4);
    }

    bool isGardenSatisfied(const Coord& pos) const {
        for (const auto& nb : getNeighbors(pos)) {
            if (!isOccupied(nb)) {
                bool surrounded = true;
                for (const auto& lakeNb : getNeighbors(nb)) if (!isOccupied(lakeNb)) { surrounded = false; break; }
                if (surrounded) return true;
            }
        }
        return false;
    }
};

namespace Scoring {

    std::vector<ScoreBreakdown> calculerResultatsEnregistres(
        const std::vector<Joueur*>& joueurs,
        ModeScore mode,
        const ActiveVariants& variants)
    {
        std::vector<ScoreBreakdown> resultats;
        resultats.reserve(joueurs.size());

        for (Joueur* j : joueurs) {
            ScoreBreakdown sb;
            JoueurIA* ia = dynamic_cast<JoueurIA*>(j);
            if (ia) {
                // Si c'est l'IA, on utilise TOUJOURS le calcul Illustre (Réserve + Difficulté)
                sb = calculerIllustre(ia);
            }
            else {
                const auto& grille = j->getCite().getGrillePlate().getGrille();

                size_t pierres = j->getPierres();


                switch (mode) {
                case ModeScore::Classique:
                    sb = calculerClassique(grille, pierres);
                    break;
                case ModeScore::Variante:
                    sb = calculerVariante(grille, pierres, variants);
                    break;
                case ModeScore::Personnel:
                    sb = calculerPersonnel(grille, pierres);
                    break;
                default:
                    sb = calculerClassique(grille, pierres);
                    break;
                }
            }
            resultats.push_back(sb);
        }
        return resultats;
    }

    ScoreBreakdown calculerClassique(const std::map<Coord, Hexagone>& grille, size_t nbStones) {
        ScoreReader reader(grille);
        auto vis = reader.getMap();
        auto st = reader.getStars();
        ScoreBreakdown sb;
        sb.hab = sumHabLargestConnected(grille, vis) * st.hab;
        sb.mar = scoreMarkets(vis) * st.mar;
        sb.cas = scoreBarracks(grille, vis) * st.cas;
        sb.tem = scoreTemples(grille, vis) * st.tem;
        sb.jar = scoreGardens(vis) * st.jar;
        sb.stones = nbStones;
        sb.total = sb.hab + sb.mar + sb.cas + sb.tem + sb.jar + sb.stones;
        return sb;
    }

    int sumHabLargestConnected(const std::map<Coord, Hexagone>& grille, const std::vector<VisibleCell>& vis) {
        std::map<Coord, int> habs;
        for (const auto& vc : vis) if (vc.district == "Hab") habs[vc.pos] = vc.height;
        std::set<Coord> visited;
        int maxVal = 0;
        for (const auto& vc : vis) {
            if (vc.district != "Hab" || visited.count(vc.pos)) continue;
            int current = 0; std::queue<Coord> q; q.push(vc.pos); visited.insert(vc.pos);
            while (!q.empty()) {
                Coord curr = q.front(); q.pop(); current += habs[curr];
                for (const auto& nb : ScoreReader::getNeighbors(curr)) {
                    if (habs.count(nb) && !visited.count(nb)) { visited.insert(nb); q.push(nb); }
                }
            }
            maxVal = std::max(maxVal, current);
        }
        return maxVal;
    }

    int scoreMarkets(const std::vector<VisibleCell>& vis) {
        int total = 0;
        for (const auto& vc : vis) {
            if (vc.district == "Mar") {
                bool isolated = true;
                for (const auto& nb : ScoreReader::getNeighbors(vc.pos)) {
                    auto it = std::find_if(vis.begin(), vis.end(), [&](const VisibleCell& other) { return other.pos == nb && other.district == "Mar"; });
                    if (it != vis.end()) { isolated = false; break; }
                }
                if (isolated) total += vc.height;
            }
        }
        return total;
    }

    int scoreBarracks(const std::map<Coord, Hexagone>& grille, const std::vector<VisibleCell>& vis) {
        int total = 0; ScoreReader r(grille);
        for (const auto& vc : vis) {
            if (vc.district == "Cas") {
                for (const auto& nb : ScoreReader::getNeighbors(vc.pos)) if (!r.isOccupied(nb)) { total += vc.height; break; }
            }
        }
        return total;
    }

    int scoreTemples(const std::map<Coord, Hexagone>& grille, const std::vector<VisibleCell>& vis) {
        int total = 0; ScoreReader r(grille);
        for (const auto& vc : vis) {
            if (vc.district == "Tem") {
                bool surrounded = true;
                for (const auto& nb : ScoreReader::getNeighbors(vc.pos)) if (!r.isOccupied(nb)) { surrounded = false; break; }
                if (surrounded) total += vc.height;
            }
        }
        return total;
    }

    int scoreGardens(const std::vector<VisibleCell>& vis) {
        int total = 0;
        for (const auto& vc : vis) if (vc.district == "Jar") total += vc.height;
        return total;
    }

    ScoreBreakdown calculerVariante(const std::map<Coord, Hexagone>& grille, size_t nbStones, const ActiveVariants& v) {
        ScoreBreakdown sb = calculerClassique(grille, nbStones);
        ScoreReader reader(grille); auto vis = reader.getMap();
        if (v.hab && sumHabLargestConnected(grille, vis) >= 10) sb.hab *= 2;
        if (v.tem) { for (auto const& c : vis) if (c.district == "Tem" && c.height > 1) { sb.tem *= 2; break; } }
        if (v.mar) { for (auto const& c : vis) if (c.district == "Mar" && reader.isMarketSatisfied(c.pos)) { sb.mar *= 2; break; } }
        if (v.cas) { for (auto const& c : vis) if (c.district == "Cas" && reader.isBarrackSatisfied(c.pos)) { sb.cas *= 2; break; } }
        if (v.jar) { for (auto const& c : vis) if (c.district == "Jar" && reader.isGardenSatisfied(c.pos)) { sb.jar *= 2; break; } }
        sb.total = sb.hab + sb.mar + sb.cas + sb.tem + sb.jar + sb.stones;
        return sb;
    }

    ScoreBreakdown calculerPersonnel(const std::map<Coord, Hexagone>& g, size_t s) { return calculerClassique(g, s); }

    ScoreBreakdown calculerIllustre(const JoueurIA* ia) {
        ScoreBreakdown sb;
        sb.hab = sb.mar = sb.cas = sb.tem = sb.jar = sb.stones = sb.total = 0;
        std::map<Couleur, int> quartiers;
        std::map<Couleur, int> etoiles;
        int nbCarrieres = 0;

        // 1. On parcourt les tuiles stockées dans la reserve_abstraite de l'IA
        for (const Tuile* t : ia->getReserve()) {
            const Hexagone* hexs[3] = { &t->getHex1(), &t->getHex2(), &t->getHex3() };

            for (int i = 0; i < 3; i++) {
                std::string contenu = hexs[i]->getContenu();
                Couleur coul = hexs[i]->getCouleur();

                // Comptage des quartiers [Q]
                if (contenu.find("[Q]") != std::string::npos) {
                    quartiers[coul]++;
                }
                // Comptage des étoiles (places)
                else if (contenu.find('*') != std::string::npos) {
                    int nbStars = (int)std::count(contenu.begin(), contenu.end(), '*');
                    etoiles[coul] += nbStars;
                }
                // Comptage des carrières [C]
                else if (contenu.find("[C]") != std::string::npos) {
                    nbCarrieres++;
                }
            }
        }
        int mult = (ia->getDifficulte() == Difficulte::Callicrates) ? 2 : 1;

        // 3. Calcul des points par type (Quartiers * Multiplicateur * Etoiles)
        sb.hab = quartiers[Couleur::Bleu] * mult * etoiles[Couleur::Bleu];
        sb.mar = quartiers[Couleur::Jaune] * mult * etoiles[Couleur::Jaune];
        sb.cas = quartiers[Couleur::Rouge] * mult * etoiles[Couleur::Rouge];
        sb.tem = quartiers[Couleur::Violet] * mult * etoiles[Couleur::Violet];
        sb.jar = quartiers[Couleur::Vert] * mult * etoiles[Couleur::Vert];
        sb.stones = (int)ia->getPierres();

        sb.total = sb.hab + sb.mar + sb.cas + sb.tem + sb.jar + sb.stones;


        if (ia->getDifficulte() == Difficulte::Metagenes) {
            sb.total += (nbCarrieres * 2);
        }

        return sb;
    }
}


// --- Classe Pont pour l'interface Score ---
class ScoreBridge : public Score {
    ModeScore mode_;
    ActiveVariants vars_;
public:
    ScoreBridge(ModeScore m, const ActiveVariants& v) : mode_(m), vars_(v) {}

    ScoreBreakdown compute(const std::map<Coord, Hexagone>& grille, size_t nbStones) const override {
        // redirection vers les fonctions de namespace existantes
        if (mode_ == ModeScore::Variante) {
            return Scoring::calculerVariante(grille, nbStones, vars_);
        }
        // Par défaut ou pour le mode classique
        return Scoring::calculerClassique(grille, nbStones);
    }
};

// fonction à appeler une seule fois pour initialiser le catalogue de calculs.

void initialiserScores() {
    auto& reg = ScoreRegistry::instance();

    reg.registerScore(ModeScore::Classique, [](const ActiveVariants& v) {
        return std::make_unique<ScoreBridge>(ModeScore::Classique, v);
        });

    reg.registerScore(ModeScore::Variante, [](const ActiveVariants& v) {
        return std::make_unique<ScoreBridge>(ModeScore::Variante, v);
        });
}
#pragma once
#include "../grilleHexa/grilleHexa.h"
#include "affichageTuile.h"

namespace GrilleRendering {
    using namespace ConsoleRendering;

    inline void afficherGrilleBase(std::ostream& os, const GrilleHexa& grille, const std::set<Coord>* positions_mises_en_evidence = nullptr) {
        Coord min_coord = grille.minCoord();
        Coord max_coord = grille.maxCoord();

        if (positions_mises_en_evidence) {
            for (const auto& c : *positions_mises_en_evidence) {
                if (c.q < min_coord.q) min_coord.q = c.q;
                else if (c.r < min_coord.r) min_coord.r = c.r;
                else if (c.q > max_coord.q) max_coord.q = c.q;
                else if (c.r > max_coord.r) max_coord.r = c.r;
            }
        }

        int delta = LARG_HEXAGONE % 2;
        int larg_grille = (max_coord.q - min_coord.q + 1) * LARG_HEXAGONE;
        int nb_tiret = 4 - delta;
        int space = (LARG_HEXAGONE - nb_tiret) / 2;

        std::string vide = std::string(LARG_HEXAGONE, ' ');
        std::string adjacent = std::string(space, ' ') +
            std::string(nb_tiret, '-') +
            std::string(space, ' ');

        os << "\n " << std::string(larg_grille, '_') << "\n";

        for (int r = min_coord.r; r < max_coord.r + 1; r++) {
            os << "|";
            for (int q = min_coord.q; q < max_coord.q + 1; q++) {
                Coord coord_actuelle = { q, r };

                auto upper = grille.getGrille().count({ q, r - 1 });
                auto lower = grille.getGrille().count({ q, r + 1 });
                auto it = grille.getGrille().find(coord_actuelle);

                bool est_mise_en_evidence = positions_mises_en_evidence &&
                    positions_mises_en_evidence->count(coord_actuelle);

                if (it != grille.getGrille().end()) {
                    const Hexagone& hex = it->second;

                    if (est_mise_en_evidence) {
                        os << gras();
                    }

                    os << codeAnsi(hex.getCouleur())
                        << "<(" << justify(TuileRendering::getContenu(hex)) << ")>"
                        << codeAnsi(Couleur::Reset);

                }
                else if (est_mise_en_evidence) {
                    os << gras() << codeAnsi(Couleur::Cyan)
                        << "<(" << justify("...") << ")>"
                        << codeAnsi(Couleur::Reset);

                }
                else if (upper && lower) {
                    os << adjacent;
                }
                else {
                    os << vide;
                }
            }
            os << "|\n";
        }

        os << " " << std::string(larg_grille, '=') << "\n";
    }

    // Surcharge de fonction pour afficher spécifiquement des Hexagones_Affichages
    inline void afficherGrilleBase(std::ostream& os, const GrilleHexa_Plate& grille, const std::set<Coord>* positions_mises_en_evidence = nullptr) {
        Coord min_coord = grille.minCoord();
        Coord max_coord = grille.maxCoord();

        if (positions_mises_en_evidence) {
            for (const auto& c : *positions_mises_en_evidence) {
                if (c.q < min_coord.q) min_coord.q = c.q;
                else if (c.r < min_coord.r) min_coord.r = c.r;
                else if (c.q > max_coord.q) max_coord.q = c.q;
                else if (c.r > max_coord.r) max_coord.r = c.r;
            }
        }

        int delta = LARG_HEXAGONE % 2;
        int larg_grille = (max_coord.q - min_coord.q + 1) * LARG_HEXAGONE;
        int nb_tiret = 4 - delta;
        int space = (LARG_HEXAGONE - nb_tiret) / 2;

        std::string vide = std::string(LARG_HEXAGONE, ' ');
        std::string adjacent = std::string(space, ' ') +
            std::string(nb_tiret, '-') +
            std::string(space, ' ');

        os << "\n " << std::string(larg_grille, '_') << "\n";

        for (int r = min_coord.r; r < max_coord.r + 1; r++) {
            os << "|";
            for (int q = min_coord.q; q < max_coord.q + 1; q++) {
                Coord coord_actuelle = { q, r };

                auto upper = grille.getGrille().count({ q, r - 1 });
                auto lower = grille.getGrille().count({ q, r + 1 });
                auto it = grille.getGrille().find(coord_actuelle);

                bool est_mise_en_evidence = positions_mises_en_evidence &&
                    positions_mises_en_evidence->count(coord_actuelle);

                if (it != grille.getGrille().end()) {
                    try {
                        const HexagoneAffichage hex = dynamic_cast<const HexagoneAffichage&>(it->second);

                        os << codeAnsi(hex.getCouleur()) << "<(" << justify(TuileRendering::getContenuAvecNiveau(hex)) << ")>" << codeAnsi(Couleur::Reset);

                        if (est_mise_en_evidence) os << gras();

                        os << codeAnsi(hex.getCouleur())
                            << "<(" << justify(TuileRendering::getContenuAvecNiveau(hex)) << ")>"
                            << codeAnsi(Couleur::Reset);
                    }
                    catch (std::bad_cast& e) {
                        throw AkropolisException("La grille plate contient des Hexagones qui ne sont pas des Hexagones d'affichage", "affichageGrille");
                    }

                }
                else if (est_mise_en_evidence) {
                    os << gras() << codeAnsi(Couleur::Cyan)
                        << "<(" << justify("...") << ")>"
                        << codeAnsi(Couleur::Reset);

                }
                else if (upper && lower) {
                    os << adjacent;
                }
                else {
                    os << vide;
                }
            }
            os << "|\n";
        }

        os << " " << std::string(larg_grille, '=') << "\n";
    }

    inline void afficherGrille(const GrilleHexa& grille, const std::set<Coord>* positions_mises_en_evidence = nullptr) {
        afficherGrilleBase(std::cout, grille, positions_mises_en_evidence);
    }

    inline void afficherPositionsPossibles(const GrilleHexa& grille, const std::set<Coord>& positions, size_t niveau) {
        std::cout << "\n" << std::string(46, '.') << " Placements possibles : " << std::string(46, '.');

        afficherGrille(grille, &positions);

    }

    inline void afficherOrientationsPossibles(const GrilleHexa& grille, const Coord& centre, const std::set<int>& orientations, size_t niveau) {
        std::cout << "\n" << std::string(45, '.') << " Orientations possibles : " << std::string(45, '.') << "\n";

        std::vector<Coord> coords;
        std::set<Coord> brillance;

        if (orientations.size() == 1) {
            std::cout << "\n Une seule orientation possible.";
            coords = GrilleHexa::calculerPositionsTuile(centre, *orientations.begin());
            for (const auto& c : coords) brillance.insert(c);
            afficherGrille(grille, &brillance);
        }
        else {
            size_t ind = 1;
            for (const auto& orient : orientations) {
                std::cout << "\nOption " << ind++ << " : ";
                coords = GrilleHexa::calculerPositionsTuile(centre, orient);
                for (const auto& c : coords) brillance.insert(c);
                afficherGrille(grille, &brillance);
                brillance.clear();
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const GrilleHexa& grille) {
    GrilleRendering::afficherGrille(grille);
    return os;
}
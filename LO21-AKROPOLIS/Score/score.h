#pragma once
#include <vector>
#include <string>
#include <map>
#include <queue>
#include "../grilleHexa/grilleHexa.h"

class Joueur;
class JoueurIA;
enum class ModeScore { Classique, Variante, IllustreArchitecte, Personnel };


struct VisibleCell {
    Coord pos;
    int height;
    std::string district;
};

struct Stars {
    size_t hab = 0, mar = 0, cas = 0, tem = 0, jar = 0;
};

struct ScoreBreakdown {
    size_t hab = 0, mar = 0, cas = 0, tem = 0, jar = 0;
    size_t stones = 0;
    size_t total = 0;
};

struct ActiveVariants {
    bool hab = false, mar = false, cas = false, tem = false, jar = false;
};

class Score {
public:
    virtual ~Score() = default;
    virtual ScoreBreakdown compute(const std::map<Coord, Hexagone>& grille, size_t nbStones) const = 0;
};

namespace Scoring {

    // retourne les scores dans l'ordre du vecteur 'joueurs'
    std::vector<ScoreBreakdown> calculerResultatsEnregistres(
        const std::vector<Joueur*>& joueurs,
        ModeScore mode,
        const ActiveVariants& variants = {}
    );

    //  fonction de calculs principaux
    ScoreBreakdown calculerClassique(const std::map<Coord, Hexagone>& grille, size_t nbStones);
    ScoreBreakdown calculerVariante(const std::map<Coord, Hexagone>& grille, size_t nbStones, const ActiveVariants& v);
    ScoreBreakdown calculerIllustre(const JoueurIA* ia);
    ScoreBreakdown calculerPersonnel(const std::map<Coord, Hexagone>& grille, size_t nbStones);

    //  calcul spatial
    int sumHabLargestConnected(const std::map<Coord, Hexagone>& grille, const std::vector<VisibleCell>& vis);
    int scoreMarkets(const std::vector<VisibleCell>& vis);
    int scoreBarracks(const std::map<Coord, Hexagone>& grille, const std::vector<VisibleCell>& vis);
    int scoreTemples(const std::map<Coord, Hexagone>& grille, const std::vector<VisibleCell>& vis);
    int scoreGardens(const std::vector<VisibleCell>& vis);
}
void initialiserScores();
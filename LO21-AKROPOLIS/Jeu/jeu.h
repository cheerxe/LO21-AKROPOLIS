#pragma once

#include <vector>
#include <string>
#include <iostream>

//utilise pour creer les tuiles de akropolis a partir de photos

class Tuile;

class Jeu {
private:
    const size_t nb_tuiles;
    std::vector<Tuile> tuiles;//pas besoin de conaitre nb_tuiles � la compilation
    static Jeu* instance;
    Jeu(size_t nb_joueur);

public:

    size_t getNbTuiles() const { return nb_tuiles; }
    const Tuile& getTuile(size_t i) { return tuiles[i]; }//recuperer une tuile en particulier
    std::vector<const Tuile*> getReservoir() const;

    Jeu(const Jeu& autre_jeu) = delete;
    Jeu& operator=(const Jeu& autre) = delete;

    static Jeu& getInstance(size_t nb_joueur);

    void instancierTuiles(size_t nbjoueurs);

    ~Jeu() = default;
};
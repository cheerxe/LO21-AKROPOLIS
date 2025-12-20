#pragma once

#include "../Tuile/tuile.h"

class Tas {
private:
    size_t nb_tuiles;
    std::vector<const Tuile*> tuiles;
public:
    explicit Tas(int nbjoueurs);
    const Tuile* getTuilesTas(size_t i) const { return tuiles.at(i); }
    size_t getNbTuiles() const { return nb_tuiles; }
    ~Tas();
    void pushTuile(const Tuile* t);
};


class Pioche {
private:
    std::vector<const Tas*> tas;
    std::vector<size_t> curseur;
public:
    Pioche() = default;
    ~Pioche() = default;
    void ajouterTas(const Tas* t);
    bool estVide() const;
    const Tuile* piocherDepuisTas(size_t i);
    int prendreIndiceTasNonVide() const;

    //getter pour sofia
    std::vector<const Tas*> getTasPioche() const { return tas; }
    std::vector<size_t> getCurseur() const { return curseur; }
};
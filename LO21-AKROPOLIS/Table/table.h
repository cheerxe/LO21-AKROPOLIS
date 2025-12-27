#pragma once
#include <vector>

class Tuile;
class Pioche;

class Table {
private:
    size_t capacite;
    std::vector<const Tuile*> visibles;
    std::vector<const Tuile*> reservoir;
public:
    Table(size_t nb_joueurs, std::vector<const Tuile*> res);
    size_t taille() const { return visibles.size(); }
    bool estVide() const { return visibles.empty(); }
    size_t cout(size_t i) const { return i; } // ?????????????????
    const Tuile* prendre(size_t i);
    const Tuile* regarder(size_t i) const;
    void recharger(Pioche& p);

    //rajoute pour sofiette
    const size_t getCapacite() const { return capacite; }
    size_t tailleReservoir() const { return reservoir.size(); }
    size_t tailleR() const { return reservoir.size(); }

private:
    void remplirDepuisReservoir();
    void remplirDepuisReservoirOuTas(Pioche& p);
    void regleUneTuileRestante(Pioche& p);

};
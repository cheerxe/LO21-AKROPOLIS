#include "joueur.h"
#include "../Table/table.h"

void IllustreArchitecte::jouerTour(Table& chantier) override {
    const Tuile* t, choisie = chantier.prendre(0);
    for (size_t i = 0; i < chantier.taille(); ++i) {
        t = chantier.regarder(i);
        if (egalPlace(t->getHex1().getType()) || egalPlace(t->getHex2().getType()) || egalPlace(t->getHex3().getType())) {
            choisie = t;
            break;
        }
    }
    tuiles_collectees.push_back(tuile);
}
#include "../Table/table.h"
#include "../Cite/cite.h"
#include "../Dialogue/dialogueStrategy.h"
#include "controleurJoueur.h"

#include <vector>

const Tuile& ControleurJoueur::choisirTuile(size_t& pierres, Table chantier) {
    std::vector<Tuile> vect_tuile;
    for (size_t i = 0; i < chantier.taille(); i++) vect_tuile.emplace_back(*chantier.regarder(i));
    size_t choix_tuile = dialogue->demanderTuileDansChantier(vect_tuile, pierres);
    pierres -= choix_tuile;
    return *chantier.prendre(choix_tuile);
}

void ControleurJoueur::jouerTour(Cite& cite, size_t& pierres, Table chantier) {
    int recommencer = 1;
    while (recommencer == 1) {
        dialogue->afficherEtatActuelCite(cite);
        const Tuile& tuile = choisirTuile(pierres, chantier);
        size_t niveau_choisi = dialogue->demanderNiveau(cite.getNiveauxDisponibles());

        const GrilleHexa_Plate& grille = cite.getGrille();
        auto positions_possibles = cite.getPositionsValides(niveau_choisi);
        Coord centre = dialogue->demanderPosition(positions_possibles, niveau_choisi, grille);

        auto orient_possibles = cite.getOrientationsPossibles(centre, niveau_choisi);
        int orient = dialogue->demanderOrientation(centre, orient_possibles, niveau_choisi, grille);

        int sens = dialogue->demanderSens(tuile, orient);
        cite.placerTuile(tuile, niveau_choisi, centre, orient, sens);

        recommencer = dialogue->demanderAnnulation();
        if (recommencer) cite.retirerTuile(niveau_choisi, centre, orient);
    }
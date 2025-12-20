#pragma once
#include "../Cite/cite.h"
#include "../Dialogue/dialogueStrategy.h"

class IControleurJoueur {
public:
    virtual ~IControleurJoueur() = default;
    virtual std::string getPseudo() const = 0;
};

// Pas l'Illustre Architecte
class ControleurJoueurHumain : public IControleurJoueur {
private:
    IDialogueStrategy* dialogue;
    std::string pseudo;

    const Tuile& choisirTuile(size_t& pierres, Table chantier) {
        size_t choix_tuile = dialogue->demanderTuileDansChantier(chantier, pierres);
        pierres -= choix_tuile;
        return *chantier.prendre(choix_tuile);
    }

public:
    ControleurJoueurHumain(const std::string& p, IDialogueStrategy* d) : pseudo(p) {}

    void jouerTour(Cite& cite, size_t& pierres, Table chantier) {
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
    }

    std::string getPseudo() const override { return pseudo; }
};


// Illustre Architecte
class ControleurJoueurIA : public IControleurJoueur {
private:

    std::string pseudo;

    void prendreTuileAutomatique(Cite& cite, const Tuile& tuile) {
    }

public:
    ControleurJoueurIA() : pseudo(" Illustre Architecte ") {}

    void jouerTour(const Cite&, Table chantier) {

        //placerTuileAutomatique();
    }

    std::string getPseudo() const override { return pseudo; }
};
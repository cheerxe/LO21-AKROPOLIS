#pragma once
#include "../Partie/partie.h"
#include "../Dialogue/dialogueStrategy.h"


class IControleurParticipant {
    virtual const Tuile& choisirTuile(size_t& pierres, Table chantier) = 0;
public:
    virtual ~IControleurParticipant() = default;
    virtual std::string getPseudo() const = 0;
    // Implémentation de l'interface parent (délégation)
    virtual void jouerTour(Table& chantier) = 0;
};


class ControleurIllustreArchitecte : public IControleurParticipant {
    const Tuile& choisirTuile(size_t& pierres, Table chantier) {
        // A ECRIRE SELON LES REGLES PLACES ET MOINS CHER
    }
public:
    void jouerTour(Table& chantier) override {
        // A ECRIRE AUSSI
    }

    std::string getPseudo() const override {
        return "Illustre Architecte";
    }
};

// Pour implémenter de nouvelles difficultés en mode solo on pourrait définir de vrais IA qui possède une cité 
// et qui choisisse et place selon un certain modèle la tuile dans sa cite.

// Pour ce faire, on définit un controleur (IControleurJoueur) pour les actions d'un "joueur" qui possède une Cite (pas un Illustre Architecte)
// qui peut prendre la forme soit d'un controleur pour humain (ControleurJoueurHumain), soit d'un controleur pour IA (ControleurJoueurIA)


// Pas l'Illustre Architecte
class IControleurJoueur : public IControleurParticipant {
    const Tuile& choisirTuile(size_t& pierres, Table chantier) const;
public:
    virtual ~IControleurJoueur() = default;
    virtual void jouerTour(Cite& cite, size_t& pierres, Table& chantier) = 0;

    // Implémentation de l'interface parent (délégation)
    void jouerTour(Table& chantier) override = 0; // Sera implémentée par Joueur
};


class ControleurJoueurHumain : public IControleurJoueur {
private:
    IDialogueStrategy* dialogue;
    std::string pseudo;

    const Tuile& choisirTuile(size_t& pierres, Table chantier) {
        std::vector<Tuile> vect_tuile;
        for (size_t i = 0; i < chantier.taille(); i++) vect_tuile.emplace_back(*chantier.regarder(i));
        size_t choix_tuile = dialogue->demanderTuileDansChantier(vect_tuile, pierres);
        pierres -= choix_tuile;
        return *chantier.prendre(choix_tuile);
    }

public:
    ControleurJoueurHumain(const std::string& p, IDialogueStrategy* d) : pseudo(p), dialogue(d) {}

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

    void jouerTour(Table& chantier) override {
        // Cette version ne sera jamais appelée directement
        // Elle existe pour satisfaire l'interface mais sera surchargée par Joueur
    }

    std::string getPseudo() const override { return pseudo; }
};


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
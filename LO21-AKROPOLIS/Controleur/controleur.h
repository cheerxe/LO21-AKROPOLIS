#pragma once
#include "../Partie/partie.h"
#include "../Sauvegarde/sauvegarde.h"

class ControleurPartie {
private:
    Partie* partie;
    
    IDialogueStrategy* dialogue;

    // Création de la partie
    Partie* creerPartie(const std::vector<IControleurJoueur*>& controleurs) {
        if (controleurs.empty()) {
            throw AkropolisException("Il faut au moins un joueur", "ControleurPartie");
        }

        std::vector<Joueur*> joueurs;
        Pioche* pioche = nullptr;

        try {
            // Créer les joueurs avec leurs contrôleurs
            for (IControleurJoueur* controleur : controleurs) {
                joueurs.push_back(new Joueur(controleur));
            }

            pioche = new Pioche();

            return new Partie(joueurs, pioche);

        }
        catch (...) {
            for (Joueur* joueur : joueurs) {
                delete joueur;
            }
            delete pioche;
            throw;
        }
    }

    std::vector<IControleurJoueur*> creerControleurs() {
        size_t nb_joueurs = dialogue->demanderNombreJoueurs(2, 4);

        std::vector<IControleurJoueur*> controleurs;
        std::set<std::string> pseudos_pris; // pour savoir quel pseudo sont déjà pris par les autres joueurs

        try {
            for (size_t i = 0; i < nb_joueurs; ++i) {
                dialogue->afficherMessage("\n============ Joueur " + std::to_string(i + 1) + " ============");

                std::string pseudo = dialogue->demanderPseudo(pseudos_pris);
                controleurs.push_back(new ControleurJoueurHumain(pseudo, dialogue));

                pseudos_pris.insert(pseudo);
            }

            if (nb_joueurs == 1) {
                controleurs.push_back(new ControleurJoueurHumain( dialogue));
            }

            return controleurs;

        }
        catch (...) {
            for (IControleurJoueur* ctrl : controleurs) {
                delete ctrl;
            }
            throw;
        }
    }

    void initialiserPartie() {
        // Créer les contrôleurs de joueurs
        std::vector<IControleurJoueur*> controleurs = creerControleurs();

        // Créer la partie avec ces contrôleurs
        partie = creerPartie(controleurs);

        // Choisir l'architecte chef
        std::set<std::string> pseudos = partie->getPseudosJoueurs();
        size_t indice_architecte = dialogue->demanderArchitecteChef(pseudos);
        partie->setArchitecteChef(indice_architecte);
    }

    void jouerManche() {
        dialogue->afficherMessage("\n=== Nouvelle Manche ===");
        dialogue->afficherMessage("Architecte en chef : " + partie->getArchitecteChef()->getPseudo());

        // L'architecte chef commence
        size_t indice_debut = partie->getIndiceArchitecteChef();
        Table chantier = partie->chantier;

        for (size_t i = 0; i < partie->getNbJoueurs(); ++i) {
            size_t indice = (indice_debut + i) % partie->getNbJoueurs();
            Joueur* joueur = partie->getJoueur(indice);
            joueur->jouerTour(tuile);
        }

        // Changer d'architecte chef pour la prochaine manche
        partie->passerArchitecteChefSuivant();
    }

public:
    ControleurPartie(IDialogueStrategy* d)
        : partie(nullptr), dialogue(d) {
    }

    ~ControleurPartie() {
        delete partie;
    }

    ControleurPartie(const ControleurPartie&) = delete;
    ControleurPartie& operator=(const ControleurPartie&) = delete;

    void jouer() {
        dialogue->afficherEcranAccueil();
        int choix = dialogue->demanderReprisePartie();
        if (choix == 2) {
            auto chemin = Sauvegarde::recupererChemin();

            if (partieEnCours(chemin)) {
                dialogue->afficherMessage("\nReprise de la partie en cours ...");
                reprendrePartie(chemin);
            }
            else {
                dialogue->afficherErreur("\nIl n'y a pas de partie en cours. Lancement de la nouvelle partie ...");
            }
        }


        initialiserPartie();
        partie->demarrer();

        while (partie->estEnCours() && !partie->getPioche()->estVide()) {
            jouerManche();
        }

        partie->terminer();
        afficherResultats();
    }

    void afficherResultats() {
        dialogue->afficherMessage("\n=== Fin de la partie ===");

        for (size_t i = 0; i < partie->getNbJoueurs(); ++i) {
            const Joueur* joueur = partie->getJoueur(i);
            // int score = calculerScore(joueur->getCite());
            dialogue->afficherMessage(joueur->getPseudo() + " : [score]");
        }
    }
};

void lancerPartie(IDialogueStrategy dialogue) {
    ControleurPartie controleur(&dialogue);
    controleur.jouer();
}
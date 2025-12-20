#pragma once
#include "../Partie/partie.h"
#include "../Jeu/jeu.h"
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
        Table* chantier = nullptr;

        try {
            // Créer les joueurs avec leurs contrôleurs
            for (IControleurJoueur* controleur : controleurs) {
                joueurs.push_back(new Joueur(controleur));
            }

            Jeu& jeu = Jeu::getInstance(joueurs.size());

            pioche = new Pioche();

            chantier = new Table(joueurs.size(), jeu.getReservoir());

            return new Partie(joueurs, pioche, chantier);

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
        std::vector<IControleurJoueur*> controleurs;

        bool mode_solo = dialogue->demanderModeSolo();

        if (mode_solo) {
            std::string pseudo = dialogue->demanderPseudo();
            controleurs.push_back(new ControleurJoueurHumain(pseudo, dialogue));
            //
        }

        else {
            size_t nb_joueurs = dialogue->demanderNombreJoueurs(2, 4);

            std::set<std::string> pseudos_pris; // pour savoir quel pseudo sont déjà pris par les autres joueurs

            try {
                for (size_t i = 0; i < nb_joueurs; ++i) {
                    std::string pseudo = dialogue->demanderPseudo(pseudos_pris, i + 1);
                    controleurs.push_back(new ControleurJoueurHumain(pseudo, dialogue));

                    pseudos_pris.insert(pseudo);
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
    }

    void initialiserPartie() {
        std::vector<IControleurJoueur*> controleurs = creerControleurs();

        partie = creerPartie(controleurs);

        std::set<std::string> pseudos = partie->getPseudosJoueurs();
        size_t indice_architecte = dialogue->demanderArchitecteChef(pseudos);
        partie->setArchitecteChef(indice_architecte);
    }

    void jouerManche() {
        dialogue->annoncerNouvelleManche();
        dialogue->afficherArchitecte(*partie->getArchitecteChef());

        // L'architecte chef commence
        size_t indice_debut = partie->getIndiceArchitecteChef();
        Table chantier = *partie->getChantier();

        for (size_t i = 0; i < partie->getNbJoueurs(); ++i) {
            size_t indice = (indice_debut + i) % partie->getNbJoueurs();
            Joueur* joueur = partie->getJoueur(indice);
            joueur->jouerTour(chantier);
        }

        // Changer d'architecte chef pour la prochaine manche
        partie->passerArchitecteChefSuivant();
    }

    void afficherResultats() {
        dialogue->afficherMessage("\n=== Fin de la partie ===");

        for (size_t i = 0; i < partie->getNbJoueurs(); ++i) {
            const Joueur* joueur = partie->getJoueur(i);
            // int score = calculerScore(joueur->getCite());
            dialogue->afficherMessage(joueur->getPseudo() + " : [score]");
        }
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
        bool reprendre_partie = dialogue->demanderReprisePartie();
        if (reprendre_partie) {
            auto chemin = Sauvegarde::recupererChemin();
            bool en_cours = Sauvegarde::partieEnCours(chemin);
            if (en_cours) {
                dialogue->afficherMessage("\nReprise de la partie en cours ...");
                Sauvegarde::reprendrePartie(chemin);
            }
            else {
                dialogue->afficherErreur("\nIl n'y a pas de partie en cours. Lancement de la nouvelle partie ...");
                initialiserPartie();
            }
        }
        else {
            dialogue->afficherMessage("\nLancement de la nouvelle partie ...");
            initialiserPartie();
        }

        partie->demarrer();

        while (!partie->getPioche()->estVide()) {
                jouerManche();
        }
        partie->terminer();
    }
};



void lancerPartie(IDialogueStrategy& dialogue) {
    ControleurPartie controleur(&dialogue);
    controleur.jouer();
}
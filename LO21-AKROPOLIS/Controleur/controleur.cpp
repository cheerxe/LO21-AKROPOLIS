#include "controleur.h"

Partie* ControleurPartie::creerPartie(const std::vector<IParticipant*>& participants) {
    if (participants.empty()) {
        throw AkropolisException("Il faut au moins un participant", "ControleurPartie");
    }

    Pioche* pioche = nullptr;
    Table* chantier = nullptr;

    try {
        // Compter les joueurs humains pour initialiser le jeu
        size_t nb_joueurs_humains = 0;
        for (IParticipant* p : participants) {
            if (p->estJoueurHumain()) nb_joueurs_humains++;
        }

        Jeu& jeu = Jeu::getInstance(participants.size());
        pioche = new Pioche();
        chantier = new Table(participants.size(), jeu.getReservoir());

        return new Partie(participants, pioche, chantier);
    }
    catch (...) {
        delete pioche;
        throw;
    }
}

std::vector<IParticipant*> ControleurPartie::creerParticipants() {
    std::vector<IParticipant*> participants;

    bool mode_solo = dialogue->demanderModeSolo();

    try {
        if (mode_solo) {
            // Mode Solo : 1 joueur humain + Illustre Architecte
            std::string pseudo = dialogue->demanderPseudo();
            participants.push_back(
                new Joueur(new ControleurJoueurHumain(pseudo, dialogue))
            );
            participants.push_back(
                new IllustreArchitecte(new ControleurIllustreArchitecte())
            );
        }
        else {
            // Mode multijoueur : plusieurs joueurs humains
            size_t nb_joueurs = dialogue->demanderNombreJoueurs(2, 4);
            std::set<std::string> pseudos_pris;

            // au moins un joueur (pas que des IA)
            std::string pseudo = dialogue->demanderPseudo(pseudos_pris, 1);

            // if (joueurIA) {
            //      participants.push_back(new Joueur(new ControleurJoueurIA(pseudo, dialogue)));
            // } else {
            participants.push_back(new Joueur(new ControleurJoueurHumain(pseudo, dialogue)));
            pseudos_pris.insert(pseudo);

            for (size_t i = 1; i < nb_joueurs; ++i) {
                // bool joueurIA = dialogue->demanderSiJoueurIA()

                std::string pseudo = dialogue->demanderPseudo(pseudos_pris, i + 1);

                // if (joueurIA) {
                //      participants.push_back(new Joueur(new ControleurJoueurIA(pseudo, dialogue)));
                // } else {
                participants.push_back(new Joueur(new ControleurJoueurHumain(pseudo, dialogue)));
                pseudos_pris.insert(pseudo);
                // }
            }
        }

        return participants;
    }
    catch (...) {
        for (IParticipant* p : participants) {
            delete p;
        }
        throw;
    }
}

void ControleurPartie::initialiserPartie() {
    std::vector<IParticipant*> participants = creerParticipants();
    partie = creerPartie(participants);

    std::set<std::string> pseudos = partie->getPseudosParticipants();
    size_t indice_architecte = dialogue->demanderArchitecteChef(pseudos);
    partie->setArchitecteChef(indice_architecte);
}

void ControleurPartie::jouerManche() {
    dialogue->annoncerNouvelleManche();
    dialogue->afficherArchitecte(*partie->getArchitecteChef());

    size_t indice_debut = partie->getIndiceArchitecteChef();
    Table& chantier = *partie->getChantier();

    for (size_t i = 0; i < partie->getNbIParticipants(); ++i) {
        size_t indice = (indice_debut + i) % partie->getNbIParticipants();
        IParticipant* participant = partie->getIParticipant(indice);

        dialogue->annoncerTourIParticipant(*participant);
        participant->jouerTour(chantier);
    }

    partie->passerArchitecteChefSuivant();
}

void afficherResultats() {
    dialogue->annoncerFinPartie();

    for (size_t i = 0; i < partie->getNbIParticipants(); ++i) {
        IParticipant* participant = partie->getIParticipant(i);

        if (participant->estJoueur()) {
            Joueur* joueur = dynamic_cast<Joueur*>(participant);
            // int score = calculerScore(joueur->getCite());
            dialogue->afficherMessage(joueur->getPseudo() + " : [score]");
        }
        else {
            IllustreArchitecte* iA = dynamic_cast<IllustreArchitecte*>(participant);
            // int score = calculerScore(joueur->getCite());
            dialogue->afficherMessage(participant->getPseudo() + " : Adversaire");
        }
    }
}

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
    afficherResultats();
}
};
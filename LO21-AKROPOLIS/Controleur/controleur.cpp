#include "controleur.h"
#include "../Table/table.h"
#include "../Pioche/pioche.h"
#include "../Dialogue/dialogueStrategy.h"

Partie* ControleurPartie::creerPartie(const std::vector<IParticipant*>& participants, ModeScore mode) {
    if (participants.empty()) {
        throw AkropolisException("Il faut au moins un participant", "ControleurPartie");
    }

    Pioche* pioche = nullptr;
    Table* chantier = nullptr;

    try {
        Jeu& jeu = Jeu::getInstance(participants.size());
        pioche = new Pioche();
        chantier = new Table(participants.size(), jeu.getReservoir());

        return new Partie(participants, pioche, chantier, mode);
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
            std::string pseudo = dialogue->demanderPseudo();
            participants.push_back(
                new Joueur(new ControleurJoueur(pseudo, dialogue))
            );
            participants.push_back(new IllustreArchitecte());
        }
        else {
            size_t nb_joueurs = dialogue->demanderNombreJoueurs(2, 4);
            std::set<std::string> pseudos_pris;

            std::string pseudo = dialogue->demanderPseudo(pseudos_pris, 1);

            participants.push_back(new Joueur(new ControleurJoueur(pseudo, dialogue)));
            pseudos_pris.insert(pseudo);

            for (size_t i = 1; i < nb_joueurs; ++i) {
                std::string pseudo = dialogue->demanderPseudo(pseudos_pris, i + 1);

                participants.push_back(new Joueur(new ControleurJoueur(pseudo, dialogue)));
                pseudos_pris.insert(pseudo);
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
    ModeScore mode;
    int choix = dialogue->demanderModeScore();
    switch (choix) {
    case 1:  mode = ModeScore::Classique;
    case 2:  mode = ModeScore::Variante;
    case 3:  mode = ModeScore::Personnel;
    default:  mode = ModeScore::Classique;
    }
    partie = creerPartie(participants, mode);

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

void ControleurPartie::afficherResultats() {
    dialogue->annoncerFinPartie();
    dialogue->annoncerAffichageScore();

    auto calculateur = ScoreRegistry::instance().create(partie->getModeScore(), partie->getVariante());

    if (!calculateur) {
        throw AkropolisException("Mode de score non enregistre dans le registre", "ControleurPartie");
    }

    for (size_t i = 0; i < partie->getNbIParticipants(); ++i) {
        IParticipant* participant = partie->getIParticipant(i);

        if (participant->estJoueur()) {
            Joueur* joueur = dynamic_cast<Joueur*>(participant);
            ScoreBreakdown score = calculateur->compute(joueur->getCite().getGrille().getGrille(), joueur->getPierres());
            dialogue->afficherScore(*joueur, score);
        }
        else {
            IllustreArchitecte* iA = dynamic_cast<IllustreArchitecte*>(participant);
            // il est où le calcul du score de l'Illustre Architecte Nyla ????
            ScoreBreakdown score;
            dialogue->afficherScore(*iA, score);
        }
    }
}

void ControleurPartie::jouer() {
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
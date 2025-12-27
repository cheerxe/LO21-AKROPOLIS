#pragma once
#include "../Score/score_registry.h"
#include "../Jeu/jeu.h"
#include "../Sauvegarde/sauvegarde.h"


// Comment générer le Score ?

class Partie;
class IDialogueStrategy;

class ControleurPartie {
private:
    Partie* partie;
    IDialogueStrategy* dialogue;

    Partie* creerPartie(const std::vector<IParticipant*>& participants, ModeScore mode);

    std::vector<IParticipant*> creerParticipants();

    void initialiserPartie();

    void jouerManche();

    void afficherResultats();

public:
    ControleurPartie(IDialogueStrategy* d) : partie(nullptr), dialogue(d) {}

    ~ControleurPartie() { delete partie; }

    ControleurPartie(const ControleurPartie&) = delete;
    ControleurPartie& operator=(const ControleurPartie&) = delete;

    void jouer();
};



void lancerPartie(IDialogueStrategy& dialogue) {
    ControleurPartie controleur(&dialogue);
    controleur.jouer();
}
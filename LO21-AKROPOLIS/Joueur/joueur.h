#pragma once
#include "../Controleur/controleurJoueur.h"

class Joueur {
private:
    Cite cite;
    size_t pierres;
    IControleurJoueur* controleur;
    bool est_architecte_chef;

public:
    Joueur(IControleurJoueur* ctrl) : controleur(ctrl), est_architecte_chef(false), pierres(0) {}

    ~Joueur() {
        delete controleur;
    }

    // Pas de copie
    Joueur(const Joueur&) = delete;
    Joueur& operator=(const Joueur&) = delete;

    void jouerTour() { controleur->jouerTour(cite); }

    std::string getPseudo() const { return controleur->getPseudo(); }

    Cite& getCite() { return cite; }
    const Cite& getCite() const { return cite; }

    bool estArchitecteChef() const { return est_architecte_chef; }
    void setArchitecteChef(bool val) { est_architecte_chef = val; }
};
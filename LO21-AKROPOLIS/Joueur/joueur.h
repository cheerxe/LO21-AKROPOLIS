#pragma once
#include "../Cite/cite.h"
#include "../Controleur/controleurJoueur.h"

class Joueur {
private:
    Cite cite;
    size_t pierres;
    IControleurJoueur* controleur;
    bool est_architecte_chef;

public:
    Joueur(IControleurJoueur* ctrl) : cite(), controleur(ctrl), est_architecte_chef(false), pierres(0) {}

    ~Joueur() {
        delete controleur;
    }

    // Pas de copie
    Joueur(const Joueur&) = delete;
    Joueur& operator=(const Joueur&) = delete;

    void jouerTour(Table chantier) { controleur->jouerTour(cite, pierres, chantier); }

    std::string getPseudo() const { return controleur->getPseudo(); }

    Cite& getCite() { return cite; }
    const Cite& getCite() const { return cite; }

    bool estArchitecteChef() const { return est_architecte_chef; }
    void setArchitecteChef(bool val) { est_architecte_chef = val; }
};
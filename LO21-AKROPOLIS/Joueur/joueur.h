#pragma once
#include "../Cite/cite.h"
#include "../Table/table.h"
#include "../Controleur/controleurJoueur.h"

class IParticipant {
protected:
    IControleurParticipant* controleur;

public:
    IParticipant(IControleurParticipant* ctrl) : controleur(ctrl) {}
    virtual ~IParticipant() { delete controleur; }

    virtual void jouerTour(Table& chantier) = 0;
    virtual std::string getPseudo() const {
        return controleur->getPseudo();
    }
    virtual bool estJoueur() const = 0;
};



class Joueur : public IParticipant {
private:
    Cite cite;
    size_t pierres;

public:
    Joueur(IControleurJoueur* ctrl)
        : IParticipant(ctrl), pierres(0) {
    }

    void jouerTour(Table& chantier) override {
        IControleurJoueur* ctrl_joueur = dynamic_cast<IControleurJoueur*>(controleur); // Cast sûr (c'est un IControleurJoueur)
        ctrl_joueur->jouerTour(cite, pierres, chantier);
    }

    bool estJoueur() const override { return true; }

    const Cite& getCite() const { return cite; }
    size_t getPierres() const { return pierres; }

    static bool egalPlace(TypeHexagone t) { 
        return t == TypeHexagone::Place1 || t == TypeHexagone::Place2 || t == TypeHexagone::Place3; }



class IllustreArchitecte : public IParticipant {
private:
    std::vector<Tuile*> tuiles_collectees;

public:
    IllustreArchitecte(IControleurParticipant* ctrl) : IParticipant(ctrl) {}

    void jouerTour(Table& chantier) override {
        // Le contrôleur gère la logique du choix
        for (size_t i = 0; i < chantier.taille(); ++i) {
            const Tuile* t = chantier.regarder(i);
            if (egalPlace(t->getHex1().getType()) || ) 
        }
        Tuile* tuile = chantier.prendre(0);
        tuiles_collectees.push_back(tuile);
    }

    bool estJoueur() const override { return false; }

    const std::vector<Tuile*>& getTuiles() const {
        return tuiles_collectees;
    }

    ~IllustreArchitecte() {
        for (Tuile* t : tuiles_collectees) delete t;
    }
};
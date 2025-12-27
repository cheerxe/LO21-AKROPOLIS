#pragma once
#include "../Cite/cite.h"
#include "../Table/table.h"
#include "../Controleur/controleurJoueur.h"

enum class TypeHexagone;


class IParticipant {
protected:

public:
    IParticipant() {}
    virtual ~IParticipant() {}

    virtual void jouerTour(Table& chantier) = 0;
    virtual const std::string& getPseudo() const = 0;
    virtual bool estJoueur() const = 0;
};



class Joueur : public IParticipant {
private:
    Cite cite;
    size_t pierres;
    ControleurJoueur* controleur;

public:
    Joueur(ControleurJoueur* ctrl) : IParticipant(), pierres(0), controleur(ctrl) {}
    ~Joueur() { delete controleur; }

    void jouerTour(Table& chantier) override { controleur->jouerTour(cite, pierres, chantier); }

    bool estJoueur() const override { return true; }

    const Cite& getCite() const { return cite; }
    size_t getPierres() const { return pierres; }
    const std::string& getPseudo() const { return controleur->getPseudo(); }

};



class IllustreArchitecte : public IParticipant {
private:
    std::vector<Tuile*> tuiles_collectees;

    static bool egalPlace(TypeHexagone t) {
        return t == TypeHexagone::Place1 || t == TypeHexagone::Place2 || t == TypeHexagone::Place3;
    }

public:
    IllustreArchitecte() : IParticipant() {}

    void jouerTour(Table& chantier) override;

    bool estJoueur() const override { return false; }

    const std::vector<Tuile*>& getTuiles() const {
        return tuiles_collectees;
    }

    ~IllustreArchitecte() {
        for (Tuile* t : tuiles_collectees) delete t;
    }

    const std::string& getPseudo() const { return "Illustre Architecte"; }
};


#pragma once
#include "../Joueur/joueur.h"
#include "../Pioche/pioche.h"
#include <set>

class Table {
private:
    size_t capacite;
    std::vector<const Tuile*> visibles;
    std::vector<const Tuile*> reservoir;
public:
    Table(size_t nb_joueurs, std::vector<const Tuile*> res);
    size_t taille() const { return visibles.size(); }
    bool estVide() const { return visibles.empty(); }
    size_t cout(size_t i) const { return i; } // ?????????????????
    const Tuile* prendre(size_t i);
    const Tuile* regarder(size_t i) const;
    void recharger(Pioche& p);

    //rajoute pour sofiette
    const size_t getCapacite() const { return capacite; }
    size_t tailleReservoir() const { return reservoir.size(); }
    size_t tailleR() const { return reservoir.size(); }

private:
    void remplirDepuisReservoir();
    void remplirDepuisReservoirOuTas(Pioche& p);
    void regleUneTuileRestante(Pioche& p);

};

// PROBLEME A REGLER : JOUEURS N'EST PAS INITIALISE ("'Partie::joueurs' : impossible d'initialiser le membre") -> ca fait buguer tout le reste

class Partie {
private:
    std::vector<Joueur*> joueurs;
    Pioche* pioche;
    Table* chantier;
    size_t indice_architecte_chef;
    bool en_cours;

public: 
    // Constructeur prenant les joueurs déjà créés
    Partie(std::vector<Joueur*> joueurs_init, Pioche* pioche_init, Table* table_init) : joueurs(joueurs_init), pioche(pioche_init), 
        chantier(table_init), indice_architecte_chef(0), en_cours(false) {
        if (joueurs.empty()) {
            throw AkropolisException("Une partie doit avoir au moins un joueur", "Partie");
        }
    }

    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    ~Partie() {
        for (Joueur* joueur : joueurs) {
            delete joueur;
        }
        delete pioche;
    }

    size_t getNbJoueurs() const { return joueurs.size(); }

    Joueur* getJoueur(size_t indice) {
        if (indice >= joueurs.size()) {
            throw std::out_of_range("Indice joueur invalide");
        }
        return joueurs[indice];
    }

    const Joueur* getJoueur(size_t indice) const {
        if (indice >= joueurs.size()) {
            throw std::out_of_range("Indice joueur invalide");
        }
        return joueurs[indice];
    }

    Joueur* getArchitecteChef() { return joueurs[indice_architecte_chef]; }
    const Joueur* getArchitecteChef() const { return joueurs[indice_architecte_chef]; }

    size_t getIndiceArchitecteChef() const { return indice_architecte_chef; }

    Pioche* getPioche() { return pioche; }
    const Pioche* getPioche() const { return pioche; }

    std::set<std::string> getPseudosJoueurs() const {
        std::set<std::string> pseudos;
        for (const Joueur* joueur : joueurs) {
            pseudos.insert(joueur->getPseudo());
        }
        return pseudos;
    }

    const Table* getChantier() const { return chantier; }
    Table* getChantier() { return chantier; }


    void demarrer() {
        if (en_cours) {
            throw std::logic_error("La partie est déjà en cours");
        }
        en_cours = true;
    }

    void terminer() {
        en_cours = false;
    }

    void setArchitecteChef(size_t indice) {
        if (indice >= joueurs.size()) {
            throw std::out_of_range("Indice architecte invalide");
        }
        indice_architecte_chef = indice;
        joueurs[indice]->setArchitecteChef(true);
    }

    void passerArchitecteChefSuivant() {
        joueurs[indice_architecte_chef]->setArchitecteChef(false);
        indice_architecte_chef = (indice_architecte_chef + 1) % joueurs.size();
        joueurs[indice_architecte_chef]->setArchitecteChef(true);
    }

    
};

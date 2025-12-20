#pragma once
#include "../Joueur/joueur.h"
#include <set>

// PROBLEME A REGLER : JOUEURS N'EST PAS INITIALISE ("'Partie::joueurs' : impossible d'initialiser le membre") -> ca fait buguer tout le reste

class Partie {
private:
    std::vector<IParticipant*> joueurs;
    Pioche* pioche;
    Table* chantier;
    size_t indice_architecte_chef;
    bool en_cours;

public: 
    // Constructeur prenant les joueurs déjà créés
    Partie(std::vector<IParticipant*> joueurs_init, Pioche* pioche_init, Table* table_init) : joueurs(joueurs_init), pioche(pioche_init), 
        chantier(table_init), indice_architecte_chef(0), en_cours(false) {
        if (joueurs.empty()) {
            throw AkropolisException("Une partie doit avoir au moins un joueur", "Partie");
        }
    }

    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    ~Partie() {
        for (IParticipant* joueur : joueurs) {
            delete joueur;
        }
        delete pioche;
    }

    size_t getNbIParticipants() const { return joueurs.size(); }

    IParticipant* getIParticipant(size_t indice) {
        if (indice >= joueurs.size()) {
            throw std::out_of_range("Indice joueur invalide");
        }
        return joueurs[indice];
    }

    const IParticipant* getIParticipant(size_t indice) const {
        if (indice >= joueurs.size()) {
            throw std::out_of_range("Indice joueur invalide");
        }
        return joueurs[indice];
    }

    IParticipant* getArchitecteChef() { return joueurs[indice_architecte_chef]; }
    const IParticipant* getArchitecteChef() const { return joueurs[indice_architecte_chef]; }

    size_t getIndiceArchitecteChef() const { return indice_architecte_chef; }

    Pioche* getPioche() { return pioche; }
    const Pioche* getPioche() const { return pioche; }

    std::set<std::string> getPseudosParticipants() const {
        std::set<std::string> pseudos;
        for (const IParticipant* joueur : joueurs) {
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

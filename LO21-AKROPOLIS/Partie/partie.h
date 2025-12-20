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
    size_t cout(size_t i) const { return i; } // ???????????????????
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

class Partie {
private:
    std::vector<Joueur*> joueurs;
    Pioche* pioche;
    Table* chantier;
    size_t indice_architecte_chef;

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


    bool partieEnCours(fichier::path chemin) {
        std::ifstream fic(chemin, std::ios::binary);
        if (fic) {
            bool en_cours;
            fic.read(reinterpret_cast<char*>(&en_cours), sizeof(en_cours));
            return en_cours;
        }
        std::cerr << "erreur lors de l'ouverture du fichier ";
        return false;
    }

    void reprendrePartie(fichier::path chemin) {//reprendre une partie qui avait été mise en pause avec deserialisation

        if (chemin.empty()) {
            std::cerr << "chemin ouverture fichier vide";
            return;
        }
        std::ifstream fic(chemin, std::ios::binary);

        if (!fic) {//ereur ouverture du fichier
            std::cerr << "erreur lors de l ouverture du fichier ";
            return;
        }

        std::vector<char> buffer;//vector pour stocker les donnees lues
        fic.seekg(0, std::ios::end);//positionner le curseur a la fin du fichier
        size_t taille_fichier = fic.tellg();//recuperer la taille en bits du fichiers
        fic.seekg(0, std::ios::beg);//reposition deu curseur au tout debut du fichier

        buffer.resize(taille_fichier);
        fic.read(buffer.data(), taille_fichier);//on mets toute le contenu d fichier dans le vector buffer

        //recreation des joueur, de leur cite, des elements de leurs cite via boucle for et deserialize
        // recreer les tuiles
        // recreer le chantier
        //ensuite jouerManche

        fic.close();
    }

    // ---- Mutations (logique métier) ----
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

    // Logique métier d'une manche (sans interaction utilisateur)
    void executerManche(Table& chantier) {
        // Logique pure : 
        // - Piocher les tuiles nécessaires
        // - Distribuer aux joueurs
        // - etc.
        // SANS demander quoi que ce soit à l'utilisateur
    }
};

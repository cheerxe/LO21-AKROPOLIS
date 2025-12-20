#pragma once

#include "../Tuile/tuile.h"
#include <set>
#include <map>


struct Coord {
    int q, r;
    bool operator<(const Coord& other) const { return std::tie(q, r) < std::tie(other.q, other.r); }
    bool operator==(const Coord& other) const { return std::tie(q, r) == std::tie(other.q, other.r); }
};

class GrilleHexa {
protected:
    std::map<Coord, Hexagone> grille; // Map-key = Coord | protected car héritée

    /* Liste des 6 directions possibles*/
    static const std::vector<std::pair<int, int>>& directions() {
        static const std::vector<std::pair<int, int>> dirs = {
            {0, -2}, {1, -1}, {1, 1}, {0, 2}, {-1, 1}, {-1, -1}
        };
        return dirs;
    }

public:
    GrilleHexa() = default;
    GrilleHexa(const GrilleHexa& g) = default;
    GrilleHexa& operator=(const GrilleHexa& g) = default;
    virtual ~GrilleHexa() = 0;

    const Coord minCoord() const;
    const Coord maxCoord() const;

    // A supprimer revoir la structure
    std::map<Coord, Hexagone>& getGrille() { return grille; }
    const std::map<Coord, Hexagone>& getGrille() const { return grille; }

    bool estLibre(const Coord& c) const { return grille.find(c) == grille.end(); }

    /**
     * Calcule les 3 coordonnées que la tuile occuperait.
     *
     * @param centre Coordonnées de l'hexagone central
     * @param orientation Donne les coordonnées des deux autres hexagones
     */
    static std::vector<Coord> calculerPositionsTuile(const Coord& centre, int orientation);

    virtual void placerTuile(const Tuile& tuile, const Coord& centre, int orientation, size_t sens = 0);

    virtual void retirerTuile(const Coord& c1, const Coord& c2, const Coord& c3) { grille.erase(c1); grille.erase(c2); grille.erase(c3); }
};

std::ostream& operator<<(std::ostream& os, const GrilleHexa& grille);


class GrilleHexa_Niveau : public GrilleHexa {
private:
    std::vector<std::set<Coord>> groupes_tuiles;

public:
    GrilleHexa_Niveau() = default;

    virtual bool peutPlacerTuile(const Coord& centre, int orientation) const = 0;
    // virtual std::vector<std::pair<Coord, int>> toutesPositionsValides() const = 0;
    virtual std::set<Coord> toutesPositionsValides() const = 0;
    virtual std::set<int> toutesOrientationsValides(const Coord& centre) const = 0;

    bool memeTuile(const Coord& a, const Coord& b, const Coord& c) const {
        std::set<Coord> test{ a, b, c };
        for (const auto& groupe : groupes_tuiles) {
            if (groupe == test) return true;
        }
        return false;
    }

    void placerTuile(const Tuile& tuile, const Coord& centre, int orientation, size_t sens = 0) override;
    void retirerTuile(const Coord& c1, const Coord& c2, const Coord& c3) { grille.erase(c1); grille.erase(c2); grille.erase(c3); }

    const std::vector<std::set<Coord>> getGroupesTuiles() const {//rajout d un getter pour obtenir le groupes_tuiles ?
        return groupes_tuiles;
    }
};


class GrilleHexa_Niv1 : public GrilleHexa_Niveau {

    void initialiserGrilleDeBase() {
        grille.insert({ {0, 0}, Place(TypeQuartier::Habitation, NbEtoiles::une) });
        grille.insert({ {1, 1}, Carriere() });
        grille.insert({ {0, -2}, Carriere() });
        grille.insert({ {-1, 1}, Carriere() });
    }

public:

    GrilleHexa_Niv1() : GrilleHexa_Niveau() { initialiserGrilleDeBase(); }

    /**
    * Vérifie si les 3 coordonnées de la tuile sont libres et si au moins une de ces coordonnées est adjacente à un hexagone déjà posé.

    @param centre Coordonnées de l'hexagone central
    @param orientation Donne les coordonnées des deux autres hexagones
    */
    bool peutPlacerTuile(const Coord& centre, int orientation) const;

    /**
    Parcourt tous les hexagones déjà posés.

    - Pour chaque hexagone, teste les 6 directions autour de lui comme centres potentiels.

    - Pour chaque centre, teste les 6 orientations possibles.

    @return toutes les combinaisons (centre, orientation) où la tuile peut être posée.
    */
    //std::vector<std::pair<Coord, int>> toutesPositionsValides() const;
    std::set<Coord> toutesPositionsValides() const override;
    std::set<int> toutesOrientationsValides(const Coord& centre) const override;

};


class GrilleHexa_NivPlus : public GrilleHexa_Niveau {
private:
    const GrilleHexa_Niveau* grille_precedente;

public:

    GrilleHexa_NivPlus(const GrilleHexa_Niveau* precedente) : GrilleHexa_Niveau(), grille_precedente(precedente) {}

    /**
    * Vérifie si les 3 coordonnées de la tuile sont libres et si toutes ces 3 coordonnées sont occupées sur la grille inférieure

    @param centre Coordonnées de l'hexagone central
    @param orientation Donne les coordonnées des deux autres hexagones
    */
    bool peutPlacerTuile(const Coord& centre, int orientation) const;

    /**
    Parcourt tous les hexagones déjà posés.

    - Pour chaque hexagone, teste les 6 directions autour de lui comme centres potentiels.

    - Pour chaque centre, teste les 6 orientations possibles.

    @return toutes les combinaisons (centre, orientation) où la tuile peut être posée.
    */
    //std::vector<std::pair<Coord, int>> toutesPositionsValides() const;
    std::set<Coord> toutesPositionsValides() const override;
    std::set<int> toutesOrientationsValides(const Coord& centre) const override;

    const GrilleHexa* getGrillePrecedente() const {
        return grille_precedente;
    };//ajout d'un getter pour grille_precedente pour serialiser l'objet pour sofia
};


class GrilleHexa_Plate : public GrilleHexa {
public:
    GrilleHexa_Plate() = default;

    void placerHexagone(const Coord& centre, const Hexagone& hex) { grille.insert_or_assign(centre, hex); }
    // ne pas utiliser grille[centre]=hex (utilises le constructeur par défaut de la classe Hexagone)

};
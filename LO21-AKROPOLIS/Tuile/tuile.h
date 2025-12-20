#pragma once
#include "../exception.h"
#include "../couleur.h"
#include <cstddef>
#include <string>
#include <vector>
#include <iostream>


enum class TypeQuartier { Habitation, Marche, Caserne, Temple, Jardin };
enum class TypeHexagone { Carriere, Quartier, Place1, Place2, Place3, None };
enum class NbEtoiles { une = 1, deux = 2, trois = 3 };


inline Couleur matchColor(TypeQuartier type) {
    switch (type) {
    case TypeQuartier::Habitation:   return Couleur::Bleu;
    case TypeQuartier::Marche:    return Couleur::Jaune;
    case TypeQuartier::Caserne:   return Couleur::Rouge;
    case TypeQuartier::Temple:    return Couleur::Violet;
    case TypeQuartier::Jardin: return Couleur::Vert;
    default:					return Couleur::Reset;
    }
}

class Hexagone {
private:
    Couleur couleur;
    TypeHexagone type;

protected:
    Hexagone(Couleur c, TypeHexagone t) : couleur(c), type(t) {}

public:
    virtual ~Hexagone() = default;

    Couleur getCouleur() const { return couleur; }
    TypeHexagone getType() const { return type; }
};


class Quartier : public Hexagone {
private:
    TypeQuartier type_quartier;

public:
    Quartier(TypeQuartier t) : Hexagone(matchColor(t), TypeHexagone::Quartier), type_quartier(t) {}

};

class Carriere : public Hexagone {
public:
    Carriere() : Hexagone(Couleur::Gris, TypeHexagone::Carriere) {}
};

class Place : public Hexagone {
private:
    TypeQuartier type_quartier;
    NbEtoiles nb_etoiles;

    static TypeHexagone typeFromEtoiles(NbEtoiles e) {
        switch (e) {
        case NbEtoiles::une:   return TypeHexagone::Place1;
        case NbEtoiles::deux:  return TypeHexagone::Place2;
        case NbEtoiles::trois: return TypeHexagone::Place3;
        }
        return TypeHexagone::Place1;
    }

public:
    Place(TypeQuartier t, NbEtoiles e) : Hexagone(matchColor(t), typeFromEtoiles(e)), type_quartier(t), nb_etoiles(e) {}
};

class HexagoneAffichage : public Hexagone {
private:
    size_t niv;
public:
    HexagoneAffichage(size_t n) : Hexagone(Couleur::Cyan, TypeHexagone::None), niv(n) {}
    HexagoneAffichage(size_t n, const Hexagone& h) : Hexagone(h.getCouleur(), TypeHexagone::None), niv(n) {}
    size_t getNiv() const { return niv; }
};

class Tuile {
private:
    const Hexagone hex1;
    const Hexagone hex2;
    const Hexagone hex3;

public:
    Tuile(const Hexagone& h1, const Hexagone& h2, const Hexagone& h3) : hex1(h1), hex2(h2), hex3(h3) {}

    Tuile(const Tuile& t) = default;
    ~Tuile() = default;

    // Getters
    const Hexagone& getHex1() const { return hex1; }
    const Hexagone& getHex2() const { return hex2; }
    const Hexagone& getHex3() const { return hex3; }

    // Retourne les hexagones dans un ordre donné selon le sens (rotation)
    std::vector<const Hexagone*> getHexagonesDansSens(size_t sens) const {
        if (sens == 1) return { &hex2, &hex3, &hex1 };
        else if (sens == 2) return { &hex3, &hex1, &hex2 };
        return { &hex1, &hex2, &hex3 };
    }
};

// Déclaration de l'opérateur << (implémentation dans affichageTuile.cpp)
std::ostream& operator<<(std::ostream& os, const Tuile& t);
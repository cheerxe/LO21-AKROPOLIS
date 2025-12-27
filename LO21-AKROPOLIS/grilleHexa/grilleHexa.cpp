#include "grilleHexa.h"
#include "../Tuile/tuile.h"
#include <algorithm> // pour std::sort

GrilleHexa::~GrilleHexa() = default;

const Coord GrilleHexa::minCoord() const {
    int minQ = -1, minR = -1;
    for (typename std::map<Coord, Hexagone>::const_iterator it = grille.begin(); it != grille.end(); ++it) {
        const Coord& coord = it->first;
        if (coord.q < minQ) minQ = coord.q;
        if (coord.r < minR) minR = coord.r;
    }
    return { minQ, minR };
}

const Coord GrilleHexa::maxCoord() const {
    int maxQ = 1, maxR = 0;
    for (typename std::map<Coord, Hexagone>::const_iterator it = grille.begin(); it != grille.end(); ++it) {
        const Coord& coord = it->first;
        if (coord.q > maxQ) maxQ = coord.q;
        if (coord.r > maxR) maxR = coord.r;
    }
    return { maxQ, maxR };
}

std::vector<Coord> GrilleHexa::calculerPositionsTuile(const Coord& centre, int orientation) {
    const auto& dirs = directions();
    Coord d1 = { centre.q + dirs[orientation].first, centre.r + dirs[orientation].second };
    Coord d2 = { centre.q + dirs[(orientation + 1) % 6].first, centre.r + dirs[(orientation + 1) % 6].second };
    return { centre, d1, d2 };
}

void GrilleHexa::placerTuile(const Tuile& tuile, const Coord& centre, int orientation, size_t sens) {
    std::vector<Hexagone> hex = { tuile.getHex1(), tuile.getHex2(), tuile.getHex3() };

    std::vector<Coord> positions = calculerPositionsTuile(centre, orientation);
    std::sort(positions.begin(), positions.end(), [](const Coord& a, const Coord& b) { return a.r < b.r; }); // trie selon r

    grille.insert_or_assign(positions[0], hex[(0 + sens) % 3]);
    grille.insert_or_assign(positions[1], hex[(1 + sens) % 3]);
    grille.insert_or_assign(positions[2], hex[(2 + sens) % 3]);
}

void GrilleHexa_Niveau::placerTuile(const Tuile& tuile, const Coord& centre, int orientation, size_t sens) {
    std::vector<Hexagone> hex = { tuile.getHex1(), tuile.getHex2(), tuile.getHex3() };

    std::vector<Coord> positions = calculerPositionsTuile(centre, orientation);
    std::sort(positions.begin(), positions.end(), [](const Coord& a, const Coord& b) { return a.r < b.r; }); // trie selon r

    grille.insert_or_assign(positions[0], hex[(0 + sens) % 3]);
    grille.insert_or_assign(positions[1], hex[(1 + sens) % 3]);
    grille.insert_or_assign(positions[2], hex[(2 + sens) % 3]);

    groupes_tuiles.emplace_back(std::set<Coord>{positions[0], positions[1], positions[2]});
}

bool GrilleHexa_Niv1::peutPlacerTuile(const Coord& centre, int orientation) const {
    auto positions = calculerPositionsTuile(centre, orientation);
    for (const Coord& c : positions) {
        if (!estLibre(c)) return false;
    }

    for (const Coord& c : positions) {
        for (const auto& dir : directions()) {
            Coord voisin = { c.q + dir.first, c.r + dir.second };
            if (!estLibre(voisin)) return true;
        }
    }
    return false;
}

std::set<Coord> GrilleHexa_Niv1::toutesPositionsValides() const {
    std::set<Coord> positions_valides;
    for (const auto& emplacement : grille) {
        const Coord& coord = emplacement.first;
        for (const auto& dir : directions()) {
            Coord centre = { coord.q + dir.first, coord.r + dir.second };
            for (int orientation = 0; orientation < 6; orientation++) {
                if (peutPlacerTuile(centre, orientation)) {
                    positions_valides.insert(centre);
                    break; // Pas besoin de vérifier les autres orientations
                }
            }
        }
    }

    return positions_valides;
}

std::set<int> GrilleHexa_Niv1::toutesOrientationsValides(const Coord& centre) const {
    std::set<int> orientations_valides;
    for (int orientation = 0; orientation < 6; orientation++) {
        if (peutPlacerTuile(centre, orientation)) {
            orientations_valides.insert(orientation);
        }
    }
    return orientations_valides;
}


bool GrilleHexa_NivPlus::peutPlacerTuile(const Coord& centre, int orientation) const {
    auto positions = calculerPositionsTuile(centre, orientation);
    for (const Coord& c : positions) {
        if (!estLibre(c)) return false;
        if (grille_precedente->estLibre(c)) return false;
    }
    return true;
}


std::set<Coord> GrilleHexa_NivPlus::toutesPositionsValides() const {
    std::set<Coord> positions_valides;
    for (const auto& emplacement : grille_precedente->getGrille()) {
        const Coord& centre = emplacement.first;
        for (int orientation = 0; orientation < 6; orientation++) {
            std::vector<Coord> coords = calculerPositionsTuile(centre, orientation);
            Coord c1 = coords[0], c2 = coords[1], c3 = coords[2];
            if (peutPlacerTuile(centre, orientation) && !grille_precedente->memeTuile(c1, c2, c3)) {
                positions_valides.insert(centre);
                break;
            }
        }
    }
    return positions_valides;
}


std::set<int> GrilleHexa_NivPlus::toutesOrientationsValides(const Coord& centre) const {
    std::set<int> orientations_valides;
    for (int orientation = 0; orientation < 6; orientation++) {
        std::vector<Coord> coords = calculerPositionsTuile(centre, orientation);
        Coord c1 = coords[0], c2 = coords[1], c3 = coords[2];
        if (peutPlacerTuile(centre, orientation) && !grille_precedente->memeTuile(c1, c2, c3)) {
            orientations_valides.insert(orientation);
        }
    }
    return orientations_valides;
}

/*
void GrilleHexa_Plate::showPositionsPossibles(const std::set<Coord> unique_coords_center, size_t niv) const {
    GrilleHexa_Plate show_placement = *this;

    size_t ind = 1;
    std::cout << "\n" << std::string(46, '.') << " Placements possibles : " << std::string(46, '.');
    for (const auto& it : unique_coords_center) show_placement.placerHexagone(it, HexagoneAffichage(niv, std::to_string(ind++)));
    std::cout << show_placement;
}*/

/*
void GrilleHexa_Plate::showOrientationsPossibles(const Coord& centre, std::set<int> orient_possible, size_t niv) const {
    GrilleHexa_Plate grille_actuelle = *this, show_orient;
    Hexagone hvide = HexagoneAffichage(niv, "...");

    size_t ind = 1;
    std::cout << "\n" << std::string(45, '.') << " Orientations possibles : " << std::string(45, '.') << "\n";
    if (orient_possible.size() == 1) {
        std::cout << "\n Une seule orientation possible.";
        show_orient = grille_actuelle;
        show_orient.placerTuile(Tuile(hvide, hvide, hvide), centre, *(orient_possible.begin()));
        std::cout << show_orient;
    }
    else {
        for (const auto& orient : orient_possible) {
            show_orient = grille_actuelle;
            show_orient.placerTuile(Tuile(hvide, hvide, hvide), centre, orient);
            std::cout << "\nOption " << ind++ << " : ";
            std::cout << show_orient;
        }
    }
}*/
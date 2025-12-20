#include "cite.h"

bool Cite::niveauSupPossible() const {
    if (niveaux.empty()) {
        GrilleHexa_NivPlus niv_sup(&(niv1));
        if (!niv_sup.toutesPositionsValides().empty()) {
            return true;
        }
    }
    else {
        GrilleHexa_NivPlus niv_sup(niveaux.back());
        if (!niv_sup.toutesPositionsValides().empty()) {
            return true;
        }
    }
    return false;
}

std::set<size_t> Cite::tousNiveauxDisponibles() const {
    std::set<size_t> niveaux_possibles = { 1 };

    for (int i = 0; i < niveaux.size(); i++) {
        if (!niveaux[i]->toutesPositionsValides().empty()) niveaux_possibles.insert(i + 2);
    }
    if (niveauSupPossible()) niveaux_possibles.insert(niveaux.size() + 2);

    return niveaux_possibles;
}

std::set<Coord> Cite::getPositionsValides(size_t niv) const {
    if (niv == 1) return niv1.toutesPositionsValides();
    else return niveaux[niv - 2]->toutesPositionsValides();
}

std::set<int> Cite::getOrientationsPossibles(const Coord& centre, size_t niv) const {
    if (niv == 1) return niv1.toutesOrientationsValides(centre);
    else return niveaux[niv - 2]->toutesOrientationsValides(centre);
}

void Cite::placerTuile(const Tuile& t, size_t niv, const Coord& centre, int orientation, size_t sens) {
    if (niv == 1) niv1.placerTuile(t, centre, orientation, sens);
    else niveaux[niv - 2]->placerTuile(t, centre, orientation, sens);

    HexagoneAffichage h1(niv, t.getHex1()), h2(niv, t.getHex2()), h3(niv, t.getHex3());
    Tuile taffichage(h1, h2, h3);
    grille_plate.placerTuile(taffichage, centre, orientation, sens);
}


void Cite::retirerTuile(size_t niv, const Coord& centre, int orientation) {
    if (niv < 1) throw AkropolisException("Il n'existe pas de niveau inférieur à 1.", "Cité");

    std::vector<Coord> positions = GrilleHexa::calculerPositionsTuile(centre, orientation);
    const Coord c1 = positions[0], c2 = positions[1], c3 = positions[2];

    if (!(this->niveaux[niv - 1]->estLibre(c1) && this->niveaux[niv - 1]->estLibre(c2) && this->niveaux[niv - 1]->estLibre(c3))) {
        throw AkropolisException("La pièce a été recouverte - retrait impossible", "Cité");
    }

    if (niv == 1) {
        niv1.retirerTuile(c1, c2, c3);
        //grille_finale.retirerTuile(c1, c2, c3);
        grille_plate.retirerTuile(c1, c2, c3);
        /*
        std::map<Coord, Hexagone>& grille = niv1.getGrille(), finale = grille_finale.getGrille(), flat_grille = grille_plate.getGrille();
        for (const auto& it : positions) {
            grille.erase(it);
            finale.erase(it);
            flat_grille.erase(it);
        }*/
    }
    // si niv > 1 : placer sur grille_plate l'hexagone du niveau inférieur
    else if (niv == 2) {
        // placer sur grille_plate l'hexagone du niveau inférieur (ici niv1)
    }
    else {
        std::map<Coord, Hexagone>& grille = niv1.getGrille(), under_grille = niveaux[niv - 3]->getGrille();
        //finale = grille_finale.getGrille();

        GrilleHexa_Plate flat_grille = grille_plate;
        niv1.retirerTuile(c1, c2, c3);
        //grille_finale.retirerTuile(c1, c2, c3);
        grille_plate.retirerTuile(c1, c2, c3);


        for (const auto& it : positions) {
            auto found = under_grille.find(it);
            if (found != under_grille.end()) {
                //finale.insert_or_assign(it, Hexagone(found->second));
                flat_grille.placerHexagone(it, HexagoneAffichage(niv, found->second));
            }
            else { throw AkropolisException("La pièce ne recouvrait rien ????", "Cité"); }
        }
    }

    // retirer niveau si besoin
    if (!niveaux.empty()) {
        if (niveaux.back()->getGrille().empty()) niveaux.pop_back();
    }
}
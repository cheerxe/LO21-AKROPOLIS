#pragma once
#include "../grilleHexa/grilleHexa.h"


class Cite {
	GrilleHexa_Niv1 niv1;
	std::vector<GrilleHexa_NivPlus*> niveaux;
	// Grille pour l'affichage de la cité vue du dessus (la seule à attribuer des niveaux aux hexagones)
	GrilleHexa_Plate grille_plate;

	bool niveauSupPossible() const;

	std::set<size_t> tousNiveauxDisponibles() const;

public:
	Cite() {
		// Initialiser la grille plate
		for (const auto& it : niv1.getGrille()) {
			Coord c = it.first; const Hexagone& h = it.second;
			grille_plate.placerHexagone(c, HexagoneAffichage(1, h));
		}
	}

	~Cite() { for (size_t i = 0; i < niveaux.size(); i++) delete niveaux[i]; }

	Cite(const Cite&) = delete;
	Cite& operator=(const Cite&) = delete;

	const GrilleHexa_Plate& getGrille() const { return grille_plate; }

	std::set<size_t> getNiveauxDisponibles() const { return tousNiveauxDisponibles(); }

	std::set<Coord> getPositionsValides(size_t niv) const;
	std::set<int> getOrientationsPossibles(const Coord& centre, size_t niv) const;

	void placerTuile(const Tuile& t, size_t niv, const Coord& centre, int orientation, size_t sens);

	// A revoir ?
	void retirerTuile(size_t niv, const Coord& centre, int orientation);

	// Seulement pour la sérialisation
	const GrilleHexa_Plate& getGrilleHexaPlate() const { return grille_plate; }
	const GrilleHexa_Niv1& getGrilleHexaNiv1() const { return niv1; }
	const std::vector<GrilleHexa_NivPlus*>& getNiveaux() const { return niveaux; }
};

/*
class Cite {

	GrilleHexa_Niv1 niv1;
	std::vector<GrilleHexa_NivPlus*> niveaux;
	// Grille pour l'affichage de la cité vue du dessus (la seule à attribuer des niveaux aux hexagones)
	GrilleHexa_Plate grille_plate;

	bool niveauSupPossible() const;

	std::set<size_t> tousNiveauxDisponibles() const;

	size_t askNiveauNextTuile() const;

	std::pair<const Coord, std::set<int>> askPositionNextTuile(std::vector<std::pair<Coord, int>> positions_valides, size_t niv) const;

	int askOrientationNextTuile(const Coord& centre, std::set<int> orient_possible, size_t niv) const;

	size_t askSensNextTuile(const Tuile& tuile, int orientation) const;

public:

	Cite() {
		// Initialiser la grille plate
		for (const auto& it : niv1.getGrille()) {
			Coord c = it.first; Hexagone h = it.second;
			grille_plate.placerHexagone(c, HexagoneAffichage(1, h));
		}
	}

	~Cite() { for (size_t i = 0; i < niveaux.size(); i++) delete niveaux[i]; }

	Cite(const Cite&) = delete;
	Cite& operator=(const Cite&) = delete;

	void askEtPlaceTuile(const Tuile& t);

	const GrilleHexa_Plate& getGrille() const { return grille_plate; }
	const GrilleHexa_Plate& getGrillePlate() const { return grille_plate; }

	const GrilleHexa_Niv1& getNiv1() const { return niv1; }


	//const std::vector<GrilleHexa_NivPlus*> getNiveaux() const { return niveaux; }

};*/
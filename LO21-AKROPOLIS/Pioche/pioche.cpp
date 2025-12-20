#include "pioche.h"

// ==============================      CODE AXEL    ==================================

/*==========================  Tas  ==========================*/


Tas::Tas(int nbjoueurs) : nb_tuiles(static_cast<size_t>(nbjoueurs + 1)) {
	tuiles.reserve(nb_tuiles);
}
Tas::~Tas() {} // pas de delete

void Tas::pushTuile(const Tuile* t) {
	if (tuiles.size() < nb_tuiles) tuiles.push_back(t);
}

/*=========================  Pioche  ========================*/


void Pioche::ajouterTas(const Tas* t) {
	tas.push_back(t);
	curseur.push_back(0);
}

bool Pioche::estVide() const {
	for (size_t i = 0; i < tas.size(); ++i)
		if (curseur[i] < tas[i]->getNbTuiles()) return false;
	return true;
}

const Tuile* Pioche::piocherDepuisTas(size_t i) {
	if (i >= tas.size()) return nullptr;
	const Tas* t = tas[i];
	size_t& c = curseur[i];
	if (c >= t->getNbTuiles()) return nullptr;
	const Tuile* tu = t->getTuilesTas(c);
	++c; // on �consomme� logiquement 1 tuile de ce tas
	return tu;
}

int Pioche::prendreIndiceTasNonVide() const {
	for (size_t i = 0; i < tas.size(); ++i)
		if (curseur[i] < tas[i]->getNbTuiles()) return static_cast<int>(i);
	return -1;
}
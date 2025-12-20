#include "partie.h"

Table::Table(size_t nb_joueurs, std::vector<const Tuile*> res)
    : capacite(nb_joueurs + 2), visibles(), reservoir(std::move(res)) {
    remplirDepuisReservoir(); // initialisation du chantier
}

void Table::remplirDepuisReservoir() {
    while (visibles.size() < capacite && !reservoir.empty()) {
        visibles.push_back(reservoir.front());
        reservoir.erase(reservoir.begin()); // équivalent pop_front() (sans deque)
    }
}

void Table::remplirDepuisReservoirOuTas(Pioche& p) {
    // 1) d'abord le réservoir
    remplirDepuisReservoir();
    // 2) puis des tas si nécessaire
    while (visibles.size() < capacite) {
        int idx = p.prendreIndiceTasNonVide();
        if (idx < 0) break;
        while (visibles.size() < capacite) {
            const Tuile* t = p.piocherDepuisTas(static_cast<size_t>(idx));
            if (!t) break;
            visibles.push_back(t);
        }
    }
}

void Table::regleUneTuileRestante(Pioche& p) {
    if (visibles.size() != 1) return;
    // déplacer explicitement la seule tuile en position 0 (déjà le cas)
    const Tuile* seule = visibles.front();
    visibles.erase(visibles.begin());
    visibles.insert(visibles.begin(), seule);
    // compléter
    remplirDepuisReservoirOuTas(p);
}

const Tuile* Table::regarder(size_t i) const {
    if (i >= visibles.size()) return nullptr; // Sécurité pour ne pas planter
    return visibles[i]; // On renvoie juste le pointeur sans supprimer
}

const Tuile* Table::prendre(size_t i) {
    if (i >= visibles.size()) return nullptr;
    const Tuile* t = visibles[i];
    visibles.erase(visibles.begin() + static_cast<long>(i)); // décale à gauche
    return t;
}

void Table::recharger(Pioche& p) {
    if (visibles.empty()) {
        remplirDepuisReservoirOuTas(p);
    }
    else if (visibles.size() == 1) {
        regleUneTuileRestante(p);
    }
}

void Table::print() const {
    std::cout << "\nChantier (" << visibles.size() << "/" << capacite << ") [cout=index]\n";
    /*
    for (size_t i = 0; i < visibles.size(); ++i) {
        auto* t = visibles[i];
        std::cout << "  [" << i << "] " << t->id
            << "  (minJ=" << t->minPlayers << ", cout=" << i << ")\n";
    }
    std::cout << "Reservoir restant: " << reservoir.size() << "\n";*/
}

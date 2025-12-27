#pragma once
#include <string>

class Table;
class Cite;
class IDialogueStrategy;


class ControleurJoueur {
private:
    IDialogueStrategy* dialogue;
    std::string pseudo;

    const Tuile& choisirTuile(size_t& pierres, Table chantier);

public:
    ControleurJoueur(const std::string& p, IDialogueStrategy* d) : pseudo(p), dialogue(d) {}

    std::string getPseudo() const { return pseudo; }

    void jouerTour(Cite& cite, size_t& pierres, Table chantier);

};
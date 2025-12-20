// affichageTuile.h
#pragma once
#include "affichageConsole.h"


namespace TuileRendering {
    using namespace ConsoleRendering;

    // Fonction helper pour obtenir l'identifiant d'un hexagone
    inline std::string getContenu(const Hexagone& hex) {
        return matchTexte(hex.getType());
    }

    // Fonction helper pour obtenir l'identifiant avec niveau (pour affichage sur grille)
    inline std::string getContenuAvecNiveau(const HexagoneAffichage& hex) {
        return matchTexte(hex.getType()) + " #" + std::to_string(hex.getNiv());
    }


    inline void afficherTuile(const Tuile& tuile) {
        std::vector<Hexagone> hexagones = { tuile.getHex1(),tuile.getHex2(), tuile.getHex3() };

        std::vector<std::string> hex = { justify(getContenu(hexagones[0])), justify(getContenu(hexagones[1])), justify(getContenu(hexagones[2])) };
        std::vector<Couleur> col = { hexagones[0].getCouleur(), hexagones[1].getCouleur(), hexagones[2].getCouleur() };

        int nb_tiret = 4 - (LARG_HEXAGONE % 2);
        int space = (LARG_HEXAGONE - nb_tiret) / 2;

        std::string vide = std::string(LARG_HEXAGONE, ' ');
        std::string adjacent = std::string(space, ' ') + std::string(nb_tiret, '-') + std::string(space, ' ');


        std::cout << vide << codeAnsi(col[0]) << "< " << hex[0] << " >\n" << codeAnsi(Couleur::Reset);
        std::cout << vide << adjacent << codeAnsi(col[1]) << "< " << hex[1] << " >\n";
        std::cout << vide << codeAnsi(col[2]) << "< " << hex[2] << " >\n" << codeAnsi(Couleur::Reset);
    }

    inline void afficherTousSens(const Tuile& tuile, int orientation) {
        bool orientation_impaire = (orientation % 2 == 1);

        std::vector<Hexagone> hexagones = { tuile.getHex1(),tuile.getHex2(), tuile.getHex3() };

        std::vector<std::string> hex = { justify(getContenu(hexagones[0])), justify(getContenu(hexagones[1])), justify(getContenu(hexagones[2])) };
        std::vector<Couleur> col = { hexagones[0].getCouleur(), hexagones[1].getCouleur(), hexagones[2].getCouleur() };

        std::cout << "\n" << std::string(50, '.') << " Sens possibles : " << std::string(50, '.') << "\n\n";

        int nb_tiret = 4 - (LARG_HEXAGONE % 2);
        int space = (LARG_HEXAGONE - nb_tiret) / 2;

        std::string tab = std::string(LARG_HEXAGONE * 3 / 2, ' ');
        std::string vide = std::string(LARG_HEXAGONE, ' ');
        std::string adjacent = std::string(space, ' ') + std::string(nb_tiret, '-') + std::string(space, ' ');

        std::cout << "    Option 1 : " << tab << vide
            << "Option 2 : " << tab << vide
            << " Option 3 : \n";


        if (orientation_impaire) {
            std::cout << vide << vide << codeAnsi(col[0]) << "< " << hex[0] << " >" << tab << vide << codeAnsi(col[1]) << "< " << hex[1] << " >" << tab << vide << codeAnsi(col[2]) << "< " << hex[2] << " >\n";
            std::cout << vide << codeAnsi(col[1]) << "< " << hex[1] << " >" << codeAnsi(Couleur::Reset) << adjacent << tab << codeAnsi(col[2]) << "< " << hex[2] << " >";
            std::cout << codeAnsi(Couleur::Reset) << adjacent << tab << codeAnsi(col[0]) << "< " << hex[0] << " >" << codeAnsi(Couleur::Reset) << adjacent << "\n";
            std::cout << vide << vide << codeAnsi(col[2]) << "< " << hex[2] << " >" << tab << vide << codeAnsi(col[0]) << "< " << hex[0] << " >" << tab << vide << codeAnsi(col[1]) << "< " << hex[1] << " >\n";
            std::cout << codeAnsi(Couleur::Reset);
        }
        else {
            std::cout << vide << codeAnsi(col[0]) << "< " << hex[0] << " >" << tab << vide << codeAnsi(col[1]) << "< " << hex[1] << " >" << tab << vide << codeAnsi(col[2]) << "< " << hex[2] << " >\n";
            std::cout << vide << codeAnsi(Couleur::Reset) << adjacent << codeAnsi(col[1]) << "< " << hex[1] << " >" << tab << codeAnsi(Couleur::Reset) << adjacent;
            std::cout << codeAnsi(col[2]) << "< " << hex[2] << " >" << tab << codeAnsi(Couleur::Reset) << adjacent << codeAnsi(col[0]) << "< " << hex[0] << " >\n";
            std::cout << vide << codeAnsi(col[2]) << "< " << hex[2] << " >" << tab << vide << codeAnsi(col[0]) << "< " << hex[0] << " >" << tab << vide << codeAnsi(col[1]) << "< " << hex[1] << " >\n";
            std::cout << codeAnsi(Couleur::Reset);
        }
    }

    inline void afficherChantier(std::vector<Tuile> chantier) {
        for (const auto& it : chantier) {
            afficherTuile(it);
            std::cout << "\n";
        }
    }
}

inline std::ostream& operator<<(std::ostream& os, const Tuile& t) {
    TuileRendering::afficherTuile(t);
    return os;
}
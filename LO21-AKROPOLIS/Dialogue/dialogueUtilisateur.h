#pragma once

#include <iostream>
#include <sstream> // ajout�
#include <string>
#include <set>
#include <limits>
#include "../exception.h"
#include "../couleur.h"
#include "../Affichage/affichageConsole.h"


namespace DialogueUtilisateur {
    // Gestion des erreurs de buffer
    inline void gestionErreurBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    template<typename T>
    void afficherOptions(const std::set<T>& options) {
        size_t i = 0, taille = options.size();
        for (auto it = options.begin(); it != options.end(); ++it, ++i) {
            if (i < taille - 1) std::cout << *it << ", ";
            else std::cout << *it;
        }
        std::cout << "\n";
    }

    template<typename T>
    void afficherMenu(const std::set<T>& options) {
        size_t i = 1;
        for (auto it = options.begin(); it != options.end(); ++it, ++i) {
            std::ostringstream ss;
            ss << i << ". " << *it;
            std::cout << "\n" << ss.str();
        }
        std::cout << "\n";
    }

    template<typename T>
    void afficherMenu(const std::vector<T>& options) {
        size_t i = 1;
        for (auto it = options.begin(); it != options.end(); ++it, ++i) {
            std::ostringstream ss;
            ss << i << ". " << *it;
            std::cout << "\n" << ss.str();
        }
        std::cout << "\n";
    }

    // demanderInformation Renommée en demanderTexte
    template<typename T>
    T demanderTexte(const std::string& question) {
        T choix;
        while (true) {
            std::cout << question;
            std::cin >> choix;

            if (std::cin.fail()) {
                gestionErreurBuffer();
                std::cout << "Choix invalide. Veuillez reessayer : ";
            }
            else {
                return choix;
            }
        }
    }

    // Question avec choix num�rique dans un intervalle
    template<typename T>
    T demanderChoixNumerique(const std::string& question, T min, T max) {
        T choix;
        while (true) {
            std::cout << question;
            std::cin >> choix;

            if (std::cin.fail() || choix < min || choix > max) {
                gestionErreurBuffer();
                std::cout << "Veuillez entrer un nombre entre " << min << " et " << max << ".\n";
            }
            else {
                return choix;
            }
        }
    }

    // Question avec formatage automatique "parmi X, Y et Z"
    template<typename T>
    T demanderChoixParmi(const std::string& question, const std::set<T>& options) {
        if (options.empty()) throw AkropolisException("Ensemble d'options vide", "DialogueUtilisateur");

        std::string select;
        if (options.size() == 2) {
            select = " entre " + std::to_string(*(options.begin())) + " ou " + std::to_string(*(++options.begin())) + " : ";
        }
        else {
            select = " parmi ";

            size_t i = 0, taille = options.size();
            for (auto it = options.begin(); it != options.end(); ++it, ++i) {
                if (taille == 1)  select += std::to_string(*it);
                else if (i < taille - 1) select += std::to_string(*it) + ", ";
                else select += "et " + std::to_string(*it);
            }
            select += " : ";
        }

        T choix;
        while (true) {
            std::cout << question << select;
            std::cin >> choix;

            if (std::cin.fail() || options.find(choix) == options.end()) {
                gestionErreurBuffer();
                std::cout << "Veuillez entrer un element" << select << ".\n";
            }
            else {
                return choix;
            }
        }
    }

    // Question avec choix parmi un ensemble de valeurs
    template<typename T>
    T demanderChoixDansEnsemble(const std::string& question, const std::set<T>& options) {
        T choix;
        while (true) {
            std::cout << question;
            std::cin >> choix;

            if (std::cin.fail() || options.find(choix) == options.end()) {
                gestionErreurBuffer();
                std::cout << "Choix invalide. Veuillez choisir parmi : ";
                afficherOptions(options);
            }
            else {
                return choix;
            }
        }
    }

    // Question dans Menu
    template<typename T>
    size_t demanderChoixDansMenu(const std::string& question, const std::set<T>& options) {
        int choix;
        size_t n = options.size();
        while (true) {
            std::cout << question;
            afficherMenu(options);
            std::cout << "\nChoix : ";
            std::cin >> choix;

            if (std::cin.fail() || choix < 1 || choix > n) {
                gestionErreurBuffer();
                std::cout << "Choix invalide. Veuillez entrez un nombre entre 1 et " << n << " : ";
            }
            else {
                return choix;
            }
        }
    }

    template<typename T>
    size_t demanderChoixDansMenu(const std::string& question, const std::vector<T>& options) {
        int choix;
        size_t n = options.size();
        while (true) {
            std::cout << question;
            afficherMenu(options);
            std::cout << "\nChoix : ";
            std::cin >> choix;

            if (std::cin.fail() || choix < 1 || choix > n) {
                gestionErreurBuffer();
                std::cout << "Choix invalide. Veuillez entrez un nombre entre 1 et " << n << " : ";
            }
            else {
                return choix;
            }
        }
    }

    // Question oui/non
    inline int demanderConfirmation(const std::string& question) {
        int reponse;
        while (true) {
            std::cout << question << " (1 : oui / 0 : non) : ";
            std::cin >> reponse;

            if (std::cin.fail() || (reponse != 1 && reponse != 0)) {
                gestionErreurBuffer();
                std::cout << "Veuillez repondre par '1' ou '0'.\n";
            }
            else {
                return reponse;
            }
        }
    }

    // Obtenir la valeur � partir de l'index
    template<typename T>
    T obtenirValeurParIndex(const std::set<T>& options, size_t index) {
        auto it = options.begin();
        std::advance(it, index);
        return *it;
    }

    inline void afficherMessage(const std::string& message) {
        std::cout << message << std::endl;
    }

    inline void afficherErreur(const std::string& message) {
        std::string yellow = ConsoleRendering::codeAnsi(Couleur::Jaune), reset = ConsoleRendering::codeAnsi(Couleur::Reset);
        std::cout << yellow << "\n< " << message << " >\n" << reset << std::endl;
    }

    inline void afficherDansBanderole(const std::string& message) {
        std::cout << "\n" << std::string(120, '=') << std::endl;
        std::cout << std::string(50, ' ') << message << std::endl;
        std::cout << std::string(120, '=') << std::endl;
    }

    inline void afficherDansBande(const std::string& message) {
        std::string space = std::string((120 - message.size()) / 2, ' ');
        std::cout << "\n" << std::string(50, '=') << space << message << space << std::string(50, '=') << std::endl;
    }
};
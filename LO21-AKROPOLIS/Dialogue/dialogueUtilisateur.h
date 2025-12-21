#pragma once

#include <iostream>
#include <sstream> // ajouté
#include <string>
#include <set>
#include <algorithm>
#include <limits>
#include "../Exception.h"

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

    // Question dans Menu
    template<typename T>
    T demanderInformation(const std::string& question) {
        T choix;
        while (true) {
            std::cout << question;
            std::cin >> choix;

            if (std::cin.fail()) {
                gestionErreurBuffer();
                std::cout << "Choix invalide. Veuillez rééssayer : ";
            }
            else {
                return choix;
            }
        }
    }

    // Question avec choix numérique dans un intervalle
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
                std::cout << "Veuillez entrer un élément" << select << ".\n";
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
    int demanderChoixDansMenu(const std::string& question, const std::set<T>& options) {
        int choix;
        size_t n = options.size();
        while (true) {
            std::cout << question;
            afficherMenu(options);
            std::cout << "\nChoix : ";
            std::cin >> choix;

            if (std::cin.fail() || choix < 1 || choix > n ) {
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
                std::cout << "Veuillez répondre par '1' ou '0'.\n";
            }
            else {
				return reponse;
            }
        }
    }

    // Obtenir la valeur à partir de l'index
    template<typename T>
    T obtenirValeurParIndex(const std::set<T>& options, size_t index) {
        auto it = options.begin();
        std::advance(it, index);
        return *it;
    }

    // Afficher un message simple
    inline void afficherMessage(const std::string& message) {
        std::cout << message << std::endl;
    }

    // Afficher un message d'erreur
    inline void afficherErreur(const std::string& erreur) {
        std::cerr << "Erreur : " << erreur << std::endl;
    }

    inline std::string demanderPseudoValide(const std::set<std::string>& pseudos_existants, const std::string& message) {
        std::string pseudo;
        while (true) {
            std::cout << message;
            std::cin >> pseudo;

            if (pseudo.empty()) {
                std::cout << "Le pseudo ne peut pas etre vide.\n";
                continue;
            }
            if (!std::all_of(pseudo.begin(), pseudo.end(), [](unsigned char c) { return std::isalpha(c); })) {
                std::cout << "Le pseudo ne doit contenir que des lettres.\n";
                continue;
            }
            if (pseudos_existants.count(pseudo)) {
                std::cout << "Ce pseudo est deja pris.\n";
                continue;
            }
            return pseudo;
        }
    }
}
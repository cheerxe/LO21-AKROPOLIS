#pragma once
#include "../Joueur/joueur.h"
#include "dialogueUtilisateur.h"
#include "../Affichage/affichageGrille.h"

class IDialogueStrategy {
public:
    virtual ~IDialogueStrategy() = default;

    virtual void afficherMessage(const std::string& message) const = 0;
    virtual void afficherErreur(const std::string& message) const = 0;
    virtual void afficherTuile(const Tuile& t) const = 0;
    virtual void afficherGrille(const GrilleHexa& grille) const = 0;
    virtual void afficherGrille(const GrilleHexa_Plate& grille) const = 0;
    virtual void annoncerNouvelleManche() const = 0;
    virtual void afficherArchitecte(const IParticipant& archi) const = 0;
    virtual void annoncerTourIParticipant(const IParticipant& j) const = 0;
    virtual void annoncerFinPartie() const = 0;
    virtual void annoncerAffichageScore() const = 0;

    virtual void afficherEcranAccueil() const = 0;
    virtual bool demanderReprisePartie() const = 0;
    virtual bool demanderModeSolo() const = 0;

    virtual size_t demanderNombreJoueurs(size_t min, size_t max) const = 0;
    virtual std::string demanderPseudo() const = 0;
    virtual std::string demanderPseudo(const std::set<std::string>& pseudos_existants, size_t ind) const = 0;
    virtual size_t demanderArchitecteChef(const std::set<std::string>& pseudos) const = 0;

    virtual size_t demanderTuileDansChantier(const std::vector<Tuile>& chantier, size_t pierres) const = 0;

    virtual void afficherEtatActuelCite(const Cite& cite) const = 0;

    virtual size_t demanderNiveau(const std::set<size_t>& niveaux) const = 0;
    virtual Coord demanderPosition(const std::set<Coord>& positions_valides, size_t niv, const GrilleHexa_Plate& grille_affichage) const = 0;
    virtual int demanderOrientation(const Coord& centre, const std::set<int>& orient_possible, size_t niv, const GrilleHexa_Plate& grille_affichage) const = 0; 
    virtual size_t demanderSens(const Tuile& tuile, int orientation) const = 0;
    virtual int demanderAnnulation() const = 0;
    virtual int demanderModeScore() const = 0;
    virtual void afficherScore(const IParticipant& p, const ScoreBreakdown& score) const = 0;
};

class DialogueConsole : public IDialogueStrategy {
public:
    void afficherMessage(const std::string& message) const override {
        DialogueUtilisateur::afficherMessage(message);
    }

    void afficherErreur(const std::string& message) const override {
        DialogueUtilisateur::afficherErreur(message);
    }

    void afficherTuile(const Tuile& t) const override {
        TuileRendering::afficherTuile(t);
    }

    void afficherGrille(const GrilleHexa& grille) const override {
        GrilleRendering::afficherGrille(grille);
    }

    void afficherGrille(const GrilleHexa_Plate& grille) const override {
        GrilleRendering::afficherGrille(grille);
    }

    void annoncerNouvelleManche() const override {
        DialogueUtilisateur::afficherDansBanderole("Nouvelle Manche");
    }

    void afficherArchitecte(const IParticipant& archi) const override {
        DialogueUtilisateur::afficherMessage("\nArchitecte en chef : " + archi.getPseudo());
    }

    void annoncerTourIParticipant(const IParticipant& j) const override {
        DialogueUtilisateur::afficherDansBande(j.getPseudo());
    }

    void annoncerFinPartie() const override {
        DialogueUtilisateur::afficherDansBanderole("Fin de la partie");
    }

    void annoncerAffichageScore() const override {
        DialogueUtilisateur::afficherDansBanderole("Resultats - Score");
    }

    void afficherEcranAccueil() const override {
        DialogueUtilisateur::afficherMessage(ConsoleRendering::ecranAccueil());
    }

    bool demanderReprisePartie() const override {
        std::set<std::string> options = { "Lancer une nouvelle partie", "Essayer de reprendre une partie" };
        int choix = DialogueUtilisateur::demanderChoixDansMenu("", options);
        return choix == 2;
    }

    bool demanderModeSolo() const override {
        std::set<std::string> options = { "En mode Solo", "En Multijoueur" };
        int choix = DialogueUtilisateur::demanderChoixDansMenu("Souhaitez-vous jouer ?", options);
        return choix == 1;
    }

    size_t demanderNombreJoueurs(size_t min, size_t max) const override {
        std::string question = "\nCombien de joueurs pour cette partie ? (" + std::to_string(min) + "-" + std::to_string(max) + ") : ";
        return DialogueUtilisateur::demanderChoixNumerique(question, min, max);
    }

    std::string demanderPseudo() const override {
        return DialogueUtilisateur::demanderTexte<std::string>("Entrez votre pseudo : ");
    }

    std::string demanderPseudo(const std::set<std::string>& pseudos_existants, size_t ind) const override {
        DialogueUtilisateur::afficherDansBande("IParticipant " + std::to_string(ind));
        std::string pseudo;
        do {
            pseudo = DialogueUtilisateur::demanderTexte<std::string>("Entrez votre pseudo : ");
            if (pseudos_existants.count(pseudo)) {
                DialogueUtilisateur::afficherMessage("Ce pseudo est déjà pris !");
            }
        } while (pseudos_existants.count(pseudo));
        return pseudo;
    }

    size_t demanderArchitecteChef(const std::set<std::string>& pseudos) const override  {
        std::set<std::string> pseudo_set;
        for (const auto& it : pseudos) pseudo_set.insert(it);
        std::string question = "\nQuel est le dernier d\'entre vous a avoir gravi une colline ? il sera architecte en chef : ";
        return DialogueUtilisateur::demanderChoixDansMenu(question, pseudo_set) - 1;
    }

    // on peut pas faire de set de tuile (parce que on a pas "<" sur Tuile)
    size_t demanderTuileDansChantier(const std::vector<Tuile>& chantier, size_t pierres) const override {
        DialogueUtilisateur::afficherMessage("\nQuelle tuile voulez-vous ajouter a votre cite ?");
        TuileRendering::afficherChantier(chantier);
        size_t n = chantier.size();
        std::string question = "\nChoisissez une option entre 1 et " + std::to_string(n) + " : ";
        size_t position_choisie = DialogueUtilisateur::demanderChoixNumerique<size_t>(question, 1, n);
        while (position_choisie - 1 > pierres) {
            DialogueUtilisateur::afficherMessage("Vous n'avez pas assez d'argent pour l'acheter. Choisissez une autre tuile.\n");
            position_choisie = DialogueUtilisateur::demanderChoixNumerique<size_t>(question, 1, n);
        }

        return position_choisie - 1;
    }

    void afficherEtatActuelCite(const Cite& cite) const override {
        DialogueUtilisateur::afficherMessage("\n" + std::string(30, '*') + " Etat actuel de la cite : " + std::string(30, '*') + "\n");
        GrilleRendering::afficherGrille(cite.getGrille());
    }

    size_t demanderNiveau(const std::set<size_t>& niveaux) const override {
        if (niveaux.size() == 1) {
            DialogueUtilisateur::afficherMessage("\nVous pouvez jouer sur le niveau 1.");
            return 1;
        }
        std::string question = "\nChoisissez un niveau pour placer la tuile";
        return DialogueUtilisateur::demanderChoixParmi(question, niveaux);
    }

    Coord demanderPosition(const std::set<Coord>& positions_valides, size_t niv, const GrilleHexa_Plate& grille_affichage) const override {
        GrilleRendering::afficherPositionsPossibles(grille_affichage, positions_valides, niv);

        std::string question = "\nChoisissez une option entre 1 et " + std::to_string(positions_valides.size()) + " : ";
        size_t position_choisie = DialogueUtilisateur::demanderChoixNumerique<size_t>(question, 1, positions_valides.size());

        return DialogueUtilisateur::obtenirValeurParIndex(positions_valides, position_choisie - 1);
    }

    int demanderOrientation(const Coord& centre, const std::set<int>& orient_possible, size_t niv, const GrilleHexa_Plate& grille_affichage) const override {
        GrilleRendering::afficherOrientationsPossibles(grille_affichage, centre, orient_possible, niv);

        if (orient_possible.size() != 1) {
            std::string question = "\nChoisissez une option entre 1 et " + std::to_string(orient_possible.size()) + " : ";
            size_t orientation_choisie = DialogueUtilisateur::demanderChoixNumerique<size_t>(question, 1, orient_possible.size());

            return DialogueUtilisateur::obtenirValeurParIndex(orient_possible, orientation_choisie - 1);
        }

        return 1;
    }

    size_t demanderSens(const Tuile& tuile, int orientation) const override {
        TuileRendering::afficherTousSens(tuile, orientation);

        std::string question = "\nChoisissez une option entre 1 et 3 : ";
        size_t option_choisie = DialogueUtilisateur::demanderChoixNumerique<int>(question, 1, 3);

        return option_choisie - 1;
    }

    int demanderAnnulation() const override {
        return DialogueUtilisateur::demanderConfirmation("Souhaitez-vous changer d'avis et recommencer votre manche ?");
    }

    void afficherEtatActuelCite(const Cite& c) const override {
        DialogueUtilisateur::afficherMessage("\n" + std::string(30, '*') + " Etat actuel de la cite : " + std::string(30, '*') + "\n");
        GrilleRendering::afficherGrille(c.getGrille());
    }

    int demanderModeScore() const override {
        DialogueUtilisateur::afficherDansBanderole("Selection du mode de Score");
        std::vector<std::string> options { "Classique", "Variante", "Personnel" };
        return DialogueUtilisateur::demanderChoixDansMenu("", options);
    }

    void afficherScore(const IParticipant& p, const ScoreBreakdown& score) const override {
        DialogueUtilisateur::afficherDansBanderole(p.getPseudo());
        DialogueUtilisateur::afficherMessage("- Habitations : " + score.hab);
        DialogueUtilisateur::afficherMessage("- Marches : " + score.mar);
        DialogueUtilisateur::afficherMessage("- Casernes : " + score.cas);
        DialogueUtilisateur::afficherMessage("- Temples : " + score.tem);
        DialogueUtilisateur::afficherMessage("- Jardins : " + score.jar);
        DialogueUtilisateur::afficherMessage("- Pierres : " + score.stones);
        DialogueUtilisateur::afficherMessage("- TOTAL : " + score.total);
        DialogueUtilisateur::afficherMessage("\n");
    }
};
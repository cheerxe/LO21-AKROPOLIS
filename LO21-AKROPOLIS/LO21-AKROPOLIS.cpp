// LO21-AKROPOLIS.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "Cite/cite.h"
#include "Jeu/jeu.h"
#include "Affichage/affichageGrille.h"
#include "Pioche/pioche.h"
#include "Dialogue/dialogueStrategy.h"
#include "Partie/partie.h"

int main()
{

    GrilleHexa_Niv1 niv;
    GrilleHexa_Plate plate;
    plate.placerHexagone(Coord{ 0,0 }, HexagoneAffichage(1));
    GrilleRendering::afficherGrille(niv);
    GrilleRendering::afficherGrille(plate);
    auto type = (TypeQuartier::Habitation);
    auto type2 = (TypeQuartier::Marche);
    auto type3 = (TypeQuartier::Temple);
    auto q1 = Quartier(type);
    auto q2 = Quartier(type2);
    auto q3 = Quartier(type2);
    auto c = Carriere();
    Tuile t1(q1, c, c);
    Tuile t2(q2, q3, c);

    Jeu::getInstance(3);
    const std::vector<Tuile>& t = { t1,t2 };

    DialogueUtilisateur::demanderChoixDansMenu("SQSq", t);

    Pioche p;


    std::cout << "Hello World!\n";
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.

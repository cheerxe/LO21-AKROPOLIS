#pragma once
#include "../Partie/partie.h"
#include <cstdlib>
#include <fstream>

#ifdef _WIN32//le prepocesseur regarde si la macro WIN32 caracterisant l OS windows existe ou non(teste si on est sous windows)
    const char* appdata = std::getenv("APPDATA");
    if (appdata and strlen(appdata))
    chemin = std::getenv("APPDATA");//chemin ou on stocke classiquement les data des applications sous windows obtenu grace a la var d envirronement APPDATA
    else
    chemin = fichier::current_path();//chemin vers l executable
#else//pour les autres macros detectess = systeme mac ou linux
//leur var d envirronement pour ce chemin est nomme HOME
    const char* home = std::getenv("HOME");
    if (home and strlen(home))
    chemin = std::getenv("HOME");
    else
    chemin = fichier::current_path();
#endif

    namespace Sauvegarde {

        namespace fichier = std::filesystem;

        fichier::path recupererChemin() {//fonction qui retourne le chemin ou seront stockees les donnes de la partie
            fichier::path chemin;//objet qui contient le chemin ou stocker le fichier
            chemin /= "Akropolys"; //l operateur /= est surcharge dans la bibliotheque filesystem et permet d'etendre le chemin 
            fichier::create_directories(chemin);//creerts les dossiers intermediaire pour la premiere fois
            chemin /= "akropolis.bin";
            return chemin;//on retourne le chemin obtenu
        }

        //fonctions de serialisation = transformation d un objet complexe en une sequence de bit plate

        void serialize(const Coord& coord, std::ofstream& out) {
            out.write(reinterpret_cast<const char*>(&coord.q), sizeof(coord.q)); //écrit des octets et prends en entree l adresse du premier caractere et cb de caracteres ecrire
            out.write(reinterpret_cast<const char*>(&coord.r), sizeof(coord.r));
        }

        //enum class HexType : uint8_t { Carriere, Quartier, Place1, Place2, Place3, None };//enum des types hexagones accessibles partout

        HexType detecterTypeHexa(const Hexagone& hexa) {//fonction de detection du type de l hexagone

            const std::string texte = hexa.getContenu();

            if (texte == "[C]") {//renvoie du type selon le contenu affiche a partir de matchTexte
                return HexType::Carriere;
            }
            else if (texte == "[Q]") {
                return HexType::Quartier;
            }
            else if (texte == " * ") {
                return HexType::Place1;
            }
            else if (texte == "** ") {
                return HexType::Place2;
            }
            else if (texte == "***") {
                return HexType::Place3;
            }
            else {
                return HexType::None;
            }
        }

        TypeQuartier detecterTypeQuartier(const Hexagone& quartier) {//deduit le type d'un hexagone de type qurtier a partir de sa couleur
            //on passe un hexagone en paramtre mais ca n'impacte pas le processus s de deduction
            //on se servira du type quartier renvoye pour reconstruire un objet de type quartier lors de la deserialisation

            if (quartier.getCouleur() == Couleur::Rouge) {
                return TypeQuartier::Caserne;
            }
            else if (quartier.getCouleur() == Couleur::Bleu) {
                return TypeQuartier::Habitation;
            }
            else if (quartier.getCouleur() == Couleur::Vert) {
                return TypeQuartier::Jardin;
            }
            else if (quartier.getCouleur() == Couleur::Violet) {
                return TypeQuartier::Temple;
            }
            else if (quartier.getCouleur() == Couleur::Jaune) {
                return TypeQuartier::Marche;
            }
            else {
                return TypeQuartier::Habitation;
            }
        }

        void serialize(const Hexagone& hexagone, std::ofstream& out) {

            HexType type = detecterTypeHexa(hexagone);//recuperation du type de l hexagone
            out.write(reinterpret_cast<const char*>(&type), sizeof(type));//ecriture du type d hexagone que c est

            if (type == HexType::Quartier or type == HexType::Place1 or type == HexType::Place2 or type == HexType::Place3) {//on stocke aussi le type de quartier
                TypeQuartier type_quartier = detecterTypeQuartier(hexagone);
                out.write(reinterpret_cast<const char*>(&type_quartier), sizeof(type_quartier));
            }
            //ls hexagones de couleur Cyan ou sans type seront recree en HexagoneAffichage

            Couleur couleur = hexagone.getCouleur();
            out.write(reinterpret_cast<const char*>(&couleur), sizeof(couleur));//ecriture de la couleur de l hexagone

            size_t taille_contenu = hexagone.getContenu().size();
            out.write(reinterpret_cast<const char*>(&taille_contenu), sizeof(taille_contenu));//ecriture de la largeur du contenu
            out.write(hexagone.getContenu().c_str(), taille_contenu);//ecritue du champs contenu
        }

        void serialize(const GrilleHexa& grille_hexa, std::ofstream& out) {//ecrire une GrilleHexa et ses membres:

            //std::map<Coord, Hexagone> grille;Map-key = Coord | protected car héritée
            const std::map<Coord, Hexagone>& grille = grille_hexa.getGrille(); //recuperation de la grille
            size_t taille_grille = grille.size();//donne le nb de couple (corrd, hexagone) dans la grille

            out.write(reinterpret_cast<const char*>(&taille_grille), sizeof(taille_grille));//ecriture du nb de couple (coord, hexagone)

            for (const auto& [c, h] : grille) {//utilisation du structured binding de c++17 pour parcourir un couple cle-valeur
                serialize(c, out);//ecriture de chaque couple clé-valeur de la grille
                serialize(h, out);
            }
        }

        void serialize(const GrilleHexa_Niveau& grille_hexa_niveau, std::ofstream& out) {

            //std::vector<std::set<Coord>> groupes_tuiles;
            const std::vector<std::set<Coord>> groupes_tuiles = grille_hexa_niveau.getGroupesTuiles();//recuperation de vector de groupes de tuiles via getter ajoute dans grilleHexa
            size_t taille_groupes_tuiles = groupes_tuiles.size();//recuperation de la taille de ce vector
            out.write(reinterpret_cast<const char*>(&taille_groupes_tuiles), sizeof(taille_groupes_tuiles));//ecriture de sa taille
            for (const auto& g : groupes_tuiles) {
                const size_t taille_set = g.size();//recuperation et ecriture de la taille de chaque set de coordonnees 
                out.write(reinterpret_cast<const char*>(&taille_set), sizeof(taille_set));

                for (const Coord& c : g) {
                    serialize(c, out);//serialization de chacune des coordonnees du groupe courant
                }
            }
        }

        void serialize(const GrilleHexa_NivPlus& grille_hexa_niv_plus, std::ofstream& out) {//serialize la grille hexa niv p+

            const GrilleHexa* grille_precedente = grille_hexa_niv_plus.getGrillePrecedente();

            bool grille_existe = (grille_precedente != nullptr);//test si le ptr de grille_hexa_niv_plus a ecrire existe bien
            out.write(reinterpret_cast<const char*>(&grille_existe), sizeof(grille_existe)); //on ecrit le boolen dans le fichier aussi pour la deserialization 

            if (grille_existe) {//si le ptr n est pas null alors on serialize
                serialize(*grille_precedente, out);//on serialiser  l'objet pointe par l'attribut grille_precedente
            }
            else {
                std::cerr << "erreur de sauvegarde ";
            }
        }

        void serialize(const GrilleHexa_Plate& grille_hexa_plate, std::ofstream& out) {

            std::string type_grille = "GrilleHexa_Plate";
            size_t taille_type = type_grille.size();
            out.write(reinterpret_cast<const char*>(&taille_type), sizeof(taille_type));//ecriture de la taille du type cree

            out.write(type_grille.c_str(), taille_type);//c_str donne un pointeur vers un char

            serialize(static_cast<const GrilleHexa&>(grille_hexa_plate), out);//type statique devient GrilleHexa et appelle la bonne fonction( !=  type dynamique qui reste GrilleHexaplate)
        }

        void serialize(const GrilleHexa_Niv1& grille_hexa_niv1, std::ofstream& out) {
            std::string type_grille = "GrilleHexa_Niv1";
            size_t taille_type = type_grille.size();
            out.write(reinterpret_cast<const char*>(&taille_type), sizeof(taille_type));//ecriture de la taille du type cree

            out.write(type_grille.c_str(), taille_type);//c_str donne un pointeur vers un char

            serialize(static_cast<const GrilleHexa&>(grille_hexa_niv1), out);//type statique devient GrilleHexa et appelle la bonne fonction( !=  type dynamique qui reste GrilleHexaplate)
        }

        void serialize(const Cite& cite, std::ofstream& out) {//serialiser la cite et ses membres a l instant t

            serialize(cite.getGrilleHexaNiv1(), out);//GrilleHexa_Niv1 niv1

            serialize(cite.getGrilleHexaPlate(), out);//GrilleHexa_Plate grille_plate

            serialize(cite.getGrille(), out);//GrilleHexa_Niv1 grille_finale

            const std::vector<GrilleHexa_NivPlus*> niveaux = cite.getNiveaux(); //tout le contenu du vector std::vector<GrilleHexa_NivPlus*> niveaux

            size_t nb_niveaux = niveaux.size();//stocker le nb de niveaux 
            out.write(reinterpret_cast<const char*>(&nb_niveaux), sizeof(nb_niveaux));

            for (const auto& g : niveaux) {//attention si par un quelconque moyen un ptr de grille est nullptr
                if (g != nullptr) {
                    serialize(*g, out);//on serialize l objet pointe
                }
                else {
                    std::cerr << "erreur de sauvegarde";
                }
            }
        }

        void serialize(const Tuile& tuile, std::ofstream& out) {//sauvegarder une tuile
            Hexagone hex1 = tuile.getHex1();
            Hexagone hex2 = tuile.getHex2();
            Hexagone hex3 = tuile.getHex3();

            serialize(hex1, out);
            serialize(hex2, out);
            serialize(hex3, out);
        }

        void serialize(const Table& chantier, std::ofstream& out) {

            size_t capacite = chantier.getCapacite();
            out.write(reinterpret_cast<const char*>(&capacite), sizeof(capacite));

            size_t nb_visibles = chantier.taille();
            out.write(reinterpret_cast<const char*>(&nb_visibles), sizeof(nb_visibles));

            for (size_t i = 0; i < nb_visibles; i++) {
                const Tuile* tuile = chantier.regarder(i);
                if (tuile) {
                    serialize(*tuile, out);
                }
            }

            size_t nb_reservoir = chantier.tailleR();
            for (size_t i = 0; i < nb_reservoir; i++) {
                const Tuile* tuile = chantier.regarder(i);
                if (tuile) {
                    serialize(*tuile, out);
                }
            }
        }

        void serialize(const Tas& tas, std::ofstream& out) {
            size_t nb_tuiles = tas.getNbTuiles();
            out.write(reinterpret_cast<const char*>(&nb_tuiles), sizeof(nb_tuiles));

            for (size_t i = 0; i < nb_tuiles; i++) {
                const Tuile* tuile = tas.getTuilesTas(i);
                serialize(*tuile, out);
            }
        }

        void serialize(const Pioche& pioche, std::ofstream& out) {
            std::vector<const Tas*> tas = pioche.getTasPioche();
            size_t taille_tas = tas.size();
            out.write(reinterpret_cast<const char*>(&taille_tas), sizeof(taille_tas));

            for (size_t i = 0; i < taille_tas; i++) {
                serialize(*tas[i], out);
            }

            std::vector<size_t> curseur = pioche.getCurseur();
            size_t taille_curseur = curseur.size();
            out.write(reinterpret_cast<const char*>(&taille_curseur), sizeof(taille_curseur));

            for (size_t i = 0; i < taille_curseur; i++) {
                out.write(reinterpret_cast<const char*>(&curseur[i]), sizeof(curseur[i]));
            }
        }

        void serialize(Jeu& jeu, std::ofstream& out) {

            size_t nb_tuiles = jeu.getNbTuiles();
            out.write(reinterpret_cast<const char*>(&nb_tuiles), sizeof(nb_tuiles));

            for (size_t i = 0; i < nb_tuiles; i++) {
                const Tuile& tuile = jeu.getTuile(i);
            }

        }

        void pauserPartie(Partie& partie, fichier::path chemin) {//met en pause la partie et enregistre les donnes dans le fichiers
            if (chemin.empty())
                chemin = recupererChemin();//recupere le chemin du fichier ou stocker les donnees de la partie
            std::ofstream out(chemin, std::ios::binary);//ouverture du fichier en ecriture en binaire car on va stocker des objets

            if (!out) {//ereur ouverture du fichier
                std::cerr << "erreur lors de l ouverture du fichier pauserPartie ";
                return;
            }
            bool en_cours = partie.getEnCours();
            out.write(reinterpret_cast<const char*>(&en_cours), sizeof(en_cours));//stockage du booleen qui montre que la partie est en cours
            size_t nbj = partie.getNbJoueurs();
            out.write(reinterpret_cast<const char*>(&nbj), sizeof(nbj));//stockage du nb de joueurs pour deserialiser correctement

            for (size_t i = 0; i < nbj; i++) {//stockage de tous les objets joureurs du vector de joueurs de la partie

                Joueur* joueur = partie.getJoueur(i);

                //write écrit des octets et prends en entree l adresse du premier caractere et cb de caracteres ecrire
                size_t larg_pseudo = joueur->getPseudo().size();//recuperation de la taille du pseudo du joueur courant
                out.write(reinterpret_cast<const char*>(&larg_pseudo), sizeof(larg_pseudo));//ecrture de la taille du pseudo pour se retrouver et relire plus tard

                std::string pseudo = joueur->getPseudo();//recuperation du pseudo 
                out.write(pseudo.c_str(), pseudo.size());//ecriture du pseudo c_str() qui retourne un ptr de char

                size_t pierres = joueur->getPierres();//recuperation du nb de pierres du joueur
                out.write(reinterpret_cast<const char*>(&pierres), sizeof(pierres));//ecriture des pierres du joueur

                const Cite& cite_joueur = joueur->getCite();//recuperation de la cite du joueur
                serialize(cite_joueur, out); //ecriture de la cite du joueur appel a fonction surchargee
            }

            out.close();
            std::cout << "\nSauvegarde effectuee";
        }
        //fonctions de deserialisation
        void deserialize(Coord& coord, std::ifstream& fic) {

            fic.read(reinterpret_cast<char*>(&coord.q), sizeof(coord.q));
            fic.read(reinterpret_cast<char*>(&coord.r), sizeof(coord.r));
        }

        Hexagone* deserializeHexagone(std::ifstream& fic) {//on va deserialiser dans l ordre ou on a serialiser
            Hexagone* h = nullptr;
            HexType type;//va contenir le type de l hexagone a reconstruire
            fic.read(reinterpret_cast<char*>(&type), sizeof(type));//lire le type et le stocker dans la variable
            if (!fic) return nullptr;//verif sur la lecture a chaque fois

            TypeQuartier type_quartier; //va contenir le type du quartier

            if (type == HexType::Quartier or type == HexType::Place1 or type == HexType::Place2 or type == HexType::Place3) {
                fic.read(reinterpret_cast<char*>(&type_quartier), sizeof(type_quartier));
                if (!fic) return nullptr;//verif sur la lecture a chaque fois    
            }
            Couleur c;//pour les couleurs
            fic.read(reinterpret_cast<char*>(&c), sizeof(c));
            if (!fic) return nullptr;

            if (type == HexType::Quartier) {
                h = new Quartier(type_quartier);//creation de l'hexagone     
            }
            else if (type == HexType::Place1) {
                h = new Place(type_quartier, NbEtoiles::une);
            }
            else if (type == HexType::Place2) {
                h = new Place(type_quartier, NbEtoiles::deux);
            }
            else if (type == HexType::Place3) {
                h = new Place(type_quartier, NbEtoiles::trois);
            }
            else if (type == HexType::Carriere) {
                h = new Carriere;
            }
            size_t taille_contenu;
            fic.read(reinterpret_cast<char*>(&taille_contenu), sizeof(taille_contenu));//on va lire le contenu selon le nb de bits a lire
            if (!fic) return nullptr;//verif sur la lecture a chaque fois

            std::string contenu;
            fic.read(reinterpret_cast<char*>(&contenu[0]), taille_contenu);
            if (!fic) return nullptr;//verif sur la lecture a chaque fois

            if (!contenu.empty() && contenu[0] == ' ' && contenu[1] == '#') {
                std::string niv_contenu = contenu.substr(2);
                if (c == Couleur::Cyan) {
                    size_t niv = static_cast<size_t>(std::stoul(niv_contenu));
                    h = new HexagoneAffichage(niv);
                }
            }
            else {
                std::cerr << "numero affichage hexagone introuvable";
                return nullptr;
            }
            return h;
        }
        /*
        GrilleHexa deserialize(std::ifstream& fic) {
            GrilleHexa grille;
            size_t taille_grille;
            fic.read(reinterpret_cast<char*>(&taille_grille), sizeof(taille_grille));
            if (!fic) return nullptr;//verif sur la lecture a chaque fois

            const std::map<Coord, Hexagone>& grille;//comment reconstruire le contenu de la grille ?

        }

        void serialize(const GrilleHexa_Niveau& grille_hexa_niveau, std::ofstream& out) {

            //std::vector<std::set<Coord>> groupes_tuiles;
            const std::vector<std::set<Coord>> groupes_tuiles = grille_hexa_niveau.getGroupesTuiles();//recuperation de vector de groupes de tuiles via getter ajoute dans grilleHexa
            size_t taille_groupes_tuiles = groupes_tuiles.size();//recuperation de la taille de ce vector
            out.write(reinterpret_cast<const char*>(&taille_groupes_tuiles), sizeof(taille_groupes_tuiles));//ecriture de sa taille
            for (const auto& g : groupes_tuiles) {
                const size_t taille_set = g.size();//recuperation et ecriture de la taille de chaque set de coordonnees
                out.write(reinterpret_cast<const char*>(&taille_set), sizeof(taille_set));

                for (const Coord& c : g) {
                    serialize(c, out);//serialization de chacune des coordonnees du groupe courant
                }
            }
        }*/

        //grilleHexa à ne pas deserializer car il n'existe aucun objet de ce type(virtuelle pure)
        void deserializeGroupeTuiles(std::ifstream& fic, std::vector<std::set<Coord>>& groupes) {
            if (!fic) return;
            size_t nb_groupes;
            fic.read(reinterpret_cast<char*>(&nb_groupes), sizeof(nb_groupes));
            if (!fic) return;

            groupes.clear();
            groupes.reserve(nb_groupes);

            // std::vector<std::set<Coord>> groupes;
            for (size_t i = 0; i < nb_groupes; i++) {
                size_t taille_set;
                fic.read(reinterpret_cast<char*>(&taille_set), sizeof(taille_set));

                std::set<Coord> groupe;
                for (size_t j = 0; j < taille_set; j++) {
                    Coord c;
                    deserialize(c, fic);
                    groupe.insert(c);
                }
                groupes.push_back(groupe);
            }
        }

        class GrilleHexa_Concrete : public GrilleHexa {
        public:
            ~GrilleHexa_Concrete() override = default;
        };

        GrilleHexa* deserialiserGrilleHexa(std::ifstream& fic) {
            if (!fic) return nullptr;
            auto* g = new GrilleHexa_Concrete();

            size_t taille;
            fic.read(reinterpret_cast<char*>(&taille), sizeof(taille));
            if (!fic) return nullptr;

            for (size_t i = 0; i < taille; i++) {
                Coord c;
                deserialize(c, fic);

                Hexagone* h = deserializeHexagone(fic);
                if (!fic) return nullptr;

                g->getGrille().insert({ c, *h });
            }

            return g;
        }

        GrilleHexa_Niv1* deserializeGrilleHexa_Niv1(std::ifstream& fic) {

            auto* g = new GrilleHexa_Niv1();
            GrilleHexa* base = deserialiserGrilleHexa(fic);

            g->getGrille() = base->getGrille();

            deserializeGroupeTuiles(fic, g->getGroupesTuiles());

            return g;

        }

        void deserialize(const GrilleHexa_Plate& grille_hexa_plate, std::ifstream& fic) {

        }

        void deserialize(const GrilleHexa_NivPlus& grille_hexa_niv_plus, std::ifstream& fic) {

        }
        void deserialize(const GrilleHexa& grille_hexa, std::ifstream& fic) {

        }

        void deserialize(Tuile& tuile, std::ifstream& fic) {
            /*
            HexType type;
            fic.read(reinterpret_cast<char*>(&type), sizeof(type));
            Hexagone hex1, hex2, hex3;
            deserialize(hex1, fic);
            deserialize(hex2, fic);
            deserialize(hex3, fic);

            tuile = Tuile(hex1, hex2, hex3);*/
        }

        Cite* deserializeCite(std::ifstream& fic) {
            Cite* cite = nullptr;

            return cite;
        }



        void reprendrePartie(fichier::path chemin) {//reprendre une partie qui avait été mise en pause avec deserialisation

            if (chemin.empty()) {
                std::cerr << "chemin ouverture fichier vide";
                return;
            }
            std::ifstream fic(chemin, std::ios::binary);

            if (!fic) {//ereur ouverture du fichier
                std::cerr << "erreur lors de l ouverture du fichier ";
                return;
            }

            std::vector<char> buffer;//vector pour stocker les donnees lues
            fic.seekg(0, std::ios::end);//positionner le curseur a la fin du fichier
            size_t taille_fichier = fic.tellg();//recuperer la taille en bits du fichiers
            fic.seekg(0, std::ios::beg);//reposition deu curseur au tout debut du fichier

            buffer.resize(taille_fichier);
            fic.read(buffer.data(), taille_fichier);//on mets toute le contenu d fichier dans le vector buffer

            //recreation des joueur, de leur cite, des elements de leurs cite via boucle for et deserialize
            // recreer les tuiles
            // recreer le chantier
            //ensuite jouerManche

            fic.close();
        }

        bool partieEnCours(fichier::path chemin) {//retourne true si une partie est en cours et false sinon
            std::ifstream fic(chemin, std::ios::binary);
            if (fic) {
                bool en_cours;
                fic.read(reinterpret_cast<char*>(&en_cours), sizeof(en_cours));
                return en_cours;
            }
            std::cerr << "erreur lors de l'ouverture du fichier ";
            return false;
        }
    };
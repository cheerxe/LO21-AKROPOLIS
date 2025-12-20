#include "jeu.h"

Jeu& Jeu::getInstance(size_t nb_joueur) {
	if (instance == nullptr) { instance = new Jeu(nb_joueur); } //seul moyen d'appeler le constructeur
	Jeu& ref_instance = *instance;
	return ref_instance;
}

Jeu::Jeu(size_t nb_joueur) : nb_tuiles(11 * (nb_joueur + 1)) {
	tuiles.reserve(nb_tuiles);
	instancierTuiles(nb_joueur);
}

std::vector<const Tuile*> Jeu::getReservoir() const {
	std::vector<const Tuile*> ref_tuiles;
	ref_tuiles.reserve(tuiles.size());

	for (const auto& t : tuiles)
		ref_tuiles.push_back(&t);

	return ref_tuiles;
}

Jeu* Jeu::instance = nullptr;

//A REECRIRE AVEC LES BONS CONSTRUCTEUR (cf. tuile.h)

void Jeu::instancierTuiles(size_t nbjoueurs) {//creer les tuiles du jeu en fonction du nb de joueurs

	// C'est beaucoup plus lisible et copier-coller friendly :

	//tuiles.emplace_back( //emplace_back appelle le constructeur de Tuile
	Quartier q1(TypeQuartier::Jardin);
	Quartier q2(TypeQuartier::Habitation);
	Place q3(TypeQuartier::Marche, NbEtoiles::deux);
	tuiles.emplace_back(q1, q2, q3);


	Quartier q4(TypeQuartier::Temple);
	Quartier q5(TypeQuartier::Habitation);
	Place q6(TypeQuartier::Caserne, NbEtoiles::deux);
	tuiles.emplace_back(q4, q5, q6);
	//	
	//enum class TypeQuartier { Habitation, Marche, Caserne, Temple, Jardin };

	Quartier q7(TypeQuartier::Marche);
	Carriere q8;
	Quartier q9(TypeQuartier::Caserne);
	tuiles.emplace_back(q7, q8, q9);

	Quartier q10(TypeQuartier::Caserne);
	Quartier q11(TypeQuartier::Habitation);
	Place q12(TypeQuartier::Temple, NbEtoiles::deux);
	tuiles.emplace_back(q10, q11, q12);


	Carriere q13;
	Carriere q14;
	Place q15(TypeQuartier::Jardin, NbEtoiles::trois);
	tuiles.emplace_back(q13, q14, q15);


	Quartier q16(TypeQuartier::Caserne);
	Carriere q17;
	Quartier q18(TypeQuartier::Temple);
	tuiles.emplace_back(q16, q17, q18);

	Carriere q19;
	Carriere q20;
	Place q21(TypeQuartier::Temple, NbEtoiles::deux);
	tuiles.emplace_back(q19, q20, q21);


	Quartier q22(TypeQuartier::Jardin);
	Carriere q23;
	Place q24(TypeQuartier::Habitation, NbEtoiles::une);
	tuiles.emplace_back(q22, q23, q24);
	//																enum class TypeQuartier { Habitation, Marche, Caserne, Temple, Jardin };

	Quartier q25(TypeQuartier::Temple);
	Carriere q26;
	Quartier q27(TypeQuartier::Marche);
	tuiles.emplace_back(q25, q26, q27);

	Quartier q28(TypeQuartier::Habitation);
	Quartier q29(TypeQuartier::Caserne);
	Quartier q30(TypeQuartier::Jardin);
	tuiles.emplace_back(q28, q29, q30);

	Quartier q31(TypeQuartier::Marche);
	Carriere q32;
	Carriere q33;
	tuiles.emplace_back(q31, q32, q33);

	Quartier q34(TypeQuartier::Jardin);
	Carriere q35;
	Quartier q36(TypeQuartier::Marche);
	tuiles.emplace_back(q34, q35, q36);

	Quartier q37(TypeQuartier::Caserne);
	Quartier q38(TypeQuartier::Marche);
	Carriere q39;
	tuiles.emplace_back(q37, q38, q39);

	Carriere q40;
	Place q41(TypeQuartier::Jardin, NbEtoiles::trois);
	Quartier q42(TypeQuartier::Habitation);
	tuiles.emplace_back(q40, q41, q42);

	Quartier q43(TypeQuartier::Habitation);
	Carriere q44;
	Place q45(TypeQuartier::Temple, NbEtoiles::deux);
	tuiles.emplace_back(q43, q44, q45);

	Quartier q46(TypeQuartier::Marche);
	Quartier q47(TypeQuartier::Habitation);
	Place q48(TypeQuartier::Caserne, NbEtoiles::deux);
	tuiles.emplace_back(q46, q47, q48);

	Quartier q49(TypeQuartier::Marche);
	Quartier q50(TypeQuartier::Habitation);
	Quartier q51(TypeQuartier::Temple);
	tuiles.emplace_back(q49, q50, q51);

	Place q52(TypeQuartier::Habitation, NbEtoiles::une);
	Carriere q53;
	Carriere q54;
	tuiles.emplace_back(q52, q53, q54);

	Carriere q55;
	Carriere q56;
	Quartier q57(TypeQuartier::Temple);
	tuiles.emplace_back(q55, q56, q57);

	Place q58(TypeQuartier::Habitation, NbEtoiles::une);
	Quartier q59(TypeQuartier::Marche);
	Quartier q60(TypeQuartier::Caserne);
	tuiles.emplace_back(q58, q59, q60);

	Carriere q61;
	Place q62(TypeQuartier::Caserne, NbEtoiles::deux);
	Quartier q63(TypeQuartier::Habitation);
	tuiles.emplace_back(q61, q62, q63);

	Quartier q64(TypeQuartier::Habitation);
	Carriere q65;
	Quartier q66(TypeQuartier::Habitation);
	tuiles.emplace_back(q64, q65, q66);

	Carriere q67;
	Place q68(TypeQuartier::Marche, NbEtoiles::deux);
	Carriere q69;
	tuiles.emplace_back(q67, q68, q69);

	Place q70(TypeQuartier::Marche, NbEtoiles::deux);
	Quartier q71(TypeQuartier::Temple);
	Quartier q72(TypeQuartier::Habitation);
	tuiles.emplace_back(q70, q71, q72);

	Quartier q73(TypeQuartier::Temple);
	Quartier q74(TypeQuartier::Marche);
	Carriere q75;
	tuiles.emplace_back(q73, q74, q75);

	Carriere q76;
	Place q77(TypeQuartier::Caserne, NbEtoiles::deux);
	Carriere q78;
	tuiles.emplace_back(q76, q77, q78);

	Place q79(TypeQuartier::Habitation, NbEtoiles::une);
	Carriere q80;
	Carriere q81;
	tuiles.emplace_back(q79, q80, q81);

	Carriere q82;
	Place q83(TypeQuartier::Habitation, NbEtoiles::une);
	Carriere q84;
	tuiles.emplace_back(q82, q83, q84);

	Quartier q85(TypeQuartier::Caserne);
	Quartier q86(TypeQuartier::Temple);
	Quartier q87(TypeQuartier::Habitation);
	tuiles.emplace_back(q85, q86, q87);

	Quartier q88(TypeQuartier::Caserne);
	Place q89(TypeQuartier::Marche, NbEtoiles::deux);
	Carriere q90;
	tuiles.emplace_back(q88, q89, q90);

	Quartier q91(TypeQuartier::Habitation);
	Place q92(TypeQuartier::Jardin, NbEtoiles::trois);
	Carriere q93;
	tuiles.emplace_back(q91, q92, q93);

	Quartier q94(TypeQuartier::Habitation);
	Carriere q95;
	Place q96(TypeQuartier::Temple, NbEtoiles::deux);
	tuiles.emplace_back(q94, q95, q96);

	Quartier q97(TypeQuartier::Jardin);
	Carriere q98;
	Carriere q99;
	tuiles.emplace_back(q97, q98, q99);

	Quartier q100(TypeQuartier::Caserne);
	Carriere q101;
	Quartier q102(TypeQuartier::Marche);
	tuiles.emplace_back(q100, q101, q102);

	Carriere q103;
	Quartier q104(TypeQuartier::Habitation);
	Quartier q105(TypeQuartier::Caserne);
	tuiles.emplace_back(q103, q104, q105);

	Quartier q106(TypeQuartier::Marche);
	Quartier q107(TypeQuartier::Habitation);
	Carriere q108;
	tuiles.emplace_back(q106, q107, q108);

	Quartier q109(TypeQuartier::Marche);
	Quartier q110(TypeQuartier::Habitation);
	Quartier q111(TypeQuartier::Jardin);
	tuiles.emplace_back(q109, q110, q111);

	Quartier q112(TypeQuartier::Caserne);
	Quartier q113(TypeQuartier::Jardin);
	Carriere q114;
	tuiles.emplace_back(q112, q113, q114);

	Quartier q115(TypeQuartier::Temple);
	Quartier q116(TypeQuartier::Habitation);
	Carriere q117;
	tuiles.emplace_back(q115, q116, q117);

	Carriere q118;
	Carriere q119;
	Place q120(TypeQuartier::Marche, NbEtoiles::deux);
	tuiles.emplace_back(q118, q119, q120);

	Carriere q121;
	Carriere q122;
	Place q123(TypeQuartier::Temple, NbEtoiles::deux);
	tuiles.emplace_back(q121, q122, q123);

	Quartier q124(TypeQuartier::Habitation);
	Quartier q125(TypeQuartier::Marche);
	Quartier q126(TypeQuartier::Caserne);
	tuiles.emplace_back(q124, q125, q126);

	Quartier q127(TypeQuartier::Habitation);
	Carriere q128;
	Place q129(TypeQuartier::Caserne, NbEtoiles::deux);
	tuiles.emplace_back(q127, q128, q129);

	Quartier q130(TypeQuartier::Habitation);
	Carriere q131;
	Quartier q132(TypeQuartier::Marche);
	tuiles.emplace_back(q130, q131, q132);

	Carriere q133;
	Carriere q134;
	Quartier q135(TypeQuartier::Habitation);
	tuiles.emplace_back(q133, q134, q135);

	Quartier q136(TypeQuartier::Habitation);
	Place q137(TypeQuartier::Jardin, NbEtoiles::trois);
	Quartier q138(TypeQuartier::Caserne);
	tuiles.emplace_back(q136, q137, q138);

	Quartier q139(TypeQuartier::Habitation);
	Quartier q140(TypeQuartier::Habitation);
	Carriere q141;
	tuiles.emplace_back(q139, q140, q141);

	Quartier q142(TypeQuartier::Marche);
	Quartier q143(TypeQuartier::Habitation);
	Carriere q144;
	tuiles.emplace_back(q142, q143, q144);

	Place q145(TypeQuartier::Habitation, NbEtoiles::une);
	Quartier q146(TypeQuartier::Temple);
	Quartier q147(TypeQuartier::Marche);
	tuiles.emplace_back(q145, q146, q147);

	Carriere q148;
	Place q149(TypeQuartier::Caserne, NbEtoiles::deux);
	Carriere q150;
	tuiles.emplace_back(q148, q149, q150);

	Quartier q151(TypeQuartier::Marche);
	Quartier q152(TypeQuartier::Habitation);
	Place q153(TypeQuartier::Jardin, NbEtoiles::trois);
	tuiles.emplace_back(q151, q152, q153);

	Quartier q154(TypeQuartier::Habitation);
	Quartier q155(TypeQuartier::Marche);
	Place q156(TypeQuartier::Temple, NbEtoiles::deux);
	tuiles.emplace_back(q154, q155, q156);

	Carriere q157;
	Quartier q158(TypeQuartier::Habitation);
	Quartier q159(TypeQuartier::Habitation);
	tuiles.emplace_back(q157, q158, q159);

	Quartier q160(TypeQuartier::Habitation);
	Carriere q161;
	Quartier q162(TypeQuartier::Jardin);
	tuiles.emplace_back(q160, q161, q162);

	Quartier q163(TypeQuartier::Caserne);
	Quartier q164(TypeQuartier::Habitation);
	Carriere q165;
	tuiles.emplace_back(q163, q164, q165);

	Quartier q166(TypeQuartier::Marche);
	Carriere q167;
	Quartier q168(TypeQuartier::Habitation);
	tuiles.emplace_back(q166, q167, q168);

	Quartier q169(TypeQuartier::Habitation);
	Carriere q170;
	Quartier q171(TypeQuartier::Caserne);
	tuiles.emplace_back(q169, q170, q171);

	Place q172(TypeQuartier::Habitation, NbEtoiles::une);
	Carriere q173;
	Quartier q174(TypeQuartier::Marche);
	tuiles.emplace_back(q172, q173, q174);

	Carriere q175;
	Quartier q176(TypeQuartier::Habitation);
	Quartier q177(TypeQuartier::Temple);
	tuiles.emplace_back(q175, q176, q177);

	Place q178(TypeQuartier::Marche, NbEtoiles::deux);
	Carriere q179;
	Quartier q180(TypeQuartier::Temple);
	tuiles.emplace_back(q178, q179, q180);

	Carriere q181;
	Quartier q182(TypeQuartier::Caserne);
	Carriere q183;
	tuiles.emplace_back(q181, q182, q183);


} // Fin de la fonction instancierTuiles




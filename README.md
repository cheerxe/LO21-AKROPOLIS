# Akropolis - Projet C++ LO21

## À propos du projet

Ce projet consiste en une implémentation en C++ du jeu de société Akropolis, réalisé dans le cadre du cours LO21. L'objectif était de concevoir une application permettant de jouer à Akropolis en mode console et avec une interface Qt.

**État actuel** : Le projet ne compile pas, probablement en raison d'un cycle d'inclusion dans les headers. L'architecture complète est documentée dans le dossier UML.

## Architecture envisagée

L'architecture finale devait reposer sur une séparation stricte des responsabilités selon le modèle MVC :

**Couche Modèle** : entités métier (Hexagone, Tuile, GrilleHexa, Cité, Pioche, Table, Partie) 

**Couche Contrôleur** : orchestration du jeu via ControleurJoueur (gestion d'un tour) et ControleurPartie (gestion complète d'une partie, de l'initialisation aux résultats). Point d'entrée simplifié depuis le main.

**Couche Vue** : design pattern Strategy permettant de basculer entre affichage console et Qt. Interface abstraite IDialogueStrategy implémentée par DialogueConsole et DialogueQt (prévue). Namespaces de rendu séparés (ConsoleRendering, TuileRendering, GrilleRendering) pour découpler totalement l'affichage du modèle.

**Polymorphisme** : classe abstraite IParticipant permettant de gérer uniformément les Joueurs et l'IllustreArchitecte.

Cette architecture visait à permettre le changement d'interface sans toucher à la logique métier, et à faciliter les tests unitaires.

## Difficultés rencontrées

**Cycle d'inclusion de headers** : découvert le jour du rendu, ce problème a empêché la compilation de l'architecture refactorisée malgré de nombreuses heures de débogage. Les classes se référençaient circulairement, paralysant le compilateur. L'architecture complète (contrôle/affichage/modèle) n'a donc pas pu être intégrée au rendu final. Le code est disponible sur le repository GitHub mais ne compile toujours pas.

**Asynchronicité de Qt** : l'architecture synchrone du code console s'est révélée incompatible avec le modèle événementiel de Qt (slots et signaux). Toute la structure aurait dû être repensée en machine à états asynchrone, ce qui était irréalisable dans les délais. Cette contrainte aurait dû être anticipée dès la conception initiale.

**Intégration tardive** : les classes de base devaient être terminées pour le livrable 2 ou 3, permettant ensuite de travailler sur l'architecture avancée. En réalité, elles n'ont été finalisées que quelques jours avant le rendu, rendant impossible l'intégration sereine de la refonte architecturale.

## Apprentissages

**Sur le plan technique**

La conception architecturale doit anticiper les contraintes des frameworks dès le début. Un prototype Qt réalisé tôt aurait révélé le problème d'asynchronicité. La gestion des dépendances circulaires nécessite une vigilance constante : forward declarations systématiques, graphe de dépendances acyclique, tests de compilation réguliers. Les design patterns ne sont pas des solutions miracles et doivent être adaptés au contexte.

L'absence de tests unitaires et d'intégration continue a retardé la détection des problèmes. Un pipeline CI/CD aurait permis de détecter les erreurs de compilation à chaque commit plutôt qu'à l'intégration finale.

**Sur le plan organisationnel**

L'absence de chef de projet a été le principal facteur d'échec organisationnel. Personne ne tranchait sur les priorités, ne recadrait les dérives de planning, ne validait l'atteinte des jalons.

La méthodologie AGILE a été mal appliquée : pas de Definition of Done, pas de revue de sprint, dérive du backlog avec nouvelles tâches démarrées avant validation des précédentes. Les jalons auraient dû être bloquants : impossible de passer au suivant tant que le précédent n'est pas validé.

J'ai laissé mes camarades commencer des tâches secondaires (sérialisation) alors que les classes principales n'étaient pas terminées. Cette tolérance excessive a créé un goulot d'étranglement en fin de projet. Le recadrage n'est pas de l'autoritarisme mais une responsabilité envers le collectif.

La communication était insuffisante : peu de visibilité sur l'avancement réel, découverte tardive des blocages, absence de daily standups. Le diagramme de Gantt créé en livrable 3 aurait dû exister dès le début, être mieux détaillé et être mis à jour continuellement.

## Leçons pour l'avenir

**Leadership** : tout projet nécessite un pilote identifié avec mandat clair. La prochaine fois, j'assumerai ce rôle dès le début si l'équipe me fait confiance. Le leadership n'est pas de l'autoritarisme, c'est une responsabilité au service du projet.

**Planification** : définir des jalons avec critères de validation objectifs. Bloquer le passage au jalon suivant tant que les critères ne sont pas remplis. Prévoir des marges de sécurité dans le planning.

**Communication** : instaurer des rituels réguliers (standups, reviews, retrospectives). Utiliser des outils de suivi visibles (Kanban). Encourager la transparence sur les difficultés et les blocages.

**Conception technique** : prototyper tôt les parties risquées. Valider l'architecture avant l'implémentation. Anticiper les dépendances et s'assurer qu'elles sont acycliques. Mettre en place des tests et de l'intégration continue dès le début.

**Gestion d'équipe** : répartir les tâches selon les compétences. Ne pas laisser quelqu'un bloquer seul plus de 24h. Pratiquer le pair programming et les revues de code systématiques.

Ce projet n'a pas atteint ses objectifs techniques mais je le considère comme un succès pédagogique. J'ai appris l'importance de l'organisation, du leadership et de la rigueur dans le pilotage d'un projet collectif. 

## Note finale

Ce README a été rédigé quelques jours après la deadline, dans un esprit de réflexion honnête. Bien que ma contribution officielle soit estimée à 28% dans le rapport (le travail sur les contrôleurs et l'affichage n'ayant pas pu être intégré), j'ai fourni une part importante du travail technique tout en apprenant que le leadership et l'organisation sont tout aussi essentiels que la technique.

---

*Projet réalisé dans le cadre du cours LO21 - 2024*

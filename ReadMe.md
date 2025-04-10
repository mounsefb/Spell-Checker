**Projet Correction Orthographique**


**Description du Projet**
Le projet de correction orthographique vise à développer un outil capable de détecter les fautes d'orthographe dans un texte donné. Le programme utilise différentes structures de données telles que les arbres, les tableaux redimensionnables, et les méthodes de recherche telles que la recherche séquentielle ou dichotomique.

**Structure du Projet**
Le dossier racine du projet est organisé comme suit :

										├── ReadMe.md
										├── rapport.pdf
										├── Makefile
										├── ressources
											└── dicos
											└── textes
										├── include
										├── src
										├── test
										├── obj
										├── bin

*ReadMe.md*: Contient des informations sur le projet, son utilisation et dautres détails pertinents.
*rapport.pdf*: Un document PDF présentant un rapport détaillé sur le projet.
*Makefile*: Fichier de configuration pour la compilation du projet.
*ressources*: Répertoire contenant les textes à vérifier et les dictionnaires utilisés dans le projet.
*include*: Fichiers den-tête nécessaires à la compilation.
*src*: Implémentation du projet.
*test*: Programmes de test des structures de données. Les résultats sont stockés dans les répertoires *obj* et *bin*.

**Compilation**
La compilation du projet se fait en exécutant la commande *make* à la racine du projet. Cela génère les fichiers exécutables dans le répertoire bin et les fichiers objets dans le répertoire obj.

Pour nettoyer les fichiers objets et binaires, utilisez la commande :
*make clean*

**Exécution des Commandes**

Pour les tests complets d'ensemble, de trie et d'arbre avec ou sans compression :

*./bin/test_$(nom_structure)_dico $(chemin_dico)/$(nom_dictionnaire) $(chemin_texte)/$(texte_a_verifier) -unit*

ou

*./bin/test_$(nom_structure)_dico $(nom_dictionnaire) $(texte_a_verifier) -courbe*

Un exemple concret d'utilisation :

*./bin/test_dumb_dico ressources/dicos/dicoSansAccent.txt ressources/textes/a_la_recherche_du_temps_perdu.txt -unit*

L'exécution des tests unitaires et tableaux redimensionnables se fait simplement en appelant le fichier exécutable correspondant à la structure voulue. Aucun autre argument en entrée nest attendu.

*./bin/test_unit_$(nom_structure)*

ou

*./bin/test_tab_$(nom_methode_recherche)

Cet exemple exécute une correction orthographique sur le texte "a_la_recherche_du_temps_perdu.txt" à partir du dictionnaire "dicoSansAccent.txt" en utilisant un arbre lexicographique naïf pour la structure de données.

Le paramètre *dumb* peut être remplacé par *hash* et *trie* selon le besoin.

Remarques :
L'exécution des commandes se fait toujours à la racine du projet.
La compression du dictionnaire est activée en utilisant l'option -compress en cinquième argument à l'exécution.
Les résultats et les statistiques, y compris la mémoire occupée par le dictionnaire, sont affichés pendant l'exécution du programme.
Les résultats peuvent également être visualisés avec l'utilisation de Gnuplot, qui est appelé automatiquement pour construire la courbe.
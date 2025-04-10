#include "trie_dict.h"
#include "mystring.h"
#include "vect.h"
#include <time.h>
#include "utilitaire_commun.h"

void charger_dictionnaire(const char *nomFichier, tree_t *dictionnaire) {
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", nomFichier);
        return;
    }

    char *mot = malloc(MAX_SIZE_CHAR * sizeof(char));
    while (fgets(mot, MAX_SIZE_CHAR, fichier)) {
        mot[strlen(mot) - 1] = '\0';
        add_empreinte_memoire(strlen(mot) * sizeof(char));
        tree_add(*dictionnaire, mot);
    }

    free(mot);
    fclose(fichier);
}

void rechercher_mots(const char *nomFichier, tree_t dictionnaire, tree_t *motsNonExistants) {
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", nomFichier);
        return;
    }

    char *mot_verif = malloc(MAX_SIZE_CHAR * sizeof(char));
    int *res;
    int quitter = 0;

    while (lecture(fichier, mot_verif)) {
        if (isupper(mot_verif[0]) == 0 && isalpha(mot_verif[0])) {
            res = tree_search(dictionnaire, mot_verif);

            if (res == NULL || res[0] == 0) {
                tree_add(*motsNonExistants, mot_verif);
                if (res == NULL) {
                    free(mot_verif);
                    mot_verif = malloc(MAX_SIZE_CHAR * sizeof(char));
                }
            }

            if (res != NULL && res[1] == 1) {
                char *tmpExt = malloc(MAX_SIZE_CHAR * sizeof(char));

                while (res != NULL && res[1] == 1) {
                    char *tmpInt = malloc(MAX_SIZE_CHAR * sizeof(char));
                    free(res);

                    if (lecture(fichier, tmpInt)) {
                        int len = strlen(mot_verif);
                        if (len + 1 < MAX_SIZE_CHAR) {
                            mot_verif[len] = ' ';
                            mot_verif[len + 1] = '\0';
                            strncat(mot_verif, tmpInt, MAX_SIZE_CHAR - len - 1);
                            res = tree_search(dictionnaire, mot_verif);
                        } else {
                            free(tmpInt);
                            free(tmpExt);
                            break;
                        }
                    } else {
                        quitter = 1;
                        free(tmpInt);
                        free(tmpExt);
                        break;
                    }

                    strcpy(tmpExt, tmpInt);
                    free(tmpInt);
                }

                if (quitter) break;

                if (res == NULL || res[0] == 0) {
                    if (res != NULL) free(res);
                    res = tree_search(dictionnaire, tmpExt);

                    if (res != NULL && res[0] == 0) {
                        tree_add(*motsNonExistants, tmpExt);
                    }
                }

                free(tmpExt);
            }

            free(res);
        }
    }

    free(mot_verif);
    fclose(fichier);
}

void liberer_tries(tree_t *trie1, tree_t *trie2) {
    tree_delete(*trie1);
    tree_delete(*trie2);
}

void unitaire(int argc, char const *argv[]) {
    clock_t start_chargement, end_chargement, start_compression, end_compression, start_recherche, end_recherche;
    double taille;
    reset_memoire();

    start_chargement = clock();
    tree_t mytree = tree_new();
    charger_dictionnaire(argv[1], &mytree);
    end_chargement = clock();

    double duree_chargement = ((double)(end_chargement - start_chargement)) / CLOCKS_PER_SEC;
    printf("Duree chargement = %lf sec\n", duree_chargement);
    taille = (double)get_empreinte_memoire() / UN_MB;
    printf("Taille memoire occupée par le dictionnaire %f MB\n", taille);

    if (argc == 5 && strcmp(argv[4], "-compress") == 0) {
        printf("Compression du dictionnaire en cours... Cela peut prendre jusqu'à plusieurs minutes pour un très grand dictionnaire...\n");
        start_compression = clock();
        mytree = compress_tree(mytree);
        end_compression = clock();
        double duree_compression = ((double)(end_compression - start_compression)) / CLOCKS_PER_SEC;
        printf("La compression a fait gagner %lf MB et a durée %lf sec\n", (double)get_memoire_economisee() / UN_MB, duree_compression);
    }

    start_recherche = clock();
    tree_t mots_non_existants = tree_new();
    rechercher_mots(argv[2], mytree, &mots_non_existants);
    end_recherche = clock();
    tree_printf_readable(mots_non_existants);
    double duree_recherche = ((double)(end_recherche - start_recherche)) / CLOCKS_PER_SEC;
    int nbre_mot_manquant = get_nbre_manquant_tree(mots_non_existants);
    printf("Duree recherche = %lf sec\n", duree_recherche);
    printf("%d mots pas dans le dico\n", nbre_mot_manquant);
    printf("Liberation des tries\n");
    liberer_tries(&mots_non_existants, &mytree);
}

void construction_courbe(int argc, char const *argv[]) {
    clock_t start_chargement, end_chargement, start_compression, end_compression, start_recherche, end_recherche;
    int nbwords = linecounter(argv[1]);
    FILE* ftemps=fopen("temps_trie_dico.dat","w");
    double taille;
    // int k=nbwords;
    for (int k=nbwords; k>1; k-=nbwords/100) {
        reset_memoire();

        start_chargement = clock();
        tree_t mytree = tree_new();
        FILE *fichier = fopen(argv[1], "r");

        if (fichier == NULL) {
            fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", argv[1]);
            return;
        }

        char *mot = malloc(MAX_SIZE_CHAR * sizeof(char));
        int count = 0;
        while (fgets(mot, MAX_SIZE_CHAR, fichier) && count<k) {
            mot[strlen(mot) - 1] = '\0';
            // add_empreinte_memoire(strlen(mot) * sizeof(char));
            tree_add(mytree, mot);
            count++;
        }

        free(mot);
        fclose(fichier);
        end_chargement = clock();

        double duree_chargement = ((double)(end_chargement - start_chargement)) / CLOCKS_PER_SEC;
        printf("Duree chargement = %lf sec\n", duree_chargement);
        taille = (double)get_empreinte_memoire() / UN_MB;
        printf("Taille memoire occupée par le dictionnaire %f MB\n", taille);

        if (argc == 5 && strcmp(argv[4], "-compress") == 0) {
            printf("Compression du dictionnaire en cours... Cela peut prendre jusqu'à plusieurs minutes pour un très grand dictionnaire...\n");
            start_compression = clock();
            mytree = compress_tree(mytree);
            end_compression = clock();
            double duree_compression = ((double)(end_compression - start_compression)) / CLOCKS_PER_SEC;
            printf("La compression a fait gagner %lf MB et a durée %lf sec\n", (double)get_memoire_economisee() / UN_MB, duree_compression);
        }

        start_recherche = clock();
        tree_t mots_non_existants = tree_new();
        rechercher_mots(argv[2], mytree, &mots_non_existants);
        end_recherche = clock();

        double duree_recherche = ((double)(end_recherche - start_recherche)) / CLOCKS_PER_SEC;
        int nbre_mot_manquant = get_nbre_manquant_tree(mots_non_existants);
        printf("Duree recherche = %lf sec\n", duree_recherche);
        printf("%d mots pas dans le dico\n", nbre_mot_manquant);
        printf("Liberation des tries\n");
        liberer_tries(&mots_non_existants, &mytree);
        fprintf(ftemps,"%d %lf %lf\n",k,taille,duree_recherche);
        puts("");
    }

    fclose(ftemps);

     system("gnuplot -p -e \"f(x) = a * (x) + b ; fit f(x) 'temps_trie_dico.dat' u 1:2 via a,b;"
                "set y2range [0:2000];set y2tics nomirror;"
                "set xlabel 'Taille du dictionnaire'; set ylabel 'Temps (s)'; set y2label 'Taille mémoire (MB)';"
                "plot 'temps_trie_dico.dat' u 1:2 with line lt 4 axis x1y2 title 'Temps', a * (x) + b  with line lt 6 axis x1y2 title 'Fit Taille';"
                "g(x) = c * (x) + d ;fit g(x) 'temps_trie_dico.dat' u 1:3 via c,d;"
                "replot 'temps_trie_dico.dat' u 1:3 with line lt 2 title 'Recherche Trie', g(x)  with line lt 8 title 'Fit Recherche'; quit\"");
}

int main(int argc, char const *argv[]) {
    if (argc < 4) {
        printf("l'execution se fait au moins à quatre arguments, le nom de l'executable, le nom du dico, le texte à vérifier, -unit pour réaliser un test unitaire / -courbe pour construire une courbe de tendance\n");
        exit(EXIT_FAILURE);
    } else if (strcmp(argv[3], "-unit") == 0) {
        unitaire(argc, argv);
    } else if (strcmp(argv[3], "-courbe") == 0) {
        construction_courbe(argc, argv);
    } else {
        printf("l'execution se fait au moins à quatre arguments, le nom de l'executable, le nom du dico, le texte à vérifier, -unit pour réaliser un test unitaire / -courbe pour construire une courbe de tendance\n");
        exit(EXIT_FAILURE);
    }
    return 1;
}
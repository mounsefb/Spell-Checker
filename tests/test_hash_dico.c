#include "hashsetext.h"
#include "mystring.h"
#include "vect.h"
#include <time.h>
#include "utilitaire_commun.h"
#include <sys/stat.h>

void charger_dictionnaire(const char *nom_fichier, hashlset_t *hash) {
    FILE *fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    char *mot = malloc(MAX_SIZE_CHAR * sizeof(char));
    int count = 0;

    while (fgets(mot, MAX_SIZE_CHAR, fichier)) {
        mot[strlen(mot) - 1] = '\0';
        count++;
        add_empreinte_memoire(strlen(mot) * sizeof(char));
        hashlset_put(mot, *hash);
        mot = malloc(MAX_SIZE_CHAR * sizeof(char));
    }

    free(mot);
    fclose(fichier);
}

void rechercher_mots(const char *nom_fichier, hashlset_t dictionnaire, hashlset_t *mots_non_existants) {
    FILE *fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", nom_fichier);
        exit(EXIT_FAILURE);
    }

    char *mot_verif = calloc(MAX_SIZE_CHAR, sizeof(char));
    int *res;
    int quitter = 0;

    while (lecture(fichier, mot_verif)) {
        if (isupper(mot_verif[0]) == 0) {
            res = hashlset_find_key(mot_verif, dictionnaire);

            if (res == NULL) {
                hashlset_put(mot_verif, *mots_non_existants);
                mot_verif = calloc(MAX_SIZE_CHAR, sizeof(char));
            } else if (res[1] == 1) {
                char *tmp_ext = malloc(MAX_SIZE_CHAR * sizeof(char));

                while (res != NULL && res[1] == 1) {
                    char *tmp_int = malloc(MAX_SIZE_CHAR * sizeof(char));
                    free(res);

                    if (lecture(fichier, tmp_int)) {
                        int len = strlen(mot_verif);
                        mot_verif[len] = ' ';
                        mot_verif[len + 1] = '\0';
                        strcat(mot_verif, tmp_int);
                        res = hashlset_find_key(mot_verif, dictionnaire);
                    } else {
                        quitter = 1;
                        free(tmp_int);
                        free(tmp_ext);
                        break;
                    }

                    strcpy(tmp_ext, tmp_int);
                    free(tmp_int);
                }

                if (quitter) break;

                if (res == NULL && (tmp_ext != NULL && strcmp(tmp_ext, "") != 0)) {
                    res = hashlset_find_key(tmp_ext, dictionnaire);

                    if (res != NULL && res[0] == 0) {
                        hashlset_put(tmp_ext, *mots_non_existants);
                    }
                }

                free(tmp_ext);
            }

            free(res);
        }
    }

    free(mot_verif);
    fclose(fichier);
}

void unitaire(int argc, char const *argv[]) {
    clock_t start_chargement, end_chargement, start_recherche, end_recherche;
    double taille;
    reset_memoire();
    start_chargement = clock();

    hashlset_t dictionnaire = hashlset_new(100000, hash_char, char_fprintf, char_delete, char_equal);
    charger_dictionnaire(argv[1], &dictionnaire);

    end_chargement = clock();
    double duree_chargement = ((double)(end_chargement - start_chargement)) / CLOCKS_PER_SEC;
    taille = (double)get_empreinte_memoire() / UN_MB;
    printf("Taille mémoire occupée par le dictionnaire : %f MB\n", taille);
    printf("Durée chargement du dictionnaire : %f sec\n", duree_chargement);

    start_recherche = clock();
    hashlset_t mots_non_existants = hashlset_new(1000, hash_char, char_fprintf, char_delete, char_equal);
    rechercher_mots(argv[2], dictionnaire, &mots_non_existants);
    end_recherche = clock();

    double duree_recherche = ((double)(end_recherche - start_recherche)) / CLOCKS_PER_SEC;

    hashlset_fprintf_readable(mots_non_existants, stdout);
    printf("Durée recherche totale à partir du fichier à vérifier : %f sec\n", duree_recherche);
    printf("%d mots non retrouvés dans le dictionnaire \n", mots_non_existants->total_number);
    printf("Libération des ensembles\n");
    // puts("");

    hashlset_delete(mots_non_existants);
    hashlset_delete(dictionnaire);
    // puts("");
}



void construction_courbe(int argc, char const *argv[]) {

    clock_t start_chargement, end_chargement, start_recherche, end_recherche;
    int nbwords = linecounter(argv[1]);
    FILE *ftemps = fopen("temps_hash_dico.dat", "w");
    double taille;
    // int k = nbwords;
    for (int k = nbwords; k > 1; k -= nbwords / 10) {
        reset_memoire();
        start_chargement = clock();

        hashlset_t dictionnaire = hashlset_new(100000, hash_char, char_fprintf, char_delete, char_equal);
        FILE *fichier = fopen(argv[1], "r");

        if (fichier == NULL) {
            fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", argv[1]);
            return;
        }

        char *mot = malloc(MAX_SIZE_CHAR * sizeof(char));
        int count = 0;
        while (fgets(mot, MAX_SIZE_CHAR, fichier) && count<k) {
            mot[strlen(mot) - 1] = '\0';
            add_empreinte_memoire(strlen(mot) * sizeof(char));
            hashlset_put(mot, dictionnaire);
            count++;
            mot = malloc(MAX_SIZE_CHAR * sizeof(char));
        }

        free(mot);
        fclose(fichier);
        end_chargement = clock();
        double duree_chargement = ((double)(end_chargement - start_chargement)) / CLOCKS_PER_SEC;
        taille = (double)get_empreinte_memoire() / UN_MB;
        printf("Taille mémoire occupée par le dictionnaire : %f MB\n", taille);
        printf("Durée chargement du dictionnaire : %f sec\n", duree_chargement);

        start_recherche = clock();
        hashlset_t mots_non_existants = hashlset_new(1000, hash_char, char_fprintf, char_delete, char_equal);
        rechercher_mots(argv[2], dictionnaire, &mots_non_existants);
        end_recherche = clock();

        double duree_recherche = ((double)(end_recherche - start_recherche)) / CLOCKS_PER_SEC;

        // hashlset_fprintf_readable(mots_non_existants, stdout);
        printf("Durée recherche totale à partir du fichier à vérifier : %f sec\n", duree_recherche);
        printf("%d mots non retrouvés dans le dictionnaire \n", mots_non_existants->total_number);
        printf("Libération des ensembles\n");
        puts("");

        hashlset_delete(mots_non_existants);
        hashlset_delete(dictionnaire);
        puts("");

        fprintf(ftemps, "%d %lf %lf\n", k, taille, duree_recherche);
    }

    fclose(ftemps);

system("gnuplot -p -e \"f(x) = a * (x) + b ; fit f(x) 'temps_hash_dico.dat' u 1:2 via a,b;"
       "set y2range [0:100];set y2tics nomirror;"
       "set xlabel 'Taille du dictionnaire'; set ylabel 'Temps (s)'; set y2label 'Taille mémoire (MB)';"  // Ajout des titres aux axes
       "plot 'temps_hash_dico.dat' u 1:2 with line lt 4 axis x1y2 title 'Taille', a * (x) + b  with line lt 6 axis x1y2 title 'Fit Taille';"
       "g(x) = c * (x) + d ;fit g(x) 'temps_hash_dico.dat' u 1:3 via c,d;"
       "replot 'temps_hash_dico.dat' u 1:3 with line lt 2 title 'Recherche Ensemble', g(x)  with line lt 8 title 'Fit Recherche'; quit\"");

}


void liberer_hash(hashlset_t *trie1, hashlset_t *trie2) {
    hashlset_delete(*trie1);
    hashlset_delete(*trie2);
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
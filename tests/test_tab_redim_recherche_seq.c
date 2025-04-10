#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mystring.h"
#include "vect.h"
#include "utilitaire_commun.h"
// int lecture(FILE* fp, char* mot) {
//     char inter[512];
//     int j, c;
//     j = 0;
//     do {
//         c = fgetc(fp);
//         if (c == EOF) return 0;
//         if (c == '\n' || c == '\r') break;
//         if (!ispunct(c) && !isspace(c)) {
//             inter[j++] = c;
//         } else if (c == '\'') {
//             j = 0;
//         } else if (c == '-' && j) {
//             if (j > 1 && !isspace(inter[j - 2]) && !ispunct(inter[j - 2]) && !strchr("à-ü", inter[j - 2])) {
//                 break;
//             } else {
//                 inter[j++] = c;
//             }
//         } else if (c == ' ') {
//             if (j && ispunct(inter[j - 1])) {
//                 j--;
//             } else {
//                 break;
//             }
//         } else {
//             break;
//         }
//     } while (1);
//     if (j && inter[j - 1] == '-') {
//         j--;
//     }
//     inter[j] = 0;
//     if (*inter == '\xe2' && *(inter + 1) == '\x80' && *(inter + 2) == '\x98') {
//       memmove(inter, inter + 3, strlen(inter + 3) + 1); 
//     }
//     strcpy(mot, inter);
//     return 1;
// }

/*
int lecture(FILE* fp, char* mot) {
    char inter[512];
    int j,c;
    j=0;
    do {
      c=fgetc(fp);
      if (c==EOF) return 0;
      if (c=='\n' || c=='\r') break;
      if (!ispunct(c) && !isspace(c)) inter[j++]=c;
      else if (c=='\'') j=0;
      else if (c=='-' && j) inter[j++]=c;
      else if (c==' ') {
        if (j && ispunct(inter[j-1])) j--;
        else break;
      }
      else  break;
    } while (1);
    if (j && inter[j-1]=='-') j--;
    inter[j]=0;
    strcpy(mot,inter);
    return 1;
  }
*/
int make_tab(FILE* file, vect_t tab) {
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char mot[30];
    while (lecture(file, mot)) {
        if (strlen(mot)>0) {
            // printf("Read word : %s\n", mot); // Pour vérifier que la lecture marche bien
            char *new_mot = (char *)malloc(strlen(mot) + 1);
            if (new_mot == NULL) {
                perror("Memory allocation error");
                fclose(file);
                vect_delete(tab);
                return 1;
            }
            strcpy(new_mot, mot);
            vect_append(new_mot, tab);
        }
        
    }
    return 0;
}

int close_tab(vect_t tab) {
    size_t j;
    for (j = 0; j < tab->actual_size; j++) {
        free(tab->data[j]);
        tab->data[j] = NULL;
    }
    vect_delete(tab);
    return 0;
}

int sequential_search(char* mot, vect_t dico) {
    size_t i;
    for (i = 0; i < dico->actual_size; i++) {
        char* mot_dico = (char*)dico->data[i];
        if (char_equal(mot, mot_dico)) {
            return 1; // Le mot a été trouvé dans le dictionnaire
        }
    }
    return 0; // Le mot n'a pas été trouvé dans le dictionnaire
}

int check_dico(vect_t txt, vect_t dico) {
    size_t i;
    vect_t mots_uniques = vect_new(1000, char_fprintf, char_delete, char_equal, char_append, char_equal);

    for (i = 0; i < txt->actual_size; i++) {
        char* mot_texte = (char*)txt->data[i];

        if (strlen(mot_texte) == 0) {
            continue;  // Passe au mot suivant
        }

        int not_found = !sequential_search(mot_texte, dico);
        int not_seen_before = !sequential_search(mot_texte, mots_uniques);

        if (not_found && not_seen_before) {  // Si le mot n'est pas dans le dictionnaire et n'a pas déjà été vu
            printf("Le mot unique '%s' n'est pas dans le dictionnaire. C'est le '%ld' eme mot sur '%d'\n"
            , mot_texte, i, txt->actual_size);
            char *new_mot = (char *)malloc(strlen(mot_texte) + 1);
            strcpy(new_mot, mot_texte);
            vect_append(new_mot, mots_uniques);
        }
    }
    vect_print(mots_uniques,stdout);
    vect_delete(mots_uniques); 
    return 0;
}

  int main() {
      FILE *file;
      vect_t dico = vect_new(100000, char_fprintf, char_delete, char_equal, char_append, char_equal);
      vect_t texte = vect_new(100000, char_fprintf, char_delete, char_equal, char_append, char_equal);

      file = fopen("ressources/dicos/dicoSansAccent.txt", "r");
      make_tab(file,dico);
      fclose(file);

      file = fopen("ressources/textes/a_la_recherche_du_temps_perdu.txt", "r");
      make_tab(file,texte);
      fclose(file);

      check_dico(texte,dico);

      /*
      vect_print(dico, stdout);
      vect_print(texte, stdout);
      */

      close_tab(dico);
      close_tab(texte);

      return 0;
  }

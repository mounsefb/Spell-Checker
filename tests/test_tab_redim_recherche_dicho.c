#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mystring.h"
#include "vect.h"
#include "utilitaire_commun.h"


int make_tab(FILE* file, vect_t tab) {
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char mot[30];
    while (lecture(file, mot)) {
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

int dichotomique_search(char* mot, vect_t dico, int bas, int haut) {
    int ind = (bas + haut)/2;
    if (bas >= haut) {
        printf("[ECHEC] Le mot n'a pas été trouvé dans le dictionnaire\n");
        return 0;  // Le mot n'a pas été trouvé dans le dictionnaire
    }

    char* mot_dico = (char*)dico->data[ind];

    if (char_equal(mot, mot_dico)==1) {
        printf("[SUCCES] Le mot a été trouvé dans le dictionnaire \n");
        return 1;  // Le mot a été trouvé dans le dictionnaire
    } else if (char_compare(mot, mot_dico) > 0) {
        return dichotomique_search(mot, dico, ind + 1, haut);
    } else if (char_compare(mot, mot_dico) < 0) {
        return dichotomique_search(mot, dico, bas, ind - 1);
    }
    printf("[ECHEC] Le mot n'a pas été trouvé dans le dictionnaire\n");
    return 0;
}



int check_dico_bis(vect_t dico, FILE* fichier_a_verifier, vect_t mots_manquants){
    if(fichier_a_verifier==NULL){
        printf("le ficheir donné en entrée n'est pas ouvert");
        return 0;
    }
    if(dico == NULL || mots_manquants == NULL){
        printf("les tableaux en entrée ne sont pas initialisés");
        return 0;
    }
    char * mot_verif = malloc(512*sizeof(char));
    while(lecture(fichier_a_verifier, mot_verif)){
        if(dichotomique_search(mot_verif, dico,0,dico->actual_size)){
            vect_append(mot_verif, mots_manquants);
            mot_verif = malloc(512*sizeof(char));
        }
    }
    return 1;
}

void afficher_dico(vect_t dico){
    if (dico==NULL) return;
    if (dico->fprint_data){
        for (unsigned int i=0; i<dico->actual_size; i++){
            dico->fprint_data(dico->data[i], stdout); puts("");
        } 
    }
    puts("");
}

int main() {
    FILE *file;
    vect_t dico = vect_new(10, char_fprintf, char_delete, char_equal, char_append, char_compare);
    vect_t mots_manquants = vect_new(10, char_fprintf, char_delete, char_equal, char_append, char_compare);

    file = fopen("ressources/dicos/dicoSansAccent.txt", "r");
    make_tab(file,dico);
    fclose(file);
    afficher_dico(dico);

    file = fopen("ressources/textes/a_la_recherche_du_temps_perdu.txt", "r");
    check_dico_bis(dico, file, mots_manquants);
    fclose(file);

    afficher_dico(mots_manquants);
    /*
    vect_print(dico, stdout);
    vect_print(texte, stdout);
    */

    close_tab(dico);
    close_tab(mots_manquants);

    return 0;
}

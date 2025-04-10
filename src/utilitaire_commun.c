
#include "utilitaire_commun.h"
#include <wctype.h>
#include <wchar.h>
#include <locale.h>
#include <stdio.h>

int empreinte_memoire = 0;
int memoire_economisee = 0;

void add_empreinte_memoire(int a_ajouter){
    empreinte_memoire+=a_ajouter;
}

int get_empreinte_memoire(){
    return empreinte_memoire;
}

void reset_memoire(){
    empreinte_memoire = 0;
}

void add_memoire_economisee(int a_ajouter){
    memoire_economisee+=a_ajouter;
}

int get_memoire_economisee(){
    return memoire_economisee;
}

void reset_memoire_economisee(){
    memoire_economisee = 0;
}

// Fonction pour compter les lignes dans un fichier
int linecounter(const char * f){
    FILE * file = fopen(f, "r");
    assert(file!=NULL);
    int count = 0;
    char * tmp = calloc(MAX_SIZE_CHAR, sizeof(char));
    while (!feof(file)){
        fgets(tmp, MAX_SIZE_CHAR, file);
        count++;
    }
    free(tmp);
    fclose(file);
    return count;
}


int lecture(FILE* fp, char* mot) {
    char inter[512];
    int j,c;
    j=0;
    do {
        c=fgetc(fp);
        if (c==EOF) return 0;
        if (c=='\n' || c=='\r') break;
        // ° non pris en compte par ispunct, il faut tout de même l'ignorer
        // on ignore aussi les chaîne de caractère avec chiffre
        if( c==L'°' || isdigit(c)) j=0;
        else if (!ispunct(c) && !isspace(c)) inter[j++]=c;
        else if (c=='\'') j=0;
        else if (c=='-' && j) inter[j++]=c;
        else if (c==' ') {
            if (j && ispunct(inter[j-1])) j--;
            else break;
        }
        else break;
    } while (1);
    if (j && inter[j-1]=='-') j--;
    inter[j]=0;

    strcpy(mot, inter);
    return 1;
}

// Fonction de hachage pour les chaînes de caractères
unsigned int hash_char(void * key){
    char* char_key = (char*)key;
    unsigned int h = 0, i=0;
    while(char_key[i] != '\0'){
        h+=tolower(char_key[i]);
        i++;
    }
    return h;
}

unsigned int hash_double(void* key) {
    // Fonction de hachage simple pour les doubles
    double* double_key = (double*)key;
    unsigned int h = (unsigned int)(*double_key);
    return h;
}

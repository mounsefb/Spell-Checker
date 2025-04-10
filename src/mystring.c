#include "mystring.h"


// Fonction pour créer un nouveau char (chaine de caractères)
void* char_new(void * val) {return strdup((char*)val);}

  // Cree avec alocation dynamique une chaine contenant AleaXX
void char_random(void* p) {
  char tmp[256];
  sprintf(tmp,"Alea%ld",random()%100);
  *(char**)p=strdup(tmp);
}

// Fonction pour imprimer une chaîne de caractères dans un fichier
void char_fprintf( void * data, FILE* file){
    fprintf(file, "%s", (char*)data);
}

// Fonction pour afficher une chaîne de caractères à l'écran
void char_printf( void * data){
    char_fprintf((char*)data, stdout);
}

// Fonction pour supprimer une chaîne de caractères
int char_delete(void* data) {
    if (data != NULL) {
        int ret = strlen((char*)data)*sizeof(char);
        free(data);
        return ret;
    }
    return 0;
}

// Fonction pour comparer deux chaînes de caractères (insensible à la casse)
int char_equal(void * data1, void * data2){
    if (strcasecmp((char*)data1,(char*)data2)==0) return 1;
    else if(strncmp((char*)data1, (char*)data2, strlen(data2)) == 0) return 2;
    return 0;
}

// Fonction pour comparer deux chaînes de caractères
int char_compare(void * data1, void * data2){
    return strcmp((char*)data1, (char*)data2);
}

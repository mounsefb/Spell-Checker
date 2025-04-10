#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "double.h"

// Fonction pour imprimer un double dasn un fichier
void double_fprintf( void * data, FILE* file){
    fprintf(file, "%lf\t", *((double *) data));
}

// Fonction pour creer une copie allouee dynamiquement d'un double
void* double_new_ptr(void * val){
    double *clone;
    clone=(double*)calloc(1, sizeof(double));
    assert(clone!=NULL);
    *clone=*(double*)val;
    return clone;
}

// Fonction qui renvoie un pointeur vers un nouveau double
double* double_new(double val){
    double *clone;
    clone=(double*)calloc(1, sizeof(double));
    assert(clone!=NULL);
    *clone=val;
    return clone;
}

// Fonction pour imprimer un double à l'ecran
void double_printf(double* val){
    double_fprintf(val, stdout);
}

// Fonction pour liberer la memoire occupee par un double
int double_delete(void * data){
    if (data==NULL) return 1;
    free((double*)data);
    return 1;
}

// Fonction pour comparer deux doubles (egalite)
int double_equal(void * data1, void * data2){
    if (*((double*)data1)==*((double*)data2)) return 1;
    return 0;
}

// Fonction pour copier la valeur d'un double dans un autre
void double_copy(void * src, void * dest){
    *(double*)dest=*(double*)src;
}

// Fonction pour comparer deux doubles
int double_compare(void * a, void * b){
    if (*(double*)a<*(double*)b){
        return -1;
    }
    else if (*(double*)a==*(double*)b){
        return 0;
    }
    return 1;
}
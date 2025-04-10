#include <stdio.h>
#include <assert.h>


#ifndef _DOUBLE_H
#define _DOUBLE_H

// Affichage d'une chaine de type mystring
void double_fprintf(void* val,FILE* fp);
void double_printf(double* val); 
// Test l'egalite de 2 reels
int double_equal(void* pv1, void* pv2);
// Génération d'un reel aléatoire
void double_random(double*p);
// Clone d'un reel
double* double_new(double val);

int double_delete(void * data);
void double_copy(void * src, void * dest);
void* double_new_ptr(void * val);
int double_compare(void * a, void * b);

#endif
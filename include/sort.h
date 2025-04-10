
#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "double.h"
#include "mystring.h"
#include "vect.h"

    //  partitionner le tableau `tab` entre les indices `gauche`et `droite`
int partitionner(vect_t tab, int gauche, int droit);
    //  tri recursif du tableau `tab` entre les indices `gauche`et `droite`
void quick_sortrec(vect_t tab, int gauche, int droit);
    //  Tri du tableau `tab` entier (fonction wrapper cachant les autres paramètres à l'utilisateur) */
void quicksort(vect_t tab);

    /*Fusion des 2 demi tableaux triés de `tab` compris entre les indices `gauche` et `droit` à l'aide du tableau `tmp`*/
void fusion(vect_t tab, int indice_gauche, int indice_mid, int indice_droite);
    /* Tri récursif du tableau `tab` entre les indices `gauche` et `droit` à l'aide du tableau `tmp` */
void trifusionrec(vect_t tab, int indice_gauche, int indice_droite);
    /* Tri du tableau `tab` entier (fonction wrapper cachant les autres paramètres à l'utilisateur) */
void mergesort(vect_t tab);

    /* Tri par tas du tableau `tab`*/
void heapsort(vect_t tab);
void heapify(vect_t tab, int n, int i);
void swap(void **a, void **b);

    /* Tri du tableau `tab` entier (fonction wrapper cachant les autres paramètres à l'utilisateur) */
void segsort(vect_t tab);

#endif 
#ifndef UTILITAIRE_COMMUN_H_
#define UTILITAIRE_COMMUN_H_
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define UN_MB 1000000
#define MAX_SIZE_CHAR 255


void add_empreinte_memoire(int a_ajouter);
int get_empreinte_memoire();
void reset_memoire();
int get_memoire_economisee();
void reset_memoire_economisee();
void add_memoire_economisee(int a_ajouter);

int linecounter(const char * f);
int contains_digit(const char *str);
int lecture(FILE* fp, char* mot);
unsigned int hash_char(void * key);
unsigned int hash_double(void* key);
int lecture_dico_avec_expression(FILE* fp, char* mot);
int nom_propre(char * a_verifier);
#endif
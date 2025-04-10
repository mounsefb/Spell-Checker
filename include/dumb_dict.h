#ifndef LEXICO_NAIF_H
#define LEXICO_NAIF_H

#include "stdio.h"
#include "stdlib.h"
#include <string.h>
// Un noeud sera une structure définie ensuite
struct _node;

// La liste des fils d'un noeud
typedef struct _link {
  struct _node *data;  // Le premier sous arbre partant de ce fils
  struct _link *next;  // Les autres fils
} *list_t;

// Un noeud contient le caractère et la liste des _caracteres_ suivants
typedef struct _node {
  char data;    // le caractère
  list_t sons;    // La liste des noeuds suivants, contenant le caractère suivant
  int ref_count;
} *dict_t;

// Structure pour suivre les nœuds déjà rencontrés
typedef struct _visited_node {
    dict_t original;            // Le nœud original
    struct _visited_node* next; // Nœud suivant dans la liste
} visited_node_t;

void add_empreinte_memoire(int);
int get_empreinte_memoire();
void reset_memoire();
int get_nbre_manquant(dict_t dico);
int get_memoire_economisee();
void reser_memoire_economisee();


// Fonction pour compresser un nœud et ses fils par partage de suffixe
dict_t compress_node(dict_t node, visited_node_t** visited_nodes);
dict_t compress_dict(dict_t root);
dict_t dict_new();
int dict_equal(dict_t tree1, dict_t tree2);
// Affiche le dictionnaire a l'ecran, dasn un fichier.
void dict_fprintf(dict_t l, FILE* fp);
void dict_printf(dict_t l  );

void dict_fprintf_readable(dict_t mydic, FILE* fp, char* current_word);

// Fonction wrapper pour afficher le dictionnaire à l'écran
void dict_printf_readable(dict_t mydic);

// Libère toute la dicte et retourne une dicte vide
// Libere les elements avec delete_data si la fonction existe
dict_t dict_delete(dict_t mydic);
void dict_add_dict(dict_t *encountered, dict_t tree);
int *dict_search(dict_t mydic, char* word);
int dict_add(dict_t mydic, char* word);

#endif 
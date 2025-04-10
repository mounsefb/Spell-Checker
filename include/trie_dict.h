#ifndef TRIE_H
#define TRIE_H

#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#define ALPHABET_SIZE 255
#define WORD_MAX_LENGTH 100

// Structure du trie
typedef struct _node {
    struct _node *sons[ALPHABET_SIZE];
    int ref_count;
    int is_word;
} *tree_t;

// Structure pour suivre les nœuds déjà rencontrés
typedef struct _visited_node {
    tree_t original;            // Le nœud original
    struct _visited_node* next; // Nœud suivant dans la liste
} visited_node_tree_t;


void compteur_mot_tree(tree_t mytree);
int get_nbre_manquant_tree(tree_t dico);

// Creation d'une liste vide, avec les pointeurs de fonctions utiles
tree_t tree_new();
tree_t compress_node_tree(tree_t node, visited_node_tree_t** visited_nodes);
tree_t compress_tree(tree_t root);
int tree_equal(tree_t tree1, tree_t tree2);
// Affiche la treee a l'ecran, dasn un fichier.
void tree_fprintf(tree_t l, FILE* fp);
void tree_printf(tree_t l  );

void tree_fprintf_readable(tree_t mydic, FILE* fp, char* current_word);

// Fonction wrapper pour afficher le treeionnaire à l'écran
int tree_printf_readable(tree_t mydic);

// Libère toute la treee et retourne une treee vide
// Libere les elements avec delete_data si la fonction existe
tree_t tree_delete(tree_t mydic);

int *tree_search(tree_t mydic, char* word);
int tree_add(tree_t mydic, char* word);

#endif 
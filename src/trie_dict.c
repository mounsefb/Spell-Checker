#include "trie_dict.h"
#include <ctype.h>
#include "utilitaire_commun.h"

int compression_tree = 0; 
int nbre_mot_manquant_tree = 0;

// Fonction récursive pour compter les mots manquants dans l'arbre
void compteur_mot_tree(tree_t mytree){
    if(mytree->is_word){
        nbre_mot_manquant_tree++;
    }
    for (int i= 0 ; i<255; i++){
        if(mytree->sons[i]!=NULL){
            compteur_mot_tree(mytree->sons[i]);
        }
    }
}

// Fonction pour obtenir le nombre de mots manquants dans l'arbre
int get_nbre_manquant_tree(tree_t dico){
    nbre_mot_manquant_tree = 0;
    compteur_mot_tree(dico);
    return nbre_mot_manquant_tree;     
}


// Fonctions du trie
tree_t tree_new() {
    add_empreinte_memoire(sizeof(struct _node));
    return calloc(1, sizeof(struct _node));
} 

// Fonction pour compresser un nœud et ses fils par partage de suffixe
tree_t compress_node_tree(tree_t node, visited_node_tree_t** visited_nodes) {
    if (node == NULL) {
        return NULL;
    }
    visited_node_tree_t* current_visited = *visited_nodes;
    while (current_visited != NULL) {
        if (tree_equal(node, current_visited->original)) {
            // Noeud déjà rencontré, retourne le noeud compressé correspondant
            compression_tree = 1;
            tree_delete(node);
            compression_tree = 0;
            current_visited->original->ref_count++;
            return current_visited->original;
        }
        current_visited = current_visited->next;
    }
        
    // Noeud pas encore rencontré, ajoute-le à la liste des nœuds rencontrés
    visited_node_tree_t* new_visited_node = (visited_node_tree_t*)malloc(sizeof(visited_node_tree_t));
    new_visited_node->original = node;
    new_visited_node->next = *visited_nodes;
    *visited_nodes = new_visited_node;

    // Compress les fils du nœud actuel
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if(node->sons[i]!=NULL){
            node->sons[i] = compress_node_tree(node->sons[i], visited_nodes);
        }
    }

    // Retourner le nœud compressé
    return new_visited_node->original;
}


// Fonction principale pour compresser l'arbre par partage de suffixe
tree_t compress_tree(tree_t root) {
    visited_node_tree_t* visited_nodes = NULL; // Liste des nœuds déjà rencontrés

    tree_t res = compress_node_tree(root, &visited_nodes);

    visited_node_tree_t *current_visited = visited_nodes;
    while (current_visited != NULL) {
        visited_node_tree_t * next_visited = current_visited->next;
        free(current_visited);
        current_visited = next_visited;
    }

    return res;
}


int tree_equal(tree_t tree1, tree_t tree2){
    if (tree1 == NULL && tree2 == NULL) {
        return 1;
    } else if (tree1 == NULL || tree2 == NULL) {
        return 0;
    }

    // Compare les sous-arbres recursivement
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if(tree1->sons[i] != NULL && tree2->sons[i] != NULL) {
            if (!tree_equal(tree1->sons[i], tree2->sons[i])) {
                return 0;
            }
        }
        else if(tree1->sons[i] != NULL || tree2->sons[i] != NULL) return 0;
    }

    return 1;
}

void tree_fprintf_readable(tree_t mytree, FILE *fp, char *current_word) {
    if (mytree->is_word) {
        fprintf(fp, "%s\n", current_word);
        nbre_mot_manquant_tree++;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (mytree->sons[i] != NULL) {
            current_word[strlen(current_word)] = (unsigned char)i;
            current_word[strlen(current_word) + 1] = '\0';
            tree_fprintf_readable(mytree->sons[i], fp, current_word);
            current_word[strlen(current_word) - 1] = '\0'; // Remettre le mot dans l'état précédent
        }
    }
}

int tree_printf_readable(tree_t mytree) {
    char current_word[WORD_MAX_LENGTH] = "";
    tree_fprintf_readable(mytree, stdout, current_word);
    return nbre_mot_manquant_tree;
}
// Affiche le dictionnaire a l'ecran, dasn un fichier.
void tree_fprintf(tree_t mytree, FILE* fp){
    if(mytree->is_word){
        printf("%p", mytree);
        return;
    }
    printf("( fils de %p: \n", mytree);
    for (int i= 0 ; i<255; i++){
        if(mytree->sons[i]!=NULL){
            fprintf(fp, "%c\n",(unsigned char)i);
            tree_fprintf(mytree->sons[i], fp);
            printf(")\n");
        }
    }
}

void tree_printf(tree_t mytree){
    tree_fprintf(mytree, stdout);
}

// Fonction pour supprimer un arbre
tree_t tree_delete(tree_t mytree) {
    if (mytree == NULL) {
        return NULL;
    }
    if (mytree->ref_count > 0) {
        mytree->ref_count--;
        return mytree;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (mytree->sons[i] != NULL) {
            tree_delete(mytree->sons[i]);
        }
    }
    if (compression_tree) add_memoire_economisee(sizeof(mytree));
    free(mytree);
    return NULL;
}

// Fonction pour avancer dans l'arbre en ignorant la casse
int anti_case(tree_t *mytree, unsigned char word) {
    unsigned char word_lower = tolower(word);
    unsigned char word_upper = toupper(word);

    if ((*mytree)->sons[word_lower] == NULL && (*mytree)->sons[word_upper] == NULL) {
        return 0;
    }

    *mytree = (*mytree)->sons[word_lower] != NULL ? (*mytree)->sons[word_lower] : (*mytree)->sons[word_upper];

    return 1;
}

// Fonction pour rechercher un mot dans l'arbre
int *tree_search(tree_t mytree, char *word) {
    tree_t current = mytree;
    int i = 0;
    while (word[i] != '\0') {
        if (!anti_case(&current, (unsigned char)word[i++])) {
            return NULL;
        }
    }

    int *terminado = calloc(2, sizeof(int));
    if (current->sons[' '] != NULL) {
        terminado[1] = 1;
    }
    terminado[0] = current->is_word;
    return terminado;
}

// Fonction pour ajouter un mot à l'arbre
int tree_add(tree_t mytree, char *word) {
    tree_t current = mytree;

    for (int i = 0; word[i] != '\0'; i++) {
        unsigned char current_char = tolower(word[i]);

        if (current->sons[current_char] == NULL) {
            current->sons[current_char] = tree_new();
        }
        current = current->sons[current_char];
    }

    current->is_word = 1;

    return 1;
}

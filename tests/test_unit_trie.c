#include "trie_dict.h"

int main() {

    // Création d'un arbre lexicographique vide
    tree_t mytree = tree_new();
    printf("/////////////////////////////////////////\n");

    // Ajout de mots à l'arbre
    tree_add(mytree, "apple");
    tree_add(mytree, "banana");
    tree_add(mytree, "etrange");
    tree_add(mytree, "app");
    tree_add(mytree, "bat");
    tree_add(mytree, "batman");

    // Affichage du trie
    printf("Trie détaillé:\n");
    tree_printf(mytree);
    // Affichage lisible du trie
    printf("\nTrie lisible :\n");
    tree_printf_readable(mytree);
    // Recherche de mots dans l'arbre
    printf("\nRecherche de mots :\n");
    printf("apple : %s\n", tree_search(mytree, "apple") ? "trouvé" : "non trouvé");
    printf("orange : %s\n", tree_search(mytree, "orange") ? "trouvé" : "non trouvé");
    printf("grape : %s\n", tree_search(mytree, "grape") ? "trouvé" : "non trouvé");

    // Suppression du trie
    mytree = tree_delete(mytree);
    printf("/////////////////////////////////////////\n");

    printf("Essayons nous à la compression :\n");
     // Create a sample dictionary tree
    mytree = tree_new();

    // Ajout de mots à l'arbre
    tree_add(mytree, "apple");
    tree_add(mytree, "banana");
    tree_add(mytree, "etrange");
    tree_add(mytree, "arange");
    tree_add(mytree, "app");
    tree_add(mytree, "bat");
    tree_add(mytree, "batman");

    // Affichage du trie
    printf("Trie :\n");
    tree_printf_readable(mytree);

    mytree = compress_tree(mytree);
    printf("Trie compressé\n");
    tree_printf(mytree);

    // // Recherche de mots dans l'arbre
    printf("\nRecherche de mots :\n");
    printf("apple : %s\n", tree_search(mytree, "apple") ? "trouvé" : "non trouvé");
    printf("orange : %s\n", tree_search(mytree, "orange") ? "trouvé" : "non trouvé");
    printf("grape : %s\n", tree_search(mytree, "grape") ? "trouvé" : "non trouvé");

    // Affichage lisible du trie
    printf("\nTrie lisible :\n");
    tree_printf_readable(mytree);
    printf("/////////////////////////////////////////\n");
    // Suppression du trie
    mytree = tree_delete(mytree);

    return 0;
}

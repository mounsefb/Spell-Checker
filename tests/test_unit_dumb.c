#include "dumb_dict.h"

int main() {
    // Testons les fonctions du dictionnaire

    // Création d'un dictionnaire vide
    dict_t my_dict = dict_new();


    // Ajout de mots dans le dictionnaire
    dict_add(my_dict, "hello");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "H");
    // dict_printf_readable(my_dict);
    // puts("");
     dict_add(my_dict, "hella");
    // dict_printf_readable(my_dict);
    // puts("");

    dict_add(my_dict, "hi");
    // dict_printf_readable(my_dict);
    // puts("");

    dict_add(my_dict, "hella");
    // dict_printf_readable(my_dict);
    // puts("");

    dict_add(my_dict, "world");
    // dict_printf_readable(my_dict);
    // puts("");

    dict_add(my_dict, "helalaboys");
    // dict_printf_readable(my_dict);
    // puts("");

    dict_add(my_dict, "apple");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "appli");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "applo");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "applp");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "applm");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "applr");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "applt");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "appld");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "appla");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "applq");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "applx");
    // dict_printf_readable(my_dict);
    // puts("");
    dict_add(my_dict, "applc");
    // dict_printf_readable(my_dict);
    // puts("");

    // Affichage du dictionnaire
    printf("Arbre lexicogrpahique détaillé\n");
    dict_printf(my_dict);

    printf("Arbre lexicogrpahique lisible\n");
    dict_printf_readable(my_dict);

    // Recherche de mots dans le dictionnaire
    if (dict_search(my_dict, "hello")) {
        printf("Le mot 'hello' est présent dans le dictionnaire.\n");
    } else {
        printf("Le mot 'hello' n'est pas présent dans le dictionnaire.\n");
    }

    if (dict_search(my_dict, "hella")) {
        printf("Le mot 'hella' est présent dans le dictionnaire.\n");
    } else {
        printf("Le mot 'hella' n'est pas présent dans le dictionnaire.\n");
    }

    if (dict_search(my_dict, "python")) {
        printf("Le mot 'python' est présent dans le dictionnaire.\n");
    } else {
        printf("Le mot 'python' n'est pas présent dans le dictionnaire.\n");
    }

    if (dict_search(my_dict, "apple")) {
        printf("Le mot 'apple' est présent dans le dictionnaire.\n");
    } else {
        printf("Le mot 'apple' n'est pas présent dans le dictionnaire.\n");
    }

    // Libération des ressources du dictionnaire
    // (vous devez implémenter la fonction dict_delete)
    my_dict = dict_delete(my_dict);

printf("/////////////////////////////////////////\n");

    printf("Essayons nous à la compression :\n");

    dict_t root = dict_new();
    dict_add(root, "hello");
    dict_add(root, "pello");
    dict_add(root, "pi");
    dict_add(root, "d");


    dict_add(root, "world");
    dict_add(root, "hi");

    printf("Arbre lexicographique lisible :\n");
    dict_printf_readable(root);
    root = compress_dict(root);

    printf("\nL'abre compréssé:\n");
    dict_printf(root);
    
    dict_delete(root);

    return 0;
}
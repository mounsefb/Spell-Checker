#include "hashsetext.h"
#include "double.h"
#include "utilitaire_commun.h"


int main() {
    int table_size = 10; // Taille de la table de hachage
    hashlset_t table = hashlset_new(table_size, hash_double, double_fprintf, double_delete, double_equal);

    double keys[] = {3.14, 2.71, 1.618, 42.0, 0.0}; // Exemple de clés en double
    int num_keys = sizeof(keys) / sizeof(keys[0]);

    double* key;
    printf("Construction de la table en cours ...\n");
    // Ajout des clés à la table de hachage
    for (int i = 0; i < num_keys; i++) {
        key = (double*)malloc(sizeof(double));
        *key = keys[i];
        hashlset_put(key, table);
    }
    hashlset_fprintf(table, stdout);
    puts("");

    // Recherche des clés dans la table
    double search_key = 2.71;
    int * res;
    res = hashlset_find_key(&search_key, table);
    if (res!=NULL && res[0]) {
        printf("Clé %.2lf trouvée dans la table.\n", search_key);
        free(res);
    } else {
        printf("Clé %.2lf non trouvée dans la table.\n", search_key);
    }

    search_key = 2.51;
    res = hashlset_find_key(&search_key, table);
    if (res!=NULL && res[0]) {
        printf("Clé %.2lf trouvée dans la table.\n", search_key);
        free(res);
    } else {
        printf("Clé %.2lf non trouvée dans la table.\n", search_key);
    }
    
    // Suppression d'une clé de la table
    double remove_key = 1.618;
    if (hashlset_remove_key(&remove_key, table)) {
        printf("Clé %.3lf supprimée de la table.\n", remove_key);
    } else {
        printf("Clé %.3lf non trouvée dans la table, aucune suppression effectuée.\n", remove_key);
    }
    hashlset_fprintf(table, stdout);

    remove_key = 1.638;
    if (hashlset_remove_key(&remove_key, table)) {
        printf("Clé %.3lf supprimée de la table.\n", remove_key);
    } else {
        printf("Clé %.3lf non trouvée dans la table, aucune suppression effectuée.\n", remove_key);
    }
    hashlset_fprintf(table, stdout);
    // free(key);
    // Nettoyage de la table et libération de la mémoire
    hashlset_delete(table);
}
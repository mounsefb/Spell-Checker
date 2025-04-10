#include "hashsetext.h"
#include "mystring.h"
#include "utilitaire_commun.h"


int main() {
    int table_size = 10; // Taille de la table de hachage
    hashlset_t table = hashlset_new(table_size, hash_char, char_fprintf, char_delete, char_equal);

    char *keys[] = {char_new("bonjour"), char_new("salut"), char_new("coucou"), char_new("hey"), char_new("hi")}; // Exemple de clés en double
    int num_keys = sizeof(keys) / sizeof(keys[0]);

    // Ajout des clés à la table de hachage
    printf("Construction de la table en cours ...\n");
    for(int j=0;j<3; j++){
        for (int i = 0; i < num_keys; i++) {
            char* key =char_new(keys[i]);
            hashlset_put(key, table);
        }
    }
    hashlset_fprintf(table, stdout);
    puts("");

    // Recherche des clés dans la table
    char * search_key = "bonjour";
    int * res;
    res = hashlset_find_key(&search_key, table);
    if (res!=NULL && res[0]) {
        printf("Clé %s trouvée dans la table.\n", search_key);
        free(res);
    } else {
        printf("Clé %s non trouvée dans la table.\n", search_key);
    }

    search_key = "boujou";
    res = hashlset_find_key(&search_key, table);
    if (res!=NULL && res[0]) {
        printf("Clé %s trouvée dans la table.\n", search_key);
        free(res);
    } else {
        printf("Clé %s non trouvée dans la table.\n", search_key);
    }

    // Suppression d'une clé de la table
    char * remove_key ="salut";
    if (hashlset_remove_key(remove_key, table)) {
        printf("Clé %s supprimée de la table.\n", remove_key);
        hashlset_fprintf(table, stdout);
    } else {
        printf("Clé %s non trouvée dans la table, aucune suppression effectuée.\n", remove_key);
    }
    
    remove_key = "saloute";
    if (hashlset_remove_key(remove_key, table)) {
        printf("Clé %s supprimée de la table.\n", remove_key);
         hashlset_fprintf(table, stdout);
    } else {
        printf("Clé %s non trouvée dans la table, aucune suppression effectuée.\n", remove_key);
    }
   
    for (int i = 0; i < num_keys; i++){
        free(keys[i]);
    }
    
    // Nettoyage de la table et libération de la mémoire
    hashlset_delete(table);

    return 0;
}
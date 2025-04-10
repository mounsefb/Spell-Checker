#include "hashsetext.h"
#include "utilitaire_commun.h"

// Fonction pour créer un nouveau hashset avec liste chaînée
hashlset_t hashlset_new(int n, unsigned int (*fhachage)(void * ),
          void (*print_key)(void*,FILE*),
          int (*delete_key)(void*),
          int (*compare_key)(void* e1, void* e2) ){
    hashlset_t h = NULL;
    add_empreinte_memoire(sizeof(*h));
    h = calloc (1, sizeof(*h));
    assert(h!=NULL);
    h->total_number=0;
    h->size = n;
    h->hachage = fhachage;
    add_empreinte_memoire(n*sizeof(*h->data));
    h->data = calloc(n, sizeof(*h->data));
    assert(h->data!=NULL);
    h->delete_key = delete_key;
    h->compare_key = compare_key;
    h->print_key = print_key;
    return h;
}

// Fonction pour libérer la mémoire occupée par le hashset
hashlset_t hashlset_delete(hashlset_t table){
    if (table==NULL || table->data==NULL){
        return NULL;
    }
    for (unsigned int i=0; i<table->size; i++){
        while (table->data[i]!=NULL){
            table->total_number--;
            link_t l1=table->data[i]->next;
            add_empreinte_memoire(-table->delete_key(table->data[i]->data));
            add_empreinte_memoire(-(int)sizeof(table->data[i]));
            free(table->data[i]);
            
            table->data[i]=l1;
        }
    }
    table->total_number=0;
    add_empreinte_memoire(-(int)sizeof(table->data));
    free(table->data);
    add_empreinte_memoire(-(int)sizeof(table));
    free(table);
    return NULL;
}

// Fonction pour ajouter une clé au hashset
int hashlset_put(void* key, hashlset_t table){
    int h = table->hachage(key);
    int slot = h % table->size;
    link_t slink = table->data[slot];
    while (slink!=NULL){
        if (table->compare_key(slink->data, key)==1){
            add_empreinte_memoire(-table->delete_key(key));
            return 1;
        }
        slink=slink->next;
    }
    link_t insert;
    add_empreinte_memoire((int)sizeof(*insert));
    insert = calloc(1, sizeof(*insert));
    assert(insert!=NULL);
    insert->data=key;
    insert->next = table->data[slot];
    table->data[slot] = insert ;
    table->total_number++;
    return 1;
}

// Fonction pour trouver une clé dans le hashset
int* hashlset_find_key(void* key, hashlset_t table){
    if (table == NULL || table->data == NULL){
        return NULL;
    }
    int h = table->hachage(key);
    int slot = h % table->size;
    link_t slink = table->data[slot];
    int *res = calloc(2, sizeof(int));
    int res_comp;
    while (slink != NULL){
        res_comp = table->compare_key(slink->data, key);
        // Si la clé est trouvée, met à jour le résultat en conséquence
        if (res_comp == 1){
            res[0] = 1;
        }
        else if (res_comp == 2){
            res[1] = 1;
        }
        slink = slink->next;
    }
    // Si la clé n'est pas trouvée, libère la mémoire du résultat et retourne NULL
    if (res[0] == 0 && res[1] == 0) {
        free(res);
        return NULL;
    }
    return res;
}

// Fonction pour supprimer une clé du hashset
int hashlset_remove_key(void* key, hashlset_t table){
    if (table==NULL || table->data==NULL){
        return 0;
    }
    int h = table->hachage(key);
    int slot = h % table->size;
    if (table->data[slot]==NULL){
        return 0;
    }
    link_t slink = table->data[slot];
    if (slink->next == NULL) {
        // Si la clé est trouvée dans le premier élément de la liste chaînée, le supprime
        if (table->compare_key(key, slink->data)==1){
            add_empreinte_memoire(-table->delete_key(slink->data));
            add_empreinte_memoire(-(int)sizeof(table->data[slot]));
            free(table->data[slot]);
            table->data[slot]=NULL;
            table->total_number--;
        }
        return 1;
    }
    link_t prev = slink;
    link_t current = slink->next;
    // Parcourt la liste chaînée jusqu'à trouver la clé ou atteindre la fin de la liste
    while (current->next != NULL && table->compare_key(key, current->data)!=1) {
            prev = current;
            current = current->next;
    }
    // Si la clé est trouvée, supprime le nœud de la liste chaînée
    if (table->compare_key(key, current->data)){
        prev->next = current->next;
        add_empreinte_memoire(-table->delete_key(current->data));
        add_empreinte_memoire(-(int)sizeof(current));
        free(current);
        table->total_number--;
    }
    return 1;
}

void hashlset_fprintf(hashlset_t table, FILE*fp){
    printf("Table de taille : %d\n", table->size);
    printf("Nombre d'élement actuellement : %d\n", table->total_number);
    for (unsigned int i =0 ; i<table->size; i++){
        printf(" %d eme ligne : ", i);
        link_t slink=table->data[i];
        while (slink!=NULL){
            printf(" (");
            table->print_key(slink->data, fp);
            printf(") ");
            slink=slink->next;
        }
        puts("");
    }
}

// Fonction pour imprimer le hashset de manière lisible
void hashlset_fprintf_readable(hashlset_t table, FILE*fp){
    for (unsigned int i =0 ; i<table->size; i++){
        link_t slink=table->data[i];
        while (slink!=NULL){
            table->print_key(slink->data, fp);
            puts("");
            slink=slink->next;
        }
    }
}

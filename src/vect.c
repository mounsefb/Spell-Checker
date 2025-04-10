#include "vect.h"

// Fonction pour créer un nouveau vecteur
vect_t vect_new(unsigned int n,void (*print_data)(void*,FILE*), 
int (*delete_data)(void*),int (*equal_data)(void*,void*), 
vect_t (*append_data)(void*,vect_t), int (*compare_data)(void*,void*)){
    vect_t tab;
    if((tab = calloc(1, sizeof(*tab)))==NULL) exit(EXIT_FAILURE);
    if((tab->data=calloc(n,sizeof(*tab->data)))==NULL) exit(EXIT_FAILURE);
    tab->max_size = n;
    tab->actual_size =0;
    tab->fprint_data=print_data;
    tab->delete_data=delete_data;
    tab->equal_data=equal_data;
    tab->append_data=append_data;
    tab->compare_data=compare_data;
    return tab;
}

// Fonction pour ajouter un élément au vecteur
vect_t vect_append(void* val, vect_t table){
    return table->append_data(val, table);
}

// Fonction pour supprimer les n derniers éléments du vecteur
vect_t vect_remove_nlast(unsigned int n, vect_t table) {
    if (n > 0 && n <= table->actual_size) {
        for (unsigned int i=table->actual_size - n; i<table->actual_size; i++){
                table->delete_data(table->data[i]);
        }
        if (table->actual_size - n < table->max_size / 3) {
            void *new_data = realloc(table->data, (table->max_size / 2) * sizeof(*table->data));
            if (new_data == NULL) {
                exit(EXIT_FAILURE);
            }

            table->data = new_data;
            table->max_size = table->max_size / 2;
        }
        table->actual_size = table->actual_size - n;
    }
    return table;
}


// Fonction pour afficher le vecteur
void vect_print(vect_t ptab, FILE * file){
    if (ptab==NULL) return;
    if (ptab->fprint_data){
        for (unsigned int i=0; i<ptab->actual_size; i++) ptab->fprint_data(ptab->data[i], file);
    }
    puts("");
}

// Fonction pour supprimer le vecteur
vect_t vect_delete(vect_t table){
    if (table==NULL) return NULL;
    if (table->delete_data){
        for (unsigned int i=0; i<table->actual_size; i++){
            // printf("%d",i);
            table->delete_data(table->data[i]);
        }
    }
    free(table->data);
    free(table);
    return NULL;
}

// Fonction pour ajouter un double au vecteur
vect_t double_append(void* val, vect_t table){
    if (table->actual_size < table->max_size) {
        table->data[table->actual_size] = (double *)val;
        table->actual_size++;
        return table;
    }

    void *new_data;
    if ((new_data = realloc(table->data, 2 * table->max_size * sizeof(double))) == NULL) {
        exit(EXIT_FAILURE);
    }

    table->data=new_data;
    table->data[table->actual_size] = (double *)val;
    table->actual_size++;
    table->max_size = table->max_size * 2;
    return table;
}

// Fonction pour ajouter un char* au vecteur
vect_t char_append(void* val, vect_t table){
     if (table->actual_size < table->max_size) {
        table->data[table->actual_size] = (char *)(val);
        table->actual_size++;
        return table;
    }

    printf("realloc\n");

    void **new_data;
    if ((new_data = realloc(table->data, 2 * table->max_size * sizeof(char*))) == NULL) {
        exit(EXIT_FAILURE);
    }

    table->data=new_data;
    table->data[table->actual_size] = (char *)val;
    table->actual_size++;
    table->max_size = table->max_size * 2;
    return table;
}
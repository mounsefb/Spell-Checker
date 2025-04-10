#include "list.h"


// Fonction pour créer une nouvelle liste chaînée
list_t list_new(void (*fprint_data)(void*,FILE*),int (*delete_data)(void*), int (*equal_data)(void*,void*), void *data_new(void *)){
    list_t l;
    l=calloc(1, sizeof(*l));
    if (l==NULL) return NULL;
    l->data=NULL;
    l->size=0;
    l->delete_data=delete_data;
    l->fprint_data=fprint_data;
    l->equal_data=equal_data;
    l->data_new= data_new;
    return l;
}

// Fonction pour vérifier si la liste est vide
int list_is_empty(list_t l){
    if(l==NULL){
        return 1;
    }
    return link_is_empty(l->data);
}

// Fonction pour vérifier si un maillon est vide
int link_is_empty(link_t l){
    return l==NULL;
}

// Fonction pour récupérer le premier élément de la liste
void * list_first(list_t l){
    return l->data->data;
}

// Fonction pour ajouter un maillon en tête de liste
link_t link_add_first(void* e, link_t l){
    link_t pt;
    pt=calloc(1, sizeof(*pt));
    if(pt==NULL) return NULL;
    pt->data=e;
    pt->next=l;
    return pt;
}

// Fonction pour ajouter un élément en tête de liste
list_t list_add_first(void* e, list_t l){
    l->data=link_add_first(e, l->data);
    l->size++;
    return l;
}

// Fonction pour supprimer le premier maillon de la liste
link_t link_del_first(link_t l , int (*delete_data)(void*)){
    if(l==NULL) return NULL;
    link_t l1=l->next;
    if(l->data!=NULL){
        delete_data(l->data);
        l->data=NULL;
    }
    free(l);
    return l1;
}

// Fonction pour supprimer le premier élément de la liste
list_t list_del_first(list_t l ){
    if(l->data==NULL){
        return l;
    }
    l->data=link_del_first(l->data, l->delete_data);
    l->size--;
    return l;
}

// Fonction pour obtenir la longueur de la liste
size_t list_length(list_t l){
    return l->size;
}

// Fonction pour imprimer la liste dans un fichier
void list_fprintf(list_t l, FILE* fp){
    if(!list_is_empty(l)){
        link_t link;
        int i=0;
        for (link=l->data; !link_is_empty(link); link=link->next){
            l->fprint_data(link->data, fp);
            i++;
        }
        puts("");
    }
}

// Fonction pour imprimer la liste sur l'ecran
void list_printf(list_t l){
    list_fprintf(l, stdout);
}

// Fonction pour supprimer la liste
list_t list_delete(list_t l){
    if (l->delete_data){
        while(l->data!=NULL){
            l=list_del_first(l);
        }
    }
    free(l);
    return l;
}

// Fonction pour ajouter un maillon en fin de liste
link_t link_add_last(void* e, link_t l){
    link_t link = l;

    if ( link_is_empty(link) ) return link_add_first( e, link );

    for (link=l; !link_is_empty(link->next); link=link->next);

    link_t link_new = calloc(1, sizeof(*link_new));
    assert( link_new );

    link_new->data=e;
    link_new->next=NULL;
    link->next=link_new;
    return l;
}

// Fonction pour ajouter un élément en fin de liste
list_t list_add_last(void* e, list_t l){
    l->data=link_add_last(e, l->data);
    l->size++;
    return l;
}

// Fonction pour supprimer le dernier maillon de la liste
link_t link_del_last(link_t l, int (*delete_data)(void*)) {
    if (l == NULL) {
        return NULL;
    }
    if (l->next == NULL) {
        delete_data(l->data);
        l->data = NULL;
        free(l);
        return NULL;
    }
    link_t prev = l;
    link_t current = l->next;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    prev->next = NULL; 
    delete_data(current->data);
    current->data = NULL;
    free(current);

    return l;
}


// Fonction pour supprimer le dernier élément de la liste
list_t list_del_last(list_t l ){
    if(l->data==NULL){
        return l;
    }
    l->data=link_del_last(l->data, l->delete_data);
    l->size--;
    return l;
}

// Fonction pour rechercher un élément dans la liste en utilisant une fonction de comparaison
ink_t list_lookup(void* elem, list_t l, int(*compare)(void* e1, void* e2)){
    link_t link;
    for (link=l->data; !link_is_empty(link); link=link->next){
        if (compare(elem, link->data)){
            return link;
        }
    }
    return NULL;
}

// Fonction pour trouver un élément dans la liste
link_t list_find(void* e, list_t l){
     if (l->equal_data){
        return list_lookup(e, l, l->equal_data);
    }
    link_t link;
    for (link=l->data; !link_is_empty(link); link=link->next){
        if (e==link->data){
            return link;
        }
    }
    return NULL;
}

// Fonction pour supprimer l'élément à une position donnée dans la liste
list_t list_remove_at(unsigned int pos, list_t l){
    if (pos<l->size){
        link_t link=l->data;
        for (unsigned int i=0; i!=pos; i++){
            link=link->next;
        }
        return list_remove(link->data, l);
    }
    return l;
}

// Fonction pour supprimer un élément de la liste
list_t list_remove(void* param, list_t l){
    if (l->equal_data){
        if (l->data == NULL) {
            return NULL;    
        }
        if (l->data->next == NULL) {
            if (l->equal_data(param, l->data->data)){
                l->delete_data(l->data->data);
                l->data->data = NULL;
                free(l->data);
                l->data=NULL;
                l->size--;
            }
            else{
                printf("l'element cherché n'existe pas");
            }
            return l;
        }
        link_t prev = l->data;
        link_t current = l->data->next;
        while (current->next != NULL && l->equal_data(param, current->data)!=1) {
            prev = current;
            current = current->next;
        }
        if (l->equal_data(param, current->data)){
            prev->next = current->next;
            l->delete_data(current->data);
            current->data = NULL;
            free(current);
            l->size--;
        }
        else{
            printf("l'element cherché n'existe pas\n");
        }
        return l;
    }
    printf("pas de fonciton equal fournie en entrée pour effectuer la recherche\n");
    return l;
}

// Fonction pour visiter tous les éléments de la liste
void list_simple_visit(list_t l, void (*exec_on_value)(void*,void*), void* param){
    link_t link;
    for (link=l->data; link!=NULL; link=link->next){
        exec_on_value(link->data, param);
    }
}

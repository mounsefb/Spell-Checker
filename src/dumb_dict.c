#include "dumb_dict.h"
#include <ctype.h>
#include "utilitaire_commun.h"

int nbre_mot_manquant = 0;
int compression = 0; 

// Fonction pour compter les mots dans le dictionnaire
void compteur_mot(dict_t dico){
    if(dico->data == '\0'){
        nbre_mot_manquant++;
        return;
    }
    list_t current = dico->sons;
    while(current!=NULL){
        compteur_mot(current->data);
        current=current->next;
    }
}

// Fonction pour obtenir le nombre de mots manquants dans le dictionnaire
int get_nbre_manquant(dict_t dico){
    nbre_mot_manquant = 0;
    compteur_mot(dico);
    return nbre_mot_manquant;     
}



// Fonction pour compresser un nœud et ses fils par partage de suffixe
dict_t compress_node_dict(dict_t node, visited_node_t** visited_nodes) {
    if (node == NULL) {
        return NULL;
    }
    visited_node_t* current_visited = *visited_nodes;
    while (current_visited != NULL) {
        if (dict_equal(node, current_visited->original)) {
            // Noeud déjà rencontré, retourne le noeud compressé correspondant
            compression = 1;
            dict_delete(node);
            compression = 0;
            current_visited->original->ref_count++;
            return current_visited->original;
        }
        current_visited = current_visited->next;
    }
        
    // Noeud pas encore rencontré, on l'ajoute à la liste des nœuds rencontrés
    visited_node_t* new_visited_node = (visited_node_t*)malloc(sizeof(visited_node_t));
    new_visited_node->original = node;
    new_visited_node->next = *visited_nodes;
    *visited_nodes = new_visited_node;

    // Compress les fils du noeud actuel
   list_t current_son = node->sons;
    while (current_son != NULL) {
        current_son->data = compress_node_dict(current_son->data, visited_nodes);
        current_son = current_son->next;
    }

    // Retourner le noeud compressé
    return new_visited_node->original;
}


// Fonction principale pour compresser l'arbre par partage de suffixe
dict_t compress_dict(dict_t root) {
    visited_node_t* visited_nodes = NULL; // Liste des noeuds déjà rencontrés

    dict_t res = compress_node_dict(root, &visited_nodes);

    visited_node_t *current_visited = visited_nodes;
    while (current_visited != NULL) {
        visited_node_t * next_visited = current_visited->next;
        free(current_visited);
        current_visited = next_visited;
    }

    return res;
}

// Creation d'une liste vide, avec les pointeurs de fonctions utiles
dict_t dict_new(){
    dict_t new_dict;
    add_empreinte_memoire(sizeof(*new_dict));
    new_dict = calloc(1, sizeof(*new_dict));
    new_dict->data='\0';
    new_dict->sons=NULL;
    return new_dict;
}

// Fonction pour comparer deux arbres (egalite)
int dict_equal(dict_t tree1, dict_t tree2) {
    if (tree1 == NULL && tree2 == NULL) {
        return 1;
    } else if (tree1 == NULL || tree2 == NULL) {
        return 0;
    }

    // Compare la data des arbres
    if (tree1->data != tree2->data) {
        return 0;
    }
    // Compare les sous-arbres par récursivité
    list_t sons1 = tree1->sons;
    list_t sons2 = tree2->sons;

    while (sons1 != NULL && sons2 != NULL) {
        if (!dict_equal(sons1->data, sons2->data)) {
            return 0;
        }
        sons1 = sons1->next;
        sons2 = sons2->next;
    }

    return (sons1 == NULL && sons2 == NULL);
}

// Affiche le dictionnaire a l'ecran, dasn un fichier.
void dict_fprintf(dict_t dico, FILE* fp){
    if(dico->data == '\0'){
        printf("%p", &dico->data);
        return;
    }
    list_t current = dico->sons;
    printf("( fils de %c d'adresse %p: \n", dico->data, &dico->data);
    while(current!=NULL){
        fprintf(fp, "%c\n",current->data->data);
        dict_fprintf(current->data, fp);
        printf(")\n");
        current=current->next;
    }

}

// Affiche le dictionnaire a l'ecran
void dict_printf(dict_t dico){
    dict_fprintf(dico, stdout);
}

// Affiche le dictionnaire a l'ecran, dasn un fichier.
void dict_fprintf_readable(dict_t dico, FILE* fp, char* current_word){
    if(dico->data == '\0'){
        fprintf(fp, "%s\n", current_word);
        return;
    }
    list_t current = dico->sons;

    // Ajouter le caractère du nœud actuel au mot courant
    int len = strlen(current_word);
    current_word[len] = dico->data;
    current_word[len + 1] = '\0';

    while(current!=NULL){
        dict_fprintf_readable(current->data, fp, current_word);
        current_word[len+1] = '\0';
        current=current->next;
    }
}

// Fonction wrapper pour afficher le dictionnaire à l'écran
void dict_printf_readable(dict_t dico){
    // Appeler la fonction récursive avec un mot initial vide
    char current_word[100] = "";  // Choisir une taille suffisamment grande
    dict_fprintf_readable(dico, stdout, current_word);
}


// Libère toute le dictionnaire et retourne une dicte vide
// Libere les elements avec delete_data si la fonction existe
dict_t dict_delete(dict_t dico) {
    if (dico == NULL) {
        return NULL;
    }
    // Si le nœud est partagé, décrémentez le compteur de références
    if (dico->ref_count > 0) {
        dico->ref_count--;
        return dico;
    }
    list_t current = dico->sons;
    while (current != NULL) {
        list_t next = current->next;
        current->data = dict_delete(current->data);
        if(compression == 1) add_memoire_economisee(sizeof(current));
        free(current);
        current = next;
    }
    dico->sons=NULL;
    if(compression == 1) add_memoire_economisee(sizeof(dico));
    free(dico);
    return NULL;
}

int *dict_search(dict_t mydic, char* word){
    // Cas dico vide
    if(mydic->data == '\0'){
        return 0;
    }
   
    list_t current = mydic->sons;
    int i = 0;
    int trouveeee = 0;
    while(word[i]!='\0'){
        while(current!=NULL && trouveeee == 0){
            if(tolower(current->data->data)==tolower(word[i])){
                trouveeee = 1;
            }
            else current=current->next;
        }
        if(trouveeee==0){
            return NULL;
        }
        current = current->data->sons;
        trouveeee = 0;
        i++;
    }
    int * terminado = calloc(2, sizeof(int));
    while(current!=NULL){
        if(current->data->data == '\0'){
            terminado[0]=1;
        }
        if(current->data->data == ' '){
            terminado[1]=1;
        }
        current=current->next;
    }
    return terminado;
}

// Fonction pour ajouter un mot au dictionnaire
int dict_add(dict_t mydic, char* word){
    // Cas où le dictionnaire est vide
    if (mydic->data == '\0') {
        mydic->data = ' ';
        add_empreinte_memoire(sizeof(*mydic->sons));
        mydic->sons = calloc(1, sizeof(*mydic->sons));
        if (mydic->sons == NULL) {
            printf("alloc dyn echouee\n");
            return 0;
        }

        list_t current = mydic->sons;
        int i = 0;

        // Parcourt chaque caractère du mot à ajouter
        while (word[i] != '\0') {
            add_empreinte_memoire(sizeof(*current->data));
            current->data = calloc(1, sizeof(*current->data));
            if (current->data == NULL) {
                printf("alloc dyn echouee\n");
                return 0;
            }
            // Initialise le nœud avec le caractère du mot
            current->data->data = word[i];
            current->next = NULL;
            add_empreinte_memoire(sizeof(*mydic->sons));
            current->data->sons = calloc(1, sizeof(*mydic->sons));
            current = current->data->sons;
            i++;
        } 
        add_empreinte_memoire(sizeof(*current->data));
        current->data = calloc(1, sizeof(*current->data));
        if (current->data == NULL) {
            printf("alloc dyn echouee\n");
            return 0;
        }
        // Initialise le dernier nœud avec le dernier caractère du mot
        current->data->data = word[i];
        current->next = NULL;
        return 1;
    }

    // Le dictionnaire n'est pas vide, recherche l'emplacement pour ajouter le mot
    list_t current = mydic->sons;
    int i = 0;
    while(word[i]!='\0'){
        // Parcourt les nœuds fils du nœud courant jusqu'à trouver le caractère correspondant
        while(current->next!=NULL && tolower(current->data->data)!=tolower(word[i])){
            current=current->next;
        }
        // Si le caractère du mot n'est pas présent dans les fils du nœud courant, l'ajoute
        if(current->next==NULL && tolower(current->data->data)!=tolower(word[i])){
            add_empreinte_memoire(sizeof(*current->next)); 
            current->next = calloc(1, sizeof(*current->next));
            if (current->next == NULL) {
                printf("alloc dyn echouee\n");
                return 0;
            }
            current = current->next;
            add_empreinte_memoire(sizeof(*current->data));  
            current->data = calloc(1, sizeof(*current->data));
            if(current->data==NULL){
                printf("alloc dyn echouee\n");
                return 0;
            }
            current->data->data = word[i];
        }
        i++;

        if(current->data->sons==NULL){
            break;
        }
        current = current->data->sons;
        
    }
    
    // Si le mot à ajouter a des caractères restants, les ajoute comme fils du dernier nœud
    if(current->data->sons==NULL){
        current->data->sons = calloc(1, sizeof(*mydic->sons));
        current = current->data->sons;
        while (word[i] != '\0') {
            add_empreinte_memoire(sizeof(*current->data));
            current->data = calloc(1, sizeof(*current->data));
            if (current->data == NULL) {
                printf("alloc dyn echouee\n");
                return 0;
            }

            current->data->data = word[i];
            current->next = NULL;
            add_empreinte_memoire(sizeof(*mydic->sons));
            current->data->sons = calloc(1, sizeof(*mydic->sons));
            current = current->data->sons;
            i++;
        }
    }

    // Si le dernier nœud n'a pas de fils, ajoute le dernier caractère du mot
    if(current->data == NULL){
        add_empreinte_memoire(sizeof(*current->data));
        current->data = calloc(1, sizeof(*current->data));
        if (current->data == NULL) {
            printf("alloc dyn echouee\n");
            return 0;
        }
        current->data->data = word[i];

        current->next = NULL;
    }
    else{
        while(current->next!=NULL && tolower(current->data->data)!=tolower(word[i])){
            current=current->next;
        }
        if(current->next==NULL && tolower(current->data->data)!=tolower(word[i])){
            add_empreinte_memoire(sizeof(*current->next));
            current->next = calloc(1, sizeof(*current->next));
            if (current->next == NULL) {
                printf("alloc dyn echouee\n");
                return 0;
            }
            current = current->next;
            add_empreinte_memoire(sizeof(*current->data));
            current->data = calloc(1, sizeof(*current->data));
            if(current->data==NULL){
                printf("alloc dyn echouee\n");
                return 0;
            }
            current->data->data = word[i];
            current->next=NULL;
        }
    }
    return 1;
}
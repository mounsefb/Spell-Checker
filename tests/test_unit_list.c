#include "double.h"
#include "mystring.h"
#include "list.h"


#define NBTESTS 100
#define NBCAS 5

// Programme de tests aleatoire
// 3 fois plus d'ajout que de suppression
int main() {
    list_t l=list_new(double_fprintf,double_delete, double_equal, double_new_ptr);
    int j;
    srandom(time(NULL));
    list_printf(NULL);
    for (j=0; j<NBTESTS; j++) {
        switch(random() % NBCAS) {
        case 0:
            double *x=calloc(1,sizeof(double));
            *x = random() % 100;
            printf("Ajout début de liste %lf :\n ",*x);
            l=list_add_first(x,l);
            break;
        case 1:
            double *y=calloc(1,sizeof(double));
            *y = random() % 100;
            printf("Ajout fin de liste %lf :\n ",*y);
            l=list_add_last(y,l);
            break;
        case 2 : 
            printf("Suppression premier élement\n");
            l=list_del_first(l);
            break;
        case 3:
            if(l->data!=NULL){
                int n=random()%l->size;
                printf("Recherche si présence et suppression de %d\n", n);
                l=list_remove(&n, l);
            }
            break;
        case 4:
            printf("Suppression dernier élement\n");
            l=list_del_last(l);
            break;
        
        }
        // printf("size = %d\n", l->size);
        list_printf(l);
    }
    printf("Liberation de la liste \n");
    l=list_delete(l);

    list_t l1=list_new(char_fprintf,char_delete, char_equal, char_new);
    char *letters="abcdefghijklmnopqrstuvwxyz";
    // int j;
    srandom(time(NULL));
    list_printf(NULL);
    for (j=0; j<NBTESTS; j++) {
        switch(random() % NBCAS) {
        case 0:
            char *x=calloc(1,sizeof(char));
            *x = letters[random() % 26]; 
            printf("Ajout début de liste %c :\n ",*x);
            l1=list_add_first(x,l1);
            break;
        case 1:
            char *y=calloc(1,sizeof(char));
            *y = letters[random() % 26]; 
            printf("Ajout fin de liste %c :\n ",*y);
            l1=list_add_last(y,l1);
            break;
        case 2 : 
            printf("Suppression premier élement\n");
            l1=list_del_first(l1);
            break;
        case 3:
            if(l1->data!=NULL){
                char *z=calloc(1,sizeof(char));
                *z = letters[random() % 26]; 
                printf("Recherche si présence et suppression de %c\n", *z);
                l1=list_remove(z, l1);
                free(z);
            }
            break;
        case 4:
            printf("Suppression dernier élement\n");
            l1=list_del_last(l1);
            break;
        
        }
        // printf("size = %d\n", l->size);
        list_printf(l1);
    }
    printf("Liberation de la liste \n");
    l1=list_delete(l1);
}
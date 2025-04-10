#include "vect.h"
#include "mystring.h"
#include "double.h"
#define NBTESTS 1000
#define NBCAS 4

// Programme de tests aleatoire
// 3 fois plus d'ajout que de suppression
int main() {
    vect_t t1 = vect_new(10, double_fprintf, double_delete, double_equal, double_append, double_equal);
    int j,nb;

    for (j=0; j<NBTESTS; j++) {
        switch(random() % NBCAS) {
        case 0: case 1:
            double *x=calloc(1,sizeof(double));
            *x = random() % 100;
            printf("Append de %lf \n: ",*x);
            t1=vect_append(x,t1);
            vect_print(t1, stdout);
            break;
        case 2 : case 3:
            if((nb=random()%((t1->actual_size)+1))>0){
            printf("Suppression des %d dernieres valeurs : ",nb);
            t1=vect_remove_nlast(nb,t1);
            vect_print(t1, stdout);
            }
            break;
        }
    }
    printf("Liberation du tableau\n");
    t1=vect_delete(t1);

    vect_t t2 = vect_new(10, char_fprintf, char_delete, char_equal, char_append, char_compare);
    char *letters="abcdefghijklmnopqrstuvwxyz";
    for (j=0; j<NBTESTS; j++) {
        switch(random() % NBCAS) {
        case 0: case 1:
            char *x=calloc(1,sizeof(char));
            *x = letters[random() % 26]; 
            printf("Append de %c \n: ",*x);
            t2=vect_append(x,t2);
            vect_print(t2, stdout);
            break;
        case 2 : case 3:
            if((nb=random()%((t2->actual_size)+1))>0){
            printf("Suppression des %d dernieres valeurs : ",nb);
            t2=vect_remove_nlast(nb,t2);
            vect_print(t2, stdout);
            }
            break;
        }
    }
    printf("Liberation du tableau\n");
    t2=vect_delete(t2);
}
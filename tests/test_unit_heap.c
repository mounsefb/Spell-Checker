#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heap.h"
#include "double.h"
#include "mystring.h"

#define NBTESTS 100
#define NBCAS 4

int main() {
    heap_t t1 = heap_new(10, double_fprintf, double_delete, double_equal, double_append, double_compare);
    int j,nb;
    srandom(time(NULL));
    for (j=0; j<NBTESTS; j++) {
        switch(random() % NBCAS) {
        case 0: case 1:
            double *x=calloc(1,sizeof(double));
            *x = random() % 100;
            printf("Append de %lf :\n ",*x);
            heap_add(x,t1);
            heap_printf(t1);
            break;
        case 2 : case 3:
            if((nb=random()%((t1->actual_size)+1))>0){
            printf("Suppression de la tete de tas : \n");
            heap_delete_extrema(t1);
            heap_printf(t1);
            }
            break;
        }
        int index=heap_verification(t1);
        if (index!=-1) {
            printf("indice defaillant : %d\n", index);
            exit(EXIT_FAILURE);
        }
        
        // printf("verif : %d\n",heap_verification(t1));
    }
    printf("Liberation du tableau\n");
    t1=heap_delete(t1);

    heap_t t2 = heap_new(10, char_fprintf, char_delete, char_equal, char_append, char_compare);
    char *letters="abcdefghijklmnopqrstuvwxyz";
    for (j=0; j<NBTESTS; j++) {
        switch(random() % NBCAS) {
        case 0: case 1:
            char *x=calloc(2,sizeof(char)); 
            // on prend 2 car x est une chaine de caractere 
            // et on doit tester le caractere final \0
            *x = letters[random() % 26]; 
            printf("Append de %c :\n ",*x);
            heap_add(x,t2);
            heap_printf(t2);
            break;
        case 2 : case 3:
            printf("Suppression de la tete de tas : \n");
            heap_delete_extrema(t2);
            heap_printf(t2);
            break;
        }
        heap_verification(t2);
    }
    printf("Liberation du tableau\n");
    t2=heap_delete(t2);
}
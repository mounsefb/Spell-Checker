#include "sort.h"
#include "heap.h"
    /*Fusion des 2 demi tableaux triés de `tab` compris entre les indices `gauche` et `droit` à l'aide du tableau `tmp`*/
void fusion(vect_t tab, int indice_gauche, int indice_mid, int indice_droite){
    int i = indice_gauche;
    int j = indice_mid + 1;
    int k = 0;

    vect_t tab_new = vect_new((indice_droite - indice_gauche + 1), tab->fprint_data, tab->delete_data, tab->equal_data, tab->append_data, tab->compare_data);

    while (i <= indice_mid && j <= indice_droite) {
        if (tab->compare_data(tab->data[i], tab->data[j]) <= 0) {
            tab_new->data[k++] = tab->data[i++];
        } else {
            tab_new->data[k++] = tab->data[j++];
        }
    }

    while (i <= indice_mid) {
        tab_new->data[k++] = tab->data[i++];
    }

    while (j <= indice_droite) {
        tab_new->data[k++] = tab->data[j++];
    }

    for (i = indice_gauche, k = 0; i <= indice_droite; i++, k++) {
        tab->data[i] = tab_new->data[k];
    }

    vect_delete(tab_new);
}
    /* Tri récursif du tableau `tab` entre les indices `gauche` et `droit` à l'aide du tableau `tmp` */
void trifusionrec(vect_t tab, int indice_gauche, int indice_droite){
    if (indice_droite > indice_gauche) {
        int indice_mid = indice_gauche + (indice_droite - indice_gauche) / 2;
        
        if (indice_mid > indice_gauche) {
            trifusionrec(tab, indice_gauche, indice_mid);
        }

        if (indice_droite > indice_mid + 1) {
            trifusionrec(tab, indice_mid + 1, indice_droite);
        }

        fusion(tab, indice_gauche, indice_mid, indice_droite);
    }
}
    /* Tri du tableau `tab` entier (fonction wrapper cachant les autres paramètres à l'utilisateur) */
void mergesort(vect_t tab){
    trifusionrec(tab, 0, tab->actual_size-1);
}

    /* Intervetit les valeurs du tableau aux deux indice 1 et 2 */
void remplacement(vect_t tab, int indice_1, int indice_2){
    void* tampon;
    tampon = tab->data[indice_1];
    tab->data[indice_1] = tab->data[indice_2];
    tab->data[indice_2] = tampon;
}

    /* Partitionne le tableau `tab` entre les indices `gauche`et `droite`*/
int partitionner(vect_t tab, int indice_gauche, int indice_droite){
    
    //Pivot placé à la fin du tableau
    int indice_pivot = indice_droite; 
    int i = indice_gauche;

    //Place au début du tableau les éléments plus petits que le pivot (ensemble Emax) et à la fin du tableau les éléments plus grands (ensemble Emax)
    for (int j = indice_gauche; j < indice_droite; j++) {
        if (tab->compare_data(tab->data[j], tab->data[indice_pivot])<=0){
            if(i!=j){
                remplacement(tab,i, j);
            }
            i++;
        }
    }
    remplacement(tab,i,indice_droite);
    return i;
}

    /* tri recursif du tableau `tab` entre les indices `gauche`et `droite`*/
void triseg(vect_t tab, int indice_gauche, int indice_droite){
    int indice_pivot;
    if(indice_gauche<indice_droite){
        
        // partitionner le tableau
        indice_pivot = partitionner(tab, indice_gauche, indice_droite);
        if(indice_pivot == indice_droite){
            //Trier que petit
            triseg( tab, indice_gauche, indice_pivot -1);
        }
        else if(indice_pivot == indice_gauche){
            //Trier que grand
            triseg( tab, indice_pivot +1, indice_droite);
        }
        else{
            //Trier plus petits et plus grands
            triseg( tab, indice_gauche, indice_pivot -1);
            triseg( tab, indice_pivot +1, indice_droite);
        }
    }
}

    /* Tri du tableau `tab` entier (fonction wrapper cachant les autres paramètres à l'utilisateur) */
void segsort(vect_t tab){
    triseg(tab, 0, tab->actual_size-1);
}

void heapsort(vect_t tab) {
    // Construire un tas à partir du tableau de N éléments
    for (int i = tab->actual_size / 2 - 1; i >= 0; i--) {
        heapify(tab, tab->actual_size, i);
    }

    // Extraire les éléments un par un du tas
    for (int i = tab->actual_size - 1; i > 0; i--) {
        swap(&(tab->data[0]), &(tab->data[i]));
        heapify(tab, i, 0);
    }
}

void heapify(vect_t tab, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && tab->compare_data(tab->data[left], tab->data[largest]) > 0) {
        largest = left;
    }

    if (right < n && tab->compare_data(tab->data[right], tab->data[largest]) > 0) {
        largest = right;
    }

    if (largest != i) {
        swap(&(tab->data[i]), &(tab->data[largest]));
        heapify(tab, n, largest);
    }
}

void swap(void **a, void **b) {
    void *temp = *a;
    *a = *b;
    *b = temp;
}
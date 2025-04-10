
#include "sort.h" // Assurez-vous que le fichier d'en-tête contient les déclarations nécessaires pour vos structures et fonctions

int comparaison();

int main() {
    srand(time(NULL));
    // Créer un vecteur avec des données non triées
    vect_t my_vector = vect_new(4, double_fprintf, double_delete, double_equal, double_append, double_compare);
    for (size_t i = 0; i < 4; i++) {
        double * x = malloc(sizeof(double));
        *x=rand()%10;
        my_vector = vect_append(x, my_vector); 
    }

    // Afficher le vecteur non trié
    printf("Vecteur non trié: ");
    vect_print(my_vector, stdout);

    int ichoix;
    printf("Choix du trie -> fusion:1 -- Segmentation:2 -- tas:3 -- Comparaison trie:4 \n");
    scanf("%i",&ichoix);
    switch(ichoix)
    {
        case 1:
            //Appliquer le tri fusion
            mergesort(my_vector);
        break;
        case 2:
            //Appliquer le tri Segmention
            segsort(my_vector);
        break;
        case 3:
            //Appliquer le tri tas
            heapsort(my_vector);
        break;
        case 4:
            //Appliquer le tri tas
            comparaison(my_vector);
        break;
        default: 
        printf("mauvais input");

    }


    // Afficher le vecteur trié
    printf("Vecteur trié: ");
    vect_print(my_vector, stdout);

    // Nettoyer la mémoire
    vect_delete(my_vector);

    vect_t my_vector_1 = vect_new(4, char_fprintf, char_delete, char_equal, char_append, char_compare);
     char *letters="abcdefghijklmnopqrstuvwxyz";
    for (size_t i = 0; i < 4; i++) {
        char *x=calloc(2,sizeof(char)); 
        *x = letters[random() % 26]; 
        my_vector_1 = vect_append(x, my_vector_1); 
    }


    // Afficher le vecteur non trié
    printf("Vecteur non trié: ");
    vect_print(my_vector_1, stdout);

    printf("Choix du trie -> fusion:1 -- Segmentation:2 -- tas:3 \n");
    scanf("%i",&ichoix);
    switch(ichoix)
    {
        case 1:
            //Appliquer le tri fusion
            mergesort(my_vector_1);
        break;
        case 2:
            //Appliquer le tri Segmention
            segsort(my_vector_1);

        break;
        case 3:
            //Appliquer le tri tas
            heapsort(my_vector_1);
        break;
        default: 
        printf("mauvais input");

    }

    // Afficher le vecteur trié
    printf("Vecteur trié: ");
    vect_print(my_vector_1, stdout);

    // Nettoyer la mémoire
    vect_delete(my_vector_1);

    return 0;
}

int comparaison() {
    clock_t avant, apres;
    double temps1, temps2, temps3;

    /* Ouverture d'un fichier contenant les temps de calcul */
    FILE *fp = fopen("tempstri.dat", "w");
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    fprintf(fp, "Taille\tTempsQuickSort\tTempsMonTriHeap\tTempsFusionTri\n");

    for (size_t plethore_taille = 1; plethore_taille < 800; plethore_taille++) {
        vect_t t1 = vect_new(plethore_taille, double_fprintf, double_delete, double_equal, double_append, double_compare);
        vect_t t2 = vect_new(plethore_taille, double_fprintf, double_delete, double_equal, double_append, double_compare);
        vect_t t3 = vect_new(plethore_taille, double_fprintf, double_delete, double_equal, double_append, double_compare);
        
        for (size_t i = 0; i < plethore_taille; i++) {
            double *x = malloc(sizeof(double));
            *x = rand() % 10;
            t1 = vect_append(x, t1);
            t2->data[i] = t1->data[i];
            t3->data[i] = t1->data[i];
        }

        t2->actual_size = t1->actual_size;
        t3->actual_size = t1->actual_size;

        /* Tri par quick sort , version de la bibliothèque C */
        avant = clock();
        segsort(t1);
        apres = clock();
        temps1 = ((double)apres - avant) / CLOCKS_PER_SEC;

        /* Tri par ma fonction heap_sort*/
        avant = clock();
        heapsort(t2);
        apres = clock();
        temps2 = ((double)apres - avant) / CLOCKS_PER_SEC;
        // temps2=0;

        /* Tri fusion par ma fonction mergesort*/
        avant = clock();
        mergesort(t3);
        apres = clock();
        temps3 = ((double)apres - avant) / CLOCKS_PER_SEC;

        /* Affichage des temps des 3 tris dans le fichier */
        fprintf(fp, "%d\t%lf\t%lf\t%lf\n", (int)plethore_taille, temps1, temps2, temps3);

        // vect_delete(t1);
        // vect_delete(t2);
        // vect_delete(t3);

    }
    

    /* Affichage des 2 courbes contenues dans le fichier tempstri.dat avec le logiciel gnuplot */
    if (fp) {
        fclose(fp);
        system("gnuplot -p -e \"set logscale x; f(x) = a * x * log(x); fit f(x) 'tempstri.dat' u 1:2 via a;"
                    "set xlabel 'Taille du tableau'; set ylabel 'Temps (s)';"
                    "plot 'tempstri.dat' u 1:2 with line lt 1 lc rgb 'grey' title 'quickSort', a * x * log(x)  with line lt 2 lc rgb 'black' title 'Fit quick';"
                    "g(x) = b * x * log(x);fit g(x) 'tempstri.dat' u 1:3 via b;"
                    "replot 'tempstri.dat' u 1:3 with line lt 1 lc rgb 'yellow' title 'MonTriHeap', b * x * log(x)  with line lt 2 lc rgb 'orange' title 'Fit heap';"
                    "s(x) = e * x * log(x);fit s(x) 'tempstri.dat' u 1:4 via e;"
                    "replot 'tempstri.dat' u 1:4 with line lt 1 lc rgb 'pink' title 'fusionTri', e * x * log(x)  with line lt 2 lc rgb 'red' title 'Fit fusion'; quit\"");
    }
    return 1;
}
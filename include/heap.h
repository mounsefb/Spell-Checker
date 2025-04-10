#ifndef _HEAP
#define _HEAP
// Ne pas oublier d'inclure les vecteurs génériques
#include <vect.h>
// Le tas est un simple tableau avec des fonctionnalités supplémentaires ci dessous
typedef vect_t heap_t;

  // Creation d'un tas de n elements
heap_t heap_new(unsigned int n,void (*print_data)(void*,FILE*),
      int (*delete_data)(void*),int (*equal_data)(void*,void*),vect_t (*append_data)(void*,vect_t),
      int (*compare_data)(void*,void*)) ;
int heap_is_empty(heap_t tas);
  // Ajout d'un element avec redimensionnement si nécessaire
int heap_add(void* valeur, heap_t tas);
  // Obtenir l'extrema
void* heap_get_extrema(heap_t tas);
  // Supprimer le premier element et reoganisation du tas
int heap_delete_extrema(heap_t tas);
  // Fonction de vérification entre pere et fils
int heap_verification(heap_t tas) ;
  // Suppression du tas
heap_t heap_delete(heap_t tas);
  // Affichage du tas
void heap_printf(heap_t tas) ;
void heap_fprintf(heap_t tas,FILE*) ;


#define HEAP_FATHER(i) ( ((i)-1)/2)
#define HEAP_LEFTSON(i) ( 2*(i)+1)
#define HEAP_RIGHTSON(i) ( 2*((i)+1))

#endif
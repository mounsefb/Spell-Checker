#include "heap.h"

heap_t heap_new(unsigned int n,void (*print_data)(void*,FILE*),
      int (*delete_data)(void*),int (*equal_data)(void*,void*),vect_t (*append_data)(void*,vect_t),
      int (*compare_data)(void*,void*)){
    heap_t heap;
    heap=vect_new(n, print_data, delete_data, equal_data,append_data, compare_data);
    return heap;
}

int heap_is_empty(heap_t tas){
    return tas->actual_size==0;
}

  // Ajout d'un element avec redimensionnement si nécessaire
int heap_add(void* valeur, heap_t tas){
    tas=vect_append(valeur, tas);
    if (tas->actual_size==1) return 1;
    int i=tas->actual_size-1;
    void * tmp;
    if ((unsigned int)HEAP_FATHER(i)>=0 && tas->compare_data(tas->data[HEAP_FATHER(i)],tas->data[i])<0){
      tmp=tas->data[HEAP_FATHER(i)];
      tas->data[HEAP_FATHER(i)]=tas->data[i];
      tas->data[i]=tmp;
      i=HEAP_FATHER(i);
    }
    while (((unsigned int)HEAP_LEFTSON(i)<tas->actual_size && tas->compare_data(tas->data[HEAP_LEFTSON(i)],tas->data[i])>0) || 
            ((unsigned int)HEAP_RIGHTSON(i)<tas->actual_size && tas->compare_data(tas->data[HEAP_RIGHTSON(i)],tas->data[i])>0) ||
            ((unsigned int)HEAP_FATHER(i)>=0 && tas->compare_data(tas->data[HEAP_FATHER(i)],tas->data[i])<0)){
      if ((unsigned int)HEAP_LEFTSON(i)<tas->actual_size && tas->compare_data(tas->data[HEAP_LEFTSON(i)],tas->data[i])>0){
          tmp=tas->data[HEAP_LEFTSON(i)];
          tas->data[HEAP_LEFTSON(i)]=tas->data[i];
          tas->data[i]=tmp;
          i=HEAP_LEFTSON(i);
      }
      else if ((unsigned int)HEAP_RIGHTSON(i)<tas->actual_size && tas->compare_data(tas->data[HEAP_RIGHTSON(i)],tas->data[i])>0){
          tmp=tas->data[HEAP_RIGHTSON(i)];
          tas->data[HEAP_RIGHTSON(i)]=tas->data[i];
          tas->data[i]=tmp;
          i=HEAP_RIGHTSON(i);
      }
      else if ((unsigned int)HEAP_FATHER(i)>=0 && tas->compare_data(tas->data[HEAP_FATHER(i)],tas->data[i])<0){
        tmp=tas->data[HEAP_FATHER(i)];
        tas->data[HEAP_FATHER(i)]=tas->data[i];
        tas->data[i]=tmp;
        i=HEAP_FATHER(i);
      }
    }


  return 1;
}

  // Obtenir l'extrema
void* heap_get_extrema(heap_t tas){
  return tas->data[0];
}

  // Supprimer le premier element et reoganisation du tas
int heap_delete_extrema(heap_t tas){
    if (tas->actual_size>1){
      tas->delete_data(tas->data[0]);
      tas->data[0]=tas->data[tas->actual_size-1];
      // printf("on deplace le dernier vers l'avant : ");
      tas->actual_size--;
      // heap_printf(tas);
      
      int i=heap_verification(tas);
      void * tmp;
      while (i!=-1){
        if ((unsigned int)HEAP_LEFTSON(i)<tas->actual_size && tas->compare_data(tas->data[HEAP_LEFTSON(i)],tas->data[i])>0){
            tmp=tas->data[HEAP_LEFTSON(i)];
            tas->data[HEAP_LEFTSON(i)]=tas->data[i];
            tas->data[i]=tmp;
            i=HEAP_LEFTSON(i);
        }
        else if ((unsigned int)HEAP_RIGHTSON(i)<tas->actual_size && tas->compare_data(tas->data[HEAP_RIGHTSON(i)],tas->data[i])>0){
            tmp=tas->data[HEAP_RIGHTSON(i)];
            tas->data[HEAP_RIGHTSON(i)]=tas->data[i];
            tas->data[i]=tmp;
            i=HEAP_RIGHTSON(i);
        }
        else if ((unsigned int)HEAP_FATHER(i)>=0 && tas->compare_data(tas->data[HEAP_FATHER(i)],tas->data[i])<0){
          tmp=tas->data[HEAP_FATHER(i)];
          tas->data[HEAP_FATHER(i)]=tas->data[i];
          tas->data[i]=tmp;
          i=HEAP_FATHER(i);
        }
        i=heap_verification(tas);

      }
    }
    else if(tas->actual_size==1){
      tas->delete_data(tas->data[0]);
      tas->actual_size--;
    }
    return 1;
}

  // Fonction de vérification entre pere et fils
int heap_verification(heap_t tas){
  for (unsigned int i=0; i<tas->actual_size; i++){
    if(((unsigned int)HEAP_LEFTSON(i)<tas->actual_size && 
        tas->compare_data(tas->data[HEAP_LEFTSON(i)],tas->data[i])>0) || 
            ((unsigned int)HEAP_RIGHTSON(i)<tas->actual_size && 
                tas->compare_data(tas->data[HEAP_RIGHTSON(i)],tas->data[i])>0)) return i;
  }
  return -1;
}

  // Suppression du tas
heap_t heap_delete(heap_t tas){
  return vect_delete(tas);
}


  // Affichage du tas
void heap_printf(heap_t tas){
  vect_print(tas, stdout);
}
void heap_fprintf(heap_t tas,FILE*file){
  vect_print(tas, file);
}
#ifndef _VECT_H
#define _VECT_H

#include <stdlib.h>
#include <stdio.h>


typedef struct _vect_t {
  void* * data;
  unsigned int max_size;
  unsigned int actual_size;

  void (*fprint_data)(void*,FILE*);
  int (*delete_data)(void*);
  int (*equal_data)(void*,void*);
  int (*compare_data)(void*,void*);
  struct _vect_t * (*append_data)(void*,struct _vect_t *);
}  * vect_t;

vect_t vect_new(unsigned int n,void (*print_data)(void*,FILE*), 
int (*delete_data)(void*),int (*equal_data)(void*,void*), vect_t (*append_data)(void*,vect_t),
int (*compare_data)(void*,void*));

vect_t vect_append(void* val, vect_t table);

vect_t vect_remove_nlast(unsigned int n, vect_t table);


void vect_print(vect_t ptab, FILE * file);

vect_t vect_delete(vect_t table);
vect_t double_append(void* val, vect_t table);
vect_t char_append(void* val, vect_t table);

#endif
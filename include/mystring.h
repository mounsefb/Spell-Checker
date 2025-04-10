#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

#ifndef _MYSTRING_H
#define _MYSTRING_H

void char_random(void* p);

void* char_new(void* val);

void char_printf( void * data);

void char_fprintf( void * data, FILE* file);

int char_delete(void* data);
int char_equal(void * data1, void * data2);
int char_compare(void * data1, void * data2);
#endif
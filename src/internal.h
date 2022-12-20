#ifndef arba_INTERNAL_H
#define arba_INTERNAL_H
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct {
        size_t *datum;
        int sign;
        size_t radix;
	size_t total_memory;
	size_t digits;
} arba_fixed_point;


#endif
arba_fixed_point *add(arba_fixed_point *, arba_fixed_point *, arba_fixed_point *);
arba_fixed_point *expand(arba_fixed_point *, size_t);
int arba_ascii_to_base(int);
arba_fixed_point *arba_string_to_number(arba_fixed_point *, char *);
int arba_pbase(int);
void arba_print(FILE *, arba_fixed_point *);


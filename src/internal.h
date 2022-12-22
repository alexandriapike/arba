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
	size_t base;
} arba_fixed_point;

static size_t _sbase = 10;
void arba_setbase(size_t);
arba_fixed_point *multiply(arba_fixed_point *, arba_fixed_point *, arba_fixed_point *);
void arba_free(arba_fixed_point *);
arba_fixed_point *add(arba_fixed_point *, arba_fixed_point *, arba_fixed_point *);
arba_fixed_point *expand(arba_fixed_point *, size_t);
int arba_ascii_to_base(int);
arba_fixed_point *arba_string_to_number(arba_fixed_point *, char *);
int arba_pbase(int);
void arba_print(FILE *, arba_fixed_point *);

#endif

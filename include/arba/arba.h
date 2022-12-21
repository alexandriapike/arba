#ifndef arba_H
#define arba_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
typedef struct arba_fixed_point arba_fixed_point;
void arba_free(arba_fixed_point *);
arba_fixed_point *add(arba_fixed_point *, arba_fixed_point *, arba_fixed_point *);
arba_fixed_point *expand(arba_fixed_point *, size_t);
int arba_ascii_to_base(int);
arba_fixed_point *arba_string_to_number(arba_fixed_point *, char *);
int arba_pbase(int);
void arba_print(FILE *, arba_fixed_point *);


#ifdef __cplusplus
}
#endif

#endif


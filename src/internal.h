#ifndef arba_INTERNAL_H
#define arba_INTERNAL_H
#include <stddef.h>
#include <stdlib.h>
typedef struct {
        size_t *datum;
        int sign;
        size_t radix;
	size_t total_memory;
        size_t digits;
} arba_fixed_point;


#endif

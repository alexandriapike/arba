#include <arba/arba.h>
#include <stdio.h>

int main(void)
{
	arba_fixed_point *a = NULL;
	a = arba_string_to_number(a, "12.34");
	arba_fixed_point *b = NULL;
	b = arba_string_to_number(b, "123.4");
	arba_fixed_point *c = NULL;
	c = multiply(a, b, c);
	arba_print(stdout, c);
	arba_free(a);
	arba_free(b);
	arba_free(c);
	return 0;
}


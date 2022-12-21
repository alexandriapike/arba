#include <arba/arba.h>
#include <stdio.h>

int main(void)
{
	arba_fixed_point *a = NULL;
	a = arba_string_to_number(a, "+.123456789");
	arba_print(stdout, a);
	return 0;
}


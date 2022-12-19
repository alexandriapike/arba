#include <arba/arba.h>
#include <stdio.h>
int main(void)
{
	FILE *fp = NULL;
	arba_fixed_point *a = NULL;
	a = arba_string_to_number(a, "123456");
	
	arba_print(fp, a);
	return 0;

}


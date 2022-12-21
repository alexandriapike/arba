#include "internal.h"

arba_fixed_point *add(arba_fixed_point *a, arba_fixed_point *b, arba_fixed_point *c)
{
	(void) a;
	(void) b;
	(void) c;
	return c;
}

arba_fixed_point *expand(arba_fixed_point *a, size_t length)
{
	/* if a == NULL then this is a new bignum */
	/* so we may as well take the caller's size request */
	/* and get out of here */
	
	if (a == NULL) {
		a = malloc(sizeof(arba_fixed_point));
		a->total_memory = length;
		a->datum = calloc(1, sizeof(size_t) * a->total_memory + 1);
		a->radix = (sizeof(size_t));
		a->sign = (sizeof(size_t));
		// let the caller count digits;
		a->digits = 0;
		return a;
	} else {
		a->total_memory = length;
		a->datum = realloc(a->datum, sizeof(size_t) * a->total_memory + 1);
	}
	return a;
	
}

void arba_free(arba_fixed_point *a)
{
	a->radix = 0;
	a->sign = 0;
	a->digits = 0;
	a->total_memory = 0;
	free(a->datum);
	free(a);
}

int arba_ascii_to_base(int letter)
{
        int glyphs[110] = {
        '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000',
        '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000',
        '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000',
        '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000',
        '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000', '\000',
        '\000', '\000', '\000', '\000', '\001', '\002', '\003', '\004', '\005',
        '\006', '\007', '\010', '\011', '\000', '\000', '\000', '\000', '\000',
        '\000', '\000', '\012', '\013', '\014', '\015', '\016', '\017', '\020',
        '\021', '\022', '\023', '\024', '\025', '\026', '\027', '\030', '\031',
        '\032', '\033', '\034', '\035', '\036', '\037', '\040', '\041', '\042',
        '\043', '\044', '\045', '\046', '\047', '\050', '\051', '\052', '\053' };
        if (letter < 110)
                return glyphs[letter];
        return 0;
}

arba_fixed_point *arba_string_to_number(arba_fixed_point *f, char *s)
{
	size_t i = 0;
	size_t k = 0;
	f = expand(f,0); // zeroth place is still a 1 size datum
	
	for (i = 0;s[i] != 0; ++i)
	{
		if (s[i] == '.') {
			f->radix = k;
		} else if (s[i] == '+') {
			f->sign = 0;
		} else if (s[i] == '-') {
			f->sign = 1;
		} else {
			f = expand(f, k + 1); // zeroth place is still a 1 size datum
			f->datum[k] = arba_ascii_to_base(s[i]);
			f->digits++;
			k++;
	       	}
	}
	return f;
}

int arba_pbase(int a)
{
	static int base[36] = { '\060', '\061', '\062', '\063', '\064', '\065',
	       			'\066', '\067', '\070', '\071', '\101', '\102', 
				'\103', '\104', '\105', '\106', '\107', '\110',
			       	'\111', '\112', '\113', '\114', '\115', '\116',
			       	'\117', '\120', '\121', '\122', '\123', '\124',
			       	'\125', '\126', '\127', '\130', '\131', '\132' };

	if (a < 36) 
		return base[a]; 
	else 
		return a;
}

/* use fputc to abstract away any write() buffering needs */
void arba_print(FILE *fp, arba_fixed_point *a)
{
	size_t i = 0;
	size_t k = a->sign;

	if (k == 1)
		fputc('-', fp);
	if (k == 0)
		fputc('+', fp);
	for (; i < a->digits ; ++i, ++k) {
		if (k != 0 && k % 68 == 0) {
			fputc('\\', fp);
			fputc('\n', fp);
		}
		if (a->radix == i) {
			fputc('.', fp);
			if ((++k) % 68 == 0) {
				fputc('\\', fp);
				fputc('\n', fp);
			}
		}
		fputc(arba_pbase(a->datum[i]), fp);
	}

	if (!a->digits) {
		fputc('0', fp);
	}

	fputc('\n', fp);

	fflush(fp);
}





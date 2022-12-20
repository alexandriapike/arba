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
		a->digits = a->total_memory;
		a->radix = (sizeof(size_t));
		a->sign = 0;
		return a;
	} else {
		a->total_memory = length;
		a->datum = realloc(a->datum, sizeof(size_t) * a->total_memory + 1);
		a->digits = a->total_memory;
	}
	return a;
	
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
	f = expand(f,0); // zeroth place is still a 1 size datum
	for (i = 0;s[i] != 0; ++i)
	{
		
		if (s[i] == '.') {
			f->radix = i;
		} else if (s[i] == '+') {
			f->sign = 0;
		} else if (s[i] == '-') {
			f->sign = 1;
		} else {
			f = expand(f, i + 1); // zeroth place is still a 1 size datum
			f->datum[i] = arba_ascii_to_base(s[i]); }
	}

	return f;
}

int arba_pbase(int a)
{

	static int base[36] = { '0', '1', '2', 
				'3', '4', '5', 
				'6', '7', '8',
				'9', 'A', 'B', 
				'C', 'D', 'E', 
				'F', 'G', 'H',
				'I', 'J', 'K', 
				'L', 'M', 'N', 
				'O', 'P', 'Q',
				'R', 'S', 'T', 
				'U', 'V', 'W', 
				'X', 'Y', 'Z' };
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


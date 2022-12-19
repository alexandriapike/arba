#include "internal.h"

arba_fixed_point *add(arba_fixed_point *a, arba_fixed_point *b, arba_fixed_point *c)
{

	return c;
}

arba_fixed_point *expand(arba_fixed_point *a, size_t length, size_t radix, int sign)
	
{
	/* if a == NULL then this is new bignum */
	/* so we may as well take the caller's size request */
	/* and get out of here */

	if (a == NULL) {
		a = malloc(sizeof(arba_fixed_point));
		a->total_memory = length;
		size_t *bb = a->datum;
		bb = calloc(1, sizeof(size_t) * a->total_memory + 1);
		a->datum = bb;
		a->digits = a->total_memory;
		return a;
	} else {
	/* otherwise let the system's realloc do the dirty work */
	/* we MUST assume an intelligent libc memory reallocation scheme */
	/* or we will divest into primitive memory management -- buffering
	 * calls to mmap et al. is the job of malloc et al. 
	 * just as buffering calls to read et al. is the job of fread et al.
	 * we hereby establish arba as a bignum system that derives micro-opt-
	 * imization from the libc and compiler and emperical optimization
	 * from mathematics algorithms */
	/* furthermore, to assume a memory request should be +1 is */
	/* utter madness 999 * 999 = 998001 and transcendental parts are
	 * determined by, and not to exceed the length of their inputs */
		a->total_memory += length;
		size_t *cc;
		cc = a->datum;
		cc = realloc(cc, sizeof(size_t) * a->total_memory);
		a->datum =cc;
		a->digits = a->total_memory;
	}
	/* we assume the caller may be setting the radix or sign */
	//a->radix = radix;
	//a->sign = sign;
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
	for (i = 0; s[i] != 0; ++i)
	{
		if (s[i] == '.') {
			f = expand(f, i, 0, 0);
			f->radix = i;
		}
		else if (s[i] == '+') {
			f = expand(f, i, 0, 0);
			f->sign = 0;
		} else if (s[i] == '-') {
			f = expand(f, i, 0, 0);
			f->sign = 1;
		}
		f = expand(f, i, 0, 0);

		f->datum[i] = arba_ascii_to_base(s[i]);
		//f->digits += 1;
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
		fputc(arba_pbase((a->datum[i])), fp);
	}

	if (!a->digits) {
		fputc('0', fp);
	}

	fputc('\n', fp);

	fflush(fp);
}


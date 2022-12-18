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
		a->digits = calloc(1, sizeof(size_t) * a->total_memory);
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
		a = realloc(a, a->total_memory);
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
		if (s[i] == ".") {
			f = expand(f, i, 0, 0);
			f->radix = i;
		}
		else if (s[i] == "+") {
			f = expand(f, i, 0, 0);
			f->sign = 1;
		} else if (s[i] == "-") {
			f = expand(f, i, 0, 0);
			f->sign = 0;
		}
		f->digits = arba_ascii_to_base(s[i]);
	}
	return f;
}

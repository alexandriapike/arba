#include "internal.h"

void arba_setbase(size_t a)
{
	_sbase = a;
}

arba_fixed_point *multiply(arba_fixed_point *a, arba_fixed_point *b, arba_fixed_point *c)
{
        size_t prod = 0;
        size_t carry = 0;
        size_t i = 0;
        size_t j = 0;
        size_t k = 0;

	c = expand(c, a->digits+b->digits);
        c->datum[a->digits+b->digits-1] = c->datum[0] = 0;
	c->radix = a->radix + b->radix;
        
        for (i = a->digits; i > 0 ; i--){
                for (j = b->digits, k = i + b->digits, carry = 0; j > 0 ; j--, k--){
                        prod = a->datum[i-1] * b->datum[j-1] + c->datum[k-1] + carry;
                        carry = prod / _sbase;
                        c->datum[k-1] = prod % _sbase;
                }
                c->datum[k-1] = carry;
        }
	c->digits = a->digits + b->digits;
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
        int octals[110] = {
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
        '\043', '\044', '\045', '\046', '\047', '\050', '\051', '\052', '\053' 
	};
        if (letter < 110)
                return octals[letter];
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
	static int octl[36] = { '\060', '\061', '\062', '\063', '\064', '\065',
	       			'\066', '\067', '\070', '\071', '\101', '\102', 
				'\103', '\104', '\105', '\106', '\107', '\110',
			       	'\111', '\112', '\113', '\114', '\115', '\116',
			       	'\117', '\120', '\121', '\122', '\123', '\124',
			       	'\125', '\126', '\127', '\130', '\131', '\132' };

	if (a < 36) 
		return octl[a]; 
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




#include "internal.h"

/* Copyright 2017-2019 CM Graff */


/*
	While many subtraction routines require that the numbers first
	be compared and then rearranged in the case that the zero threshold
	is crossed, we mitigate this by using a special property of
	subtraction that takes advantage of a left over carry. When a
	left over carry is detected, the inverse of the solution is used.
	Ergo, if the solution was 999 we would perform the operation:

		answer = 1000 - 999

	Instead of performing this operation in its entirety however, the
	inverse solution is calculated alongside the normative one and
	simply discarded in the case it is not needed (when there is no left
	over carry).

	Addition:

		six_loop_add:
			
			This addition function aims to be fully optimized
			in regards to the number of conditional decisions
			per loop.

		arb_add_inter:

			arb_add_inter makes use of the _pl function in
			order to express addition as consisely as possible.
			This function has the drawback of fitting many
			conditionals into the same looping code block --
			some of which may or may not be optimized by the
			compiler
	Subtraction:

		arb_sub_inter:

			See arb_add_inter for details about arb_sub_inter
			as they are nearly identical functions.

		five_loop_sub:

			Work in progress 

			Correct output

			Note that most implementations are using compare()
		       	beforehand for subtractions

*/

fxdpnt *five_loop_sub(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	size_t i = 0;
	size_t k = 0;	
	size_t j = 0;
	size_t len = 0;
	ARBT sum = 0;
	int8_t borrow = 0;
	int8_t mborrow = -1; /* mirror borrow must be -1 */
	ARBT mir = 0;
	UARBT *array = NULL;
	UARBT *tmp = NULL;
	ARBT hold = 0;
	
	size_t array_allocated = (MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) + 1);

	array = arb_malloc(array_allocated * sizeof(UARBT));
	j = MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) -1;

	size_t y = b->len-1;
	size_t z = a->len-1;

	/* take care of differing tails to the right of the radix */
	if (rr(a) > rr(b)) {
		len = rr(a) - rr(b);
		for (i=0;i < len; i++, j--, z--, c->len++) {
			hold = a->number[z];
			sum = hold + borrow;
			mir = hold + mborrow;
			borrow = mborrow = 0;
			if(sum < 0) {
				borrow = -1;
				sum += base;
			}
			if(mir < 0) {
				mborrow = -1;
				mir += base;
			}
			c->number[j] = sum;
			array[j] = (base-1) - mir;
		}
	}
	/* perform subtraction from 0 on the bottom long tail */
	else if (rr(b) > rr(a)) {
		len = rr(b) - rr(a);
		for (k=0;k < len; k++, j--, y--, c->len++) {
			hold = 0 - b->number[y];
			sum = hold + borrow;
			mir = hold + mborrow;
			borrow = mborrow = 0;
			if(sum < 0) {
				borrow = -1;
				sum += base;
			}
			if(mir < 0) {
				mborrow = -1;
				mir += base;
			}
			c->number[j] = sum;
			array[j] = (base-1) - mir;
		}
	}

	for (;i < a->len && k < b->len; j--, c->len++, i++, k++, z--, y--) { 
		hold = a->number[z] - b->number[y]; 
		sum = hold + borrow;
		mir = hold + mborrow;
		borrow = mborrow = 0;
		if(sum < 0) {
			borrow = -1;
			sum += base;
		}
		if(mir < 0) {
			mborrow = -1;
			mir += base;
		}
		c->number[j] = sum;
		array[j] = (base-1) - mir;
	}

	for (;i < a->len; j--, c->len++, i++, z--) { 
		hold = a->number[z]; 
		sum = hold + borrow;
		mir = hold + mborrow;
		borrow = mborrow = 0;
		if(sum < 0) {
			borrow = -1;
			sum += base;
		}
		if(mir < 0) {
			mborrow = -1;
			mir += base;
		}
		c->number[j] = sum;
		array[j] = (base-1) - mir;
	}

	for (;k < b->len; j--, c->len++, k++, y--) { 
		hold = 0 - b->number[y];
		sum = hold + borrow;
		mir = hold + mborrow;
		borrow = mborrow = 0;
		if(sum < 0) {
			borrow = -1;
			sum += base;
		}
		if(mir < 0) {
			mborrow = -1;
			mir += base;
		}
		c->number[j] = sum;
		array[j] = (base-1) - mir;
	}

	/* a left over borrow indicates that the zero threshold was crossed */
	if (borrow == -1) {
		tmp = c->number;
		c->number = array;
		c->allocated = array_allocated; // TODO: this should be scaled
		free(tmp);
		arb_flipsign(c);
	}else {
		free(array);
	}
	return c;
}

size_t _l(arba_fixed_point *a)
{
        return a->radix;
}

size_t _r(arba_fixed_point *a)
{
        return a->digits - a->radix;
}



fxdpnt *six_loop_add(const fxdpnt *a, const fxdpnt *b, fxdpnt *c, int base)
{
	/* This addition function is designed to make a small
	 * number of conditional decisions per loop. Hence, why
	 * there are so many loops. Using temporary variables
	 * doesn't save much space over using additional loops
	 * (maybe 6 loc?) so we go ahead and use the extra loops.
	*/

	size_t i = 0;
	size_t k = 0;
	//size_t j = MAX(rr(a), rr(b)) + MAX(rl(a), rl(b)) -1;
	size_t j = 0;
	size_t len = 0;
	int sum = 0;
	int carry = 0;
	size_t z = a->len -1;
	size_t y = b->len -1;

	expand(c

	/* take care of differing tails to the right of the radix */
	if (_r(a) > _r(b)) {
		len = _r(a) - _r(b);
		for (i=0;i < len; i++, j--, z--, c->len++) {
                        c->datum[j] = a->datum[z];
                }
	}
	else if (_r(b) > _r(a)) {
		len = _r(b) - _r(a);
		 for (i=0;i < len; i++, j--, z--, c->len++) {
                        c->datum[j] = a->datum[z];
                }

	}
	if (rr(a) > rr(b)) {
		len = _r
		for (i=0;i < len; i++, j--, z--, c->len++) {
			c->number[j] = a->number[z];
		}
	}
	else if (rr(b) > rr(a)) {
		len = rr(b) - rr(a);
		for (k=0;k < len; k++, j--, y--, c->len++) {
			c->number[j] = b->number[y];
		}
	}

	/* numbers are now compatible for a straight-forward add */
	for (;i < a->len && k < b->len; i++, j--, k++, z--, y--, c->len++) {
		sum = a->number[z] + b->number[y] + carry; 
		carry = 0;
		if (sum >= base) {
			sum -= base;
			carry = 1;
		}
		c->number[j] = sum;
	}

	/* one number may be longer than the other to the left */
	for (;i < a->len; i++, j--, z--, c->len++) { 
		sum = a->number[z] + carry; 
		carry = 0;
		if (sum >= base) {
			sum -= base;
			carry = 1;
		}
		c->number[j] = sum;
	}

	for (;k < b->len; j--, k++, y--, c->len++) { 
		sum = b->number[y] + carry; 
		carry = 0;
		if (sum >= base) {
			sum -= base;
			carry = 1;
		}
		c->number[j] = sum;
	}

	/* handle the final left over carry */
	if (carry) {
		for(i = c->len+1;i > 0; i--) {
			c->number[i] = c->number[i-1];
		}
		c->number[0] = 1;
		c->len++;
		c->lp++;
	}

	return c;
}


#include <stdio.h>
#include <stdlib.h>
#include "frac.h"

int main(void)
{
	fraction a, b;
	char *str;
	a = frac_make(27, 16);
	b = frac_make(5, 12);
	b = frac_add(a, b);
	str = frac_print(b);
	printf("27/16 + 5/12 = %s\n", str);
	free(str);

	a = frac_make(1, 12);
	b = frac_make(5, 6);
	b = frac_sub(a, b);
	str = frac_print(b);
	printf("1/12 - 5/6 = %s\n", str);
	free(str);

	a = frac_make(98, 53);
	b = frac_make(9, 34);
	b = frac_mul(a, b);
	str = frac_print(b);
	printf("98/53 * 9/34 = %s\n", str);
	free(str);

	a = frac_make(7, 9);
	b = frac_make(4, 15);
	b = frac_div(a, b);
	str = frac_print(b);
	printf("7/9 / 4/15 = %s\n", str);
	free(str);
	return 0;
}

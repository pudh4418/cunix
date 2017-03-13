#include <stdio.h>
#include <stdlib.h>
#include "frac.h"

int gcd(int x, int y)
{
	int r = x % y;
	if (r != 0)
		return gcd(y, r);
	else
		return y;
}

fraction frac_inv(fraction f)
{
	fraction res;
	res.denominator = f.numerator;
	res.numerator = f.denominator;
	return res;
}

static int signum(int x)
{
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	return 0;
}

fraction frac_canon(fraction f)
{
	int sn, g;

	if (f.denominator == 0) {
		fprintf(stderr, "Divided by zero error.\n");
		exit(1);
	}

	sn = signum(f.numerator) * signum(f.denominator);
	f.numerator = abs(f.numerator);
	f.denominator = abs(f.denominator);
	g = gcd(f.numerator, f.denominator);
	f.numerator /= g;
	f.denominator /= g;
	f.numerator *= sn;

	return f;
}

fraction frac_make(int x, int y)
{
	fraction res;

	res.numerator = x;
	res.denominator = y;
	return frac_canon(res);
}

fraction frac_add(fraction x, fraction y)
{
	fraction res;

	res.numerator = x.numerator * y.denominator + y.numerator * x.denominator;
	res.denominator = x.denominator * y.denominator;
	return frac_canon(res);
}

fraction frac_sub(fraction x, fraction y)
{
	fraction res;

	res.numerator = x.numerator * y.denominator - y.numerator * x.denominator;
	res.denominator = x.denominator * y.denominator;
	return frac_canon(res);
}

fraction frac_mul(fraction x, fraction y)
{
	fraction res;

	res.numerator = x.numerator * y.numerator;
	res.denominator = x.denominator * y.denominator;
	return frac_canon(res);
}

fraction frac_div(fraction x, fraction y)
{
	return frac_mul(x, frac_inv(y));
}

char *frac_print(fraction f)
{
	char *res = NULL;
	asprintf(&res, "%d/%d", f.numerator, f.denominator);
	return res;
}


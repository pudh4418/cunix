#ifndef FRAC_H
#define FRAC_H

struct fraction {
	int numerator;
	int denominator;
};

typedef struct fraction fraction;

int gcd(int x, int y);
fraction frac_inv(fraction f);
fraction frac_canon(fraction f);
fraction frac_make(int x, int y);
fraction frac_add(fraction x, fraction y);
fraction frac_sub(fraction x, fraction y);
fraction frac_mul(fraction x, fraction y);
fraction frac_div(fraction x, fraction y);
char *frac_print(fraction f);

#endif


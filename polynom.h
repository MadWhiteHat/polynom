#ifndef _POLYNOM_H
#define _POLYNOM_H

#include <stdio.h>
#include <stdint.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct {
  int64_t* coefs;
  int64_t size;
  int64_t capacity;
} coefs_array;

typedef struct polynomial {
  coefs_array coefs;
  char letter;
} polynomial_t;

extern void print_polynomial(polynomial_t* __polynomial);
extern polynomial_t* allocate_polinomial(int64_t __power);
extern polynomial_t* init_polynomial(
  int64_t __coef,
  char __letter,
  int64_t __power
);

extern polynomial_t* neg_polynomial(polynomial_t* __polynomial);

extern polynomial_t* sum_polynomials(
  polynomial_t* __polynomial1,
  polynomial_t* __polynomial2,
  const char __action
);

extern polynomial_t* mul_polynomials(
  polynomial_t* __polynomial1,
  polynomial_t* __polynomial2
);

extern void karatsuba(
  int64_t* __coefs1,
  int64_t* __coefs2,
  int64_t* __res_coefs,
  int64_t __size
);

extern polynomial_t* copy_polynomial(polynomial_t* __polynomial);
extern polynomial_t* pow_polynomial(polynomial_t* __polynomial, int64_t __power);
extern void shrink_to_fit_polynomial(polynomial_t* __polynomial);

extern void is_valid_operation(
  polynomial_t* __polynomial1,
  polynomial_t* __polynomial2
);

extern void deallocate_polynomial(polynomial_t* __polynomial);

extern void yyerror(const char* __msg);
extern int yylex();

#endif // _POLYNOM_H

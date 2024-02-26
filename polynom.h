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

extern void print_polynomial(polynomial_t* polynomial);
extern polynomial_t* allocate_polinomial(int64_t power);
extern polynomial_t* create_polynomial(
  int64_t coef,
  char letter,
  int64_t power
);

extern polynomial_t* neg_polynomial(polynomial_t* polynomial);

extern polynomial_t* sum_polynomials(
  polynomial_t* polynomial1,
  polynomial_t* polynomial2,
  const char action
);

extern polynomial_t* mul_polynomials(
  polynomial_t* polynomial1,
  polynomial_t* polynomial2
);

extern void karatsuba(
  int64_t* coefs1,
  int64_t* coefs2,
  int64_t* res_coefs,
  int64_t size
);

extern polynomial_t* copy_polynomial(polynomial_t* polynomial);
extern polynomial_t* pow_polynomial(polynomial_t* polynomial, int64_t power);
extern void shrink_to_fit_polynomial(polynomial_t* polynomial);

extern void is_valid_operation(
  polynomial_t* polynomial1,
  polynomial_t* polynomial2
);

extern void deallocate_polynomial(polynomial_t* polynomial);

extern void yyerror(const char* msg);
extern int yylex();

#endif // _POLYNOM_H

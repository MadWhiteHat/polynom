#ifndef _POLYNOM_H
#define _POLYNOM_H

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

// Constructor
polynomial_t*
create_polynomial(int64_t coef, char letter, int64_t power);

// Destructor
void
delete_polynomial(polynomial_t* polynomial);

// Copy constructor
polynomial_t*
copy_polynomial(polynomial_t* polynomial);

// Polynomial memory resources management
polynomial_t*
allocate_polynomial(int64_t power);

void
shrink_to_fit_polynomial(polynomial_t* polynomial);

// Polynomial operations
void
is_valid_polynomial_operation(polynomial_t* lhs, polynomial_t* rhs);

void
print_polynomial(polynomial_t* polynomial);

polynomial_t*
sum_polynomials(polynomial_t* lhs, polynomial_t* rhs, const char action);

polynomial_t*
mul_polynomials(polynomial_t* lhs, polynomial_t* rhs);

void
karatsuba(int64_t* coefs1, int64_t* coefs2, int64_t* res_coefs, int64_t size);

polynomial_t*
neg_polynomial(polynomial_t* polynomial);

polynomial_t*
pow_polynomial(polynomial_t* polynomial, int64_t power);

extern int yylex();

#endif // _POLYNOM_H

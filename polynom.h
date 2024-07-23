#ifndef _POLYNOM_H
#define _POLYNOM_H

#include <stdint.h>

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
int
create_polynomial(
  polynomial_t** this,
  int64_t coef,
  char letter,
  int64_t power
);

// Destructor
void
delete_polynomial(polynomial_t** this);

// Copy constructor
int
copy_polynomial(polynomial_t** this, polynomial_t* other);

// Polynomial memory resources management
int
allocate_polynomial(polynomial_t** this, int64_t power);

int
shrink_to_fit_polynomial(polynomial_t* this);

// Polynomial operations
int
is_valid_polynomial_operation(polynomial_t* lhs, polynomial_t* rhs);

int
is_valid_polynomial(polynomial_t* this);

void
debug_polynomial(polynomial_t* this);

void
print_polynomial(polynomial_t* this);

int
sum_polynomials(
  polynomial_t** this,
  polynomial_t* lhs,
  polynomial_t* rhs,
  const char action
);

int
mul_polynomials(polynomial_t** this, polynomial_t* lhs, polynomial_t* rhs);

int
karatsuba(int64_t* coefs1, int64_t* coefs2, int64_t* res_coefs, int64_t size);

int
neg_polynomial(polynomial_t** this, polynomial_t* other);

int
pow_polynomial(polynomial_t** this, polynomial_t* lhs, polynomial_t* rhs);

int
convert_polynomial_to_power(polynomial_t* this, int64_t* power);

extern int yylex();

#endif // _POLYNOM_H

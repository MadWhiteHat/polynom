#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "polynom.h"
#include "utility.h"

// Static functions

int64_t
pow2(int64_t num) {
  int64_t res = 1;

  do { res *= 2; } while (res < num);

  return res;
} 

// Constructor
int
create_polynomial(
  polynomial_t** this,
  int64_t coef,
  char letter,
  int64_t power
) {
  int err = ERROR_SUCCESS;
  polynomial_t* polynomial = NULL;

  err = allocate_polynomial(&polynomial, power);
  if (FAILED(err)) { return err; }

  if (coef != 0) {
    polynomial->coefs.coefs[power] = coef;
    polynomial->coefs.size = power + 1;
    polynomial->letter = letter;
  } else {
    polynomial->coefs.size = 0;
    polynomial->letter = 0;
  }

  *this = polynomial;
  return ERROR_SUCCESS;
}

// Destructor
void
delete_polynomial(polynomial_t** this) {

  if (*this) {
    if ((*this)->coefs.coefs) { free((*this)->coefs.coefs); }
    free(*this);
  }

  *this = NULL;
}

// Copy constructor
int
copy_polynomial(polynomial_t** this, polynomial_t* other) {

  int err = ERROR_SUCCESS;
  polynomial_t* polynomial = NULL;

  err = is_valid_polynomial(other);
  if (FAILED(err)) { return err; }

  err = allocate_polynomial(&polynomial, other->coefs.size - 1);
  if (FAILED(err)) { return err; }

  for (int64_t i = 0; i < other->coefs.size; ++i) {
    polynomial->coefs.coefs[i] = other->coefs.coefs[i];
  }
  polynomial->coefs.size = other->coefs.size;
  polynomial->letter = other->letter;

  *this = polynomial;
  return ERROR_SUCCESS;
}

// Memory management
int
allocate_polynomial(polynomial_t** this, int64_t power) {

  if (power < 0) {
    print_error(SEMANTICS, "power must be a positive number");
    return ERROR_POWER_BELOW_ZERO;
  }

  polynomial_t* polynomial = (polynomial_t*)calloc(1, sizeof(polynomial_t));
  if (polynomial == NULL) { return ERROR_MEMORY_ALLOCATION; }

  int64_t req_cap = power + 1;
  int64_t new_cap = pow2(req_cap);

  polynomial->coefs.coefs = (int64_t*)calloc(new_cap, sizeof(int64_t));
  if (polynomial->coefs.coefs == NULL) {
    free(polynomial);
    polynomial = NULL;
    return ERROR_MEMORY_ALLOCATION;
  }

  polynomial->coefs.capacity = new_cap;

  *this = polynomial;
  return ERROR_SUCCESS;
}

int
shrink_to_fit_polynomial(polynomial_t* this) {

  int64_t new_cap = pow2(this->coefs.size);
  if (new_cap == this->coefs.capacity) { return ERROR_SUCCESS; }

  int64_t* new_coefs = (int64_t*)calloc(new_cap, sizeof(int64_t));
  if (new_coefs == NULL) { return ERROR_MEMORY_ALLOCATION; }

  memcpy(new_coefs, this->coefs.coefs, this->coefs.size * sizeof(int64_t));

  // Swap
  int64_t* tmp = this->coefs.coefs;
  this->coefs.coefs = new_coefs;
  new_coefs = tmp;

  this->coefs.capacity = new_cap;

  free(new_coefs);

  return ERROR_SUCCESS;
}

// Opertaions
int
is_valid_polynomial_operation(polynomial_t* lhs, polynomial_t* rhs) {
  int err = ERROR_SUCCESS;

  err = is_valid_polynomial(lhs);
  if (FAILED(err)) { return err; }

  err = is_valid_polynomial(rhs);
  if (FAILED(err)) { return err; }

  if (lhs->letter == 0 || rhs->letter == 0) { return ERROR_SUCCESS; }
  else if (lhs->letter != rhs->letter) {
    print_error(SEMANTICS, "Cannot perform operation between polynomials with"
      " different polynomial's variable");
    return ERROR_INVALID_OPERATION;
  }

  return ERROR_SUCCESS;
}

int
is_valid_polynomial(polynomial_t* this) {

  if (this == NULL || this->coefs.coefs == NULL) {
    print_error(SEMANTICS, "invalid polynomial");
    return ERROR_INVALID_POLYNOMIAL;
  }

  return ERROR_SUCCESS;
}

void
debug_polynomial(polynomial_t* this) {
  DEBUG_PRINT("ptr: %p\nlength: %ld\ncapacity: %ld\nletter: %c\n",
    (void*)polynomial->coefs.coefs, polynomial->coefs.size,
    polynomial->coefs.capacity, polynomial->letter);
  for (int64_t i = 0; i < this->coefs.capacity; ++i) {
    DEBUG_PRINT("%ld ", polynomial->coefs.coefs[i]);
  }
  DEBUG_PRINT("\n");
}

void
print_polynomial(polynomial_t* this) {

  if (this == NULL) {
    printf("Polynomial is NULL\n");
    return;
  }

  int64_t size = this->coefs.size;

  if (size == 0) {
    puts("0");
    return;
  } else if (size == 1) {
    printf("%ld", this->coefs.coefs[0]);
  } else if (size == 2) {
    if (this->coefs.coefs[1] == 1) {
      printf("%c", this->letter);
    } else if (this->coefs.coefs[1] == -1) {
      printf("-%c", this->letter);
    } else {
      printf("%ld%c", this->coefs.coefs[1], this->letter);
    }
    if (this->coefs.coefs[0] > 0) {
      printf("+%ld", this->coefs.coefs[0]);
    } else if (this->coefs.coefs[0] < 0) {
      printf("%ld", this->coefs.coefs[0]);
    }
  } else {
    if (this->coefs.coefs[size - 1] == 1) {
      printf("%c^%ld", this->letter, size - 1);
    } else if (this->coefs.coefs[size - 1] == -1) {
      printf("-%c^%ld", this->letter, size - 1);
    } else {
      printf("%ld%c^%ld", this->coefs.coefs[size - 1],
        this->letter, size - 1);
    }
    for (int64_t i = size - 1; i > 2; --i) {
      int64_t coef = this->coefs.coefs[i - 1];
      if (coef == 0 ) { continue; }
      else if (coef == 1) {
        printf("+%c^%ld", this->letter, i - 1);
      } else if (coef == -1) {
        printf("-%c^%ld", this->letter, i - 1);
      } else if (coef > 0) {
        printf("+%ld%c^%ld", coef, this->letter, i - 1);
      } else {
        printf("%ld%c^%ld", coef, this->letter, i - 1);
      }
    }
    if (this->coefs.coefs[1] == 1) {
      printf("+%c", this->letter);
    } else if (this->coefs.coefs[1] == -1) {
      printf("-%c", this->letter);
    } else if (this->coefs.coefs[1] > 0) {
      printf("+%ld%c", this->coefs.coefs[1], this->letter);
    } else if (this->coefs.coefs[1] < 0) {
      printf("%ld%c", this->coefs.coefs[1], this->letter);
    }
    if (this->coefs.coefs[0] > 0) {
      printf("+%ld", this->coefs.coefs[0]);
    } else if (this->coefs.coefs[0] < 0) {
      printf("%ld", this->coefs.coefs[0]);
    }
  }
  puts("");
}

int
sum_polynomials(
  polynomial_t** this,
  polynomial_t* lhs,
  polynomial_t* rhs,
  const char action
) {
  // Rhs with applied '-' sign if action = '-', otherwise neg_rhs = rhs;
  int err = ERROR_SUCCESS;
  polynomial_t* neg_rhs = rhs;

  err = is_valid_polynomial_operation(lhs, rhs);
  if (FAILED(err)) { return err; }

  if (action == '-') {
     err = neg_polynomial(&neg_rhs, rhs);
     if (FAILED(err)) { return err; }
  }

  polynomial_t* min_polynomial = NULL;
  polynomial_t* max_polynomial = NULL;
  polynomial_t* res_polynomial = NULL;

  if (lhs->coefs.size >= neg_rhs->coefs.size) {
    max_polynomial = lhs;
    min_polynomial = neg_rhs;
  } else {
    max_polynomial = neg_rhs;
    min_polynomial = lhs;
  }

  err = copy_polynomial(&res_polynomial, max_polynomial);
  if (FAILED(err)) { return err; }

  for (int64_t i = 0; i < min_polynomial->coefs.size; ++i) {
    res_polynomial->coefs.coefs[i] += min_polynomial->coefs.coefs[i];
  }

  int64_t new_size = res_polynomial->coefs.size;
  while (new_size) {
    if (res_polynomial->coefs.coefs[new_size - 1] != 0) { break; }
    --new_size;
  }

  // Check if new_size is not zero
  res_polynomial->coefs.size = new_size ? new_size : 1;

  err = shrink_to_fit_polynomial(res_polynomial);
  if (FAILED(err)) { return err; }

  // Cleanup neg_rhs if action is '-'
  if(neg_rhs != rhs) { delete_polynomial(&neg_rhs); }

  *this = res_polynomial;
  return ERROR_SUCCESS;
}

int
mul_polynomials(polynomial_t** this, polynomial_t* lhs, polynomial_t* rhs) {

  int err = ERROR_SUCCESS;
  int64_t max_cap = MAX(lhs->coefs.capacity, rhs->coefs.capacity);
  int64_t max_power = max_cap - 1;

  polynomial_t* tmp_lhs = lhs;
  polynomial_t* tmp_rhs = rhs;
  polynomial_t* res_polynomial = NULL;

  err = is_valid_polynomial_operation(lhs, rhs);
  if (FAILED(err)) { return err; }

  if (lhs->coefs.capacity != max_cap) {
    err = allocate_polynomial(&tmp_lhs, max_power);
    if (FAILED(err)) { return err; }

    memcpy(
      tmp_lhs->coefs.coefs,
      lhs->coefs.coefs,
      lhs->coefs.size * sizeof(int64_t)
    );
    tmp_lhs->letter = lhs->letter;
    tmp_lhs->coefs.size = lhs->coefs.size;

  } else if (rhs->coefs.capacity != max_cap) {
    err = allocate_polynomial(&tmp_rhs, max_power);
    if (FAILED(err)) { return err; }

    memcpy(
      tmp_rhs->coefs.coefs,
      rhs->coefs.coefs,
      rhs->coefs.size * sizeof(int64_t)
    );
    tmp_rhs->letter = rhs->letter;
    tmp_rhs->coefs.size = rhs->coefs.size;
  }

  err = allocate_polynomial(&res_polynomial, 2 * max_cap - 1);
  if (FAILED(err)) { return err; }

  err = karatsuba(
    tmp_lhs->coefs.coefs,
    tmp_rhs->coefs.coefs,
    res_polynomial->coefs.coefs,
    max_cap
  );
  if (FAILED(err)) { return err; }

  int64_t power = res_polynomial->coefs.capacity - 1;

  while (res_polynomial->coefs.coefs[power] == 0) { --power; }

  res_polynomial->coefs.size = power + 1;
  res_polynomial->letter = (lhs->letter != 0) ? lhs->letter : rhs->letter;

  err = shrink_to_fit_polynomial(res_polynomial);
  if (FAILED(err)) { return err; }

  if (tmp_lhs != lhs) { delete_polynomial(&tmp_lhs); }
  if (tmp_rhs != rhs) { delete_polynomial(&tmp_rhs); }

  *this = res_polynomial;
  return ERROR_SUCCESS;
}

int
karatsuba(int64_t* coefs1, int64_t* coefs2, int64_t* res_coefs, int64_t size) {
  int err = ERROR_SUCCESS;

  if (size <= 64) {
    for (int64_t i = 0; i < size; ++i) {
      for (int64_t j = 0; j < size; ++j) {
        res_coefs[i + j] += coefs1[i] * coefs2[j];
      }
    }
  } else {
    int64_t k =  size / 2;
    int64_t* l = (int64_t*)calloc(k, sizeof(int64_t));
    int64_t* r = (int64_t*)calloc(k, sizeof(int64_t));
    int64_t* t = (int64_t*)calloc(size, sizeof(int64_t));

    if (l == NULL || r == NULL || t == NULL) {
      // free(NULL) is OK according to C standard, so..
      free(l);
      free(r);
      free(t);
      return ERROR_MEMORY_ALLOCATION;
    }

    for (int64_t i = 0; i < k; ++i) {
      l[i] = coefs1[i] + coefs1[i + k];
      r[i] = coefs2[i] + coefs2[i + k];
    }

    err = karatsuba(l, r, t, k);
    if (FAILED(err)) {
      free(l);
      free(r);
      free(t);
      return err;
    }
    err = karatsuba(coefs1, coefs2, res_coefs, k);
    if (FAILED(err)) {
      free(l);
      free(r);
      free(t);
      return err;
    }
    err = karatsuba(coefs1 + k, coefs2 + k, res_coefs + size, k);
    if (FAILED(err)) {
      free(l);
      free(r);
      free(t);
      return err;
    }

    int64_t* t1 = t;
    int64_t* t2 = t + k;
    int64_t* s1 = res_coefs;
    int64_t* s2 = res_coefs + k;
    int64_t* s3 = res_coefs + 2 * k;
    int64_t* s4 = res_coefs + 3 * k;

    for (int64_t i = 0; i < k; ++i) {
      int64_t c1 = s2[i] + t1[i] - s1[i] - s3[i];
      int64_t c2 = s3[i] + t2[i] - s2[i] - s4[i];
      res_coefs[k + i] = c1;
      res_coefs[size + i] = c2;
    }

    free(l);
    free(r);
    free(t);
  }

  return ERROR_SUCCESS;
}

int
neg_polynomial(polynomial_t** this, polynomial_t* other) {
  int err = ERROR_SUCCESS;
  polynomial_t* res_polynomial = NULL;

  err = copy_polynomial(&res_polynomial, other);
  if (FAILED(err)) { return err; }

  for (int64_t i = 0; i < res_polynomial->coefs.size; ++i) {
    res_polynomial->coefs.coefs[i] = -res_polynomial->coefs.coefs[i];
  }

  *this = res_polynomial;
  return ERROR_SUCCESS;;
}

int
pow_polynomial(polynomial_t** this, polynomial_t* lhs, polynomial_t* rhs) {
  int err = ERROR_SUCCESS;
  polynomial_t* res_polynomial = NULL;
  polynomial_t* tmp = NULL;
  int64_t power = 0;

  err = convert_polynomial_to_power(rhs, &power);
  if (FAILED(err)) { return err; }

  if (power < 0) {
    print_error(SEMANTICS, "power must be postive number");
    return ERROR_POWER_BELOW_ZERO;
  }

  if (power == 0) {
    if (lhs->coefs.size == 0) {
      print_error(SEMANTICS, "undefined result of 0^0");
      return ERROR_UNDEFINED_BEHAVIOR;
    }
    else {
      err = create_polynomial(&res_polynomial, 1, 0, 0);
      *this = res_polynomial;
      return err;
    }
  }

  err = copy_polynomial(&res_polynomial, lhs);
  if (FAILED(err)) { return err; }

  if (power == 1) {
    *this = res_polynomial;
    return ERROR_SUCCESS;
  }

  int64_t tmp_power = power;
  int64_t power_size = 0;

  for (;tmp_power > 1; tmp_power >>= 1) { ++power_size; }

  for (int64_t i = power_size - 1; i >= 0; --i) {
    tmp = res_polynomial;

    err = mul_polynomials(&res_polynomial, tmp, tmp);
    if (FAILED(err)) { return err; }

    delete_polynomial(&tmp);

    if (power & 1 << i) {
      tmp = res_polynomial;
      err = mul_polynomials(&res_polynomial, tmp, lhs);
      delete_polynomial(&tmp);
    }
  }

  *this = res_polynomial;
  return ERROR_SUCCESS;
}

int
convert_polynomial_to_power(polynomial_t* this, int64_t* power) {

  if (this->coefs.size > 1) {
    print_error(SEMANTICS, "Cannot power polynomial to polynomial");
    return ERROR_POLYNOMIAL_POWER;
  } else if (this->coefs.size == 0) {
    return 0;
  }

  *power = this->coefs.coefs[0];
  return ERROR_SUCCESS;
}

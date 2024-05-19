#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "polynom.h"

// Static functions

void
debug_polynomial(polynomial_t* polynomial) {
  printf("ptr: %p\nlength: %ld\ncapacity: %ld\nletter: %c\n",
    (void*)polynomial->coefs.coefs, polynomial->coefs.size,
    polynomial->coefs.capacity, polynomial->letter);
  for (int64_t i = 0; i < polynomial->coefs.capacity; ++i) {
    printf("%ld ", polynomial->coefs.coefs[i]);
  }
  puts("");
}

int64_t
pow2(int64_t num) {
  int64_t res = 1;

  do { res *= 2; } while (res < num);

  return res;
} 

// Constructor
polynomial_t*
create_polynomial(int64_t coef, char letter, int64_t power) {
  polynomial_t* polynomial = allocate_polynomial(power);
  if (polynomial == NULL) { return NULL; }
  if (coef != 0) {
    polynomial->coefs.coefs[power] = coef;
    polynomial->coefs.size = power + 1;
    polynomial->letter = letter;
  } else {
    polynomial->coefs.size = 0;
    polynomial->letter = 0;
  }

  return polynomial;
}

// Destructor
void
delete_polynomial(polynomial_t* polynomial) {
  if (polynomial) {
    if (polynomial->coefs.coefs) { free(polynomial->coefs.coefs); }
    free(polynomial);
  }
}

// Copy constructor
polynomial_t*
copy_polynomial(polynomial_t* polynomial) {
  polynomial_t* res_polynomial =
    allocate_polynomial(polynomial->coefs.size - 1);
  for (int64_t i = 0; i < polynomial->coefs.size; ++i) {
    res_polynomial->coefs.coefs[i] = polynomial->coefs.coefs[i];
  }
  res_polynomial->coefs.size = polynomial->coefs.size;
  res_polynomial->letter = polynomial->letter;

  return res_polynomial;
}

// Memory management
polynomial_t*
allocate_polynomial(int64_t power) {
  if (power < 0) {
    yyerror("Power must be postive number");
    exit(-1);
  }

  polynomial_t* polynomial = (polynomial_t*)calloc(1, sizeof(polynomial_t));
  if (polynomial == NULL) { return NULL; }

  int64_t req_cap = power + 1;
  int64_t new_cap = pow2(req_cap);
  polynomial->coefs.coefs = (int64_t*)calloc(new_cap, sizeof(int64_t));

  if (polynomial->coefs.coefs == NULL) {
    free(polynomial);
    polynomial = NULL;
  }
  polynomial->coefs.capacity = new_cap;

  return polynomial;
}

void
shrink_to_fit_polynomial(polynomial_t* polynomial) {
  int64_t new_cap = pow2(polynomial->coefs.size);
  if (new_cap == polynomial->coefs.capacity) { return; }

  int64_t* new_coefs = (int64_t*)calloc(new_cap, sizeof(int64_t));
  if (new_coefs == NULL) { return; }
  memcpy(new_coefs, polynomial->coefs.coefs,
    polynomial->coefs.size * sizeof(int64_t));

  int64_t* tmp = polynomial->coefs.coefs;
  polynomial->coefs.coefs = new_coefs;
  new_coefs = tmp;

  polynomial->coefs.capacity = new_cap;

  free(new_coefs);
}

// Opertaions
void
is_valid_polynomial_operation(polynomial_t* lhs, polynomial_t* rhs) {
  if (lhs == NULL || rhs == NULL) {
    yyerror("Invalid polynomial");
    exit(-1);
  } else if (lhs->letter == 0 || rhs->letter == 0) { return; }
  else if (lhs->letter != rhs->letter) {
    yyerror("Cannot perform operation between polynomials with different"
      " polynomial's variable");
    exit(-1);
  }
}

void
print_polynomial(polynomial_t* polynomial) {
  if (!polynomial) {
    printf("Polynomial is NULL\n");
    return;
  }
  int64_t size = polynomial->coefs.size;
  if (size == 0) {
    puts("0");
    return;
  } else if (size == 1) {
    printf("%ld", polynomial->coefs.coefs[0]);
  } else if (size == 2) {
    if (polynomial->coefs.coefs[1] == 1) {
      printf("%c", polynomial->letter);
    } else if (polynomial->coefs.coefs[1] == -1) {
      printf("-%c", polynomial->letter);
    } else {
      printf("%ld%c", polynomial->coefs.coefs[1], polynomial->letter);
    }
    if (polynomial->coefs.coefs[0] > 0) {
      printf("+%ld", polynomial->coefs.coefs[0]);
    } else if (polynomial->coefs.coefs[0] < 0) {
      printf("%ld", polynomial->coefs.coefs[0]);
    }
  } else {
    if (polynomial->coefs.coefs[size - 1] == 1) {
      printf("%c^%ld", polynomial->letter, size - 1);
    } else if (polynomial->coefs.coefs[size - 1] == -1) {
      printf("-%c^%ld", polynomial->letter, size - 1);
    } else {
      printf("%ld%c^%ld", polynomial->coefs.coefs[size - 1],
        polynomial->letter, size - 1);
    }
    for (int64_t i = size - 1; i > 2; --i) {
      int64_t coef = polynomial->coefs.coefs[i - 1];
      if (coef == 0 ) { continue; }
      else if (coef == 1) {
        printf("+%c^%ld", polynomial->letter, i - 1);
      } else if (coef == -1) {
        printf("-%c^%ld", polynomial->letter, i - 1);
      } else if (coef > 0) {
        printf("+%ld%c^%ld", coef, polynomial->letter, i - 1);
      } else {
        printf("%ld%c^%ld", coef, polynomial->letter, i - 1);
      }
    }
    if (polynomial->coefs.coefs[1] == 1) {
      printf("+%c", polynomial->letter);
    } else if (polynomial->coefs.coefs[1] == -1) {
      printf("-%c", polynomial->letter);
    } else if (polynomial->coefs.coefs[1] > 0) {
      printf("+%ld%c", polynomial->coefs.coefs[1], polynomial->letter);
    } else if (polynomial->coefs.coefs[1] < 0) {
      printf("%ld%c", polynomial->coefs.coefs[1], polynomial->letter);
    }
    if (polynomial->coefs.coefs[0] > 0) {
      printf("+%ld", polynomial->coefs.coefs[0]);
    } else if (polynomial->coefs.coefs[0] < 0) {
      printf("%ld", polynomial->coefs.coefs[0]);
    }
  }
  puts("");
}

polynomial_t*
sum_polynomials(polynomial_t* lhs, polynomial_t* rhs, const char action) {
  // Rhs with applied '-' sign if action = '-', otherwise neg_rhs = rhs;
  polynomial_t* neg_rhs = rhs;
  if (action == '-') {
    neg_rhs = neg_polynomial(rhs);
  }

  polynomial_t* min_polynomial = NULL;
  polynomial_t* max_polynomial = NULL;
  polynomial_t* res = NULL;

  if (lhs->coefs.size >= neg_rhs->coefs.size) {
    max_polynomial = lhs;
    min_polynomial = neg_rhs;
  } else {
    max_polynomial = neg_rhs;
    min_polynomial = lhs;
  }

  res = copy_polynomial(max_polynomial);

  for (int64_t i = 0; i < min_polynomial->coefs.size; ++i) {
    res->coefs.coefs[i] += min_polynomial->coefs.coefs[i];
  }

  int64_t new_size = res->coefs.size;
  while (new_size) {
    if (res->coefs.coefs[new_size - 1] != 0) { break; } 
    --new_size;
  }
  if (new_size == 0) {
    free(res->coefs.coefs);
    res->coefs.coefs = NULL;
    res->coefs.capacity = 0;
  }
  res->coefs.size = new_size;

  // Cleanup neg_rhs if action is '-'
  if(neg_rhs != rhs) { delete_polynomial(neg_rhs); }

  return res;
}

polynomial_t*
mul_polynomials(polynomial_t* lhs, polynomial_t* rhs) {

  int64_t max_cap = MAX(lhs->coefs.capacity, rhs->coefs.capacity);
  int64_t max_power = max_cap - 1;

  polynomial_t* tmp_lhs = lhs;
  polynomial_t* tmp_rhs = rhs;
  polynomial_t* res = NULL;

  if (lhs->coefs.capacity != max_cap) {
    tmp_lhs = allocate_polynomial(max_power);
    memcpy(
      tmp_lhs->coefs.coefs,
      lhs->coefs.coefs,
      lhs->coefs.size * sizeof(int64_t)
    );
    tmp_lhs->letter = lhs->letter;
    tmp_lhs->coefs.size = lhs->coefs.size;
  } else if (rhs->coefs.capacity != max_cap) {
    tmp_rhs = allocate_polynomial(max_cap - 1);
    memcpy(
      tmp_rhs->coefs.coefs,
      rhs->coefs.coefs,
      rhs->coefs.size * sizeof(int64_t)
    );
    tmp_rhs->letter = rhs->letter;
    tmp_rhs->coefs.size = rhs->coefs.size;
  }

  res = allocate_polynomial(2 * max_cap - 1);

  karatsuba(
    tmp_lhs->coefs.coefs,
    tmp_rhs->coefs.coefs,
    res->coefs.coefs,
    max_cap
  );

  int64_t power = res->coefs.capacity - 1;
  while (res->coefs.coefs[power] == 0) { --power; }
  res->coefs.size = power + 1;
  res->letter = (lhs->letter != 0) ? lhs->letter : rhs->letter;

  shrink_to_fit_polynomial(res);

  if (tmp_lhs != lhs) { delete_polynomial(tmp_lhs); }
  if (tmp_rhs != rhs) { delete_polynomial(tmp_rhs); }

  return res;
}

void
karatsuba(int64_t* coefs1, int64_t* coefs2, int64_t* res_coefs, int64_t size) {
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
      return;
    }
    for (int64_t i = 0; i < k; ++i) {
      l[i] = coefs1[i] + coefs1[i + k];
      r[i] = coefs2[i] + coefs2[i + k];
    }
    karatsuba(l, r, t, k);
    karatsuba(coefs1, coefs2, res_coefs, k);
    karatsuba(coefs1 + k, coefs2 + k, res_coefs + size, k);

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
}

polynomial_t*
neg_polynomial(polynomial_t* polynomial) {
  polynomial_t* res = copy_polynomial(polynomial);
  for (int64_t i = 0; i < res->coefs.size; ++i) {
    res->coefs.coefs[i] = -res->coefs.coefs[i];
  }
  return res;
}

polynomial_t*
pow_polynomial(polynomial_t* polynomial, int64_t power) {
  polynomial_t* res = NULL;
  polynomial_t* tmp = NULL;

  if (power < 0) {
    yyerror("Power must be postive number");
    exit(-1);
  }

  res = copy_polynomial(polynomial);

  if (power == 1) { return res; }

  int64_t tmp_power = power;
  int64_t power_size = 0;

  for (;tmp_power > 1; tmp_power >>= 1) { ++power_size; }

  for (int64_t i = power_size - 1; i >= 0; --i) {
    tmp = res;
    res = mul_polynomials(tmp, tmp);
    delete_polynomial(tmp);

    if (power & 1 << i) {
      tmp = res;
      res = mul_polynomials(tmp, polynomial);
      delete_polynomial(tmp);
    }
  }

  return res;
}

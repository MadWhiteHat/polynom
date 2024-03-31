#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "polynom.h"

void debug_polynomial(polynomial_t* polynomial) {
  printf("ptr: %p\nlength: %ld\ncapacity: %ld\nletter: %c\n",
    polynomial->coefs.coefs, polynomial->coefs.size,
    polynomial->coefs.capacity, polynomial->letter);
  for (int64_t i = 0; i < polynomial->coefs.capacity; ++i) {
    printf("%ld ", polynomial->coefs.coefs[i]);
  }
  puts("");
}

int64_t pow2(int64_t num) {
  int64_t res = 1;

  do { res *= 2; } while (res < num);

  return res;
} 

void print_polynomial(polynomial_t* polynomial) {
  int64_t size = polynomial->coefs.size;
  if (size == 0) {
    puts("0");
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

polynomial_t* allocate_polynomial(int64_t power) {
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

polynomial_t* create_polynomial(
  int64_t coef,
  char letter,
  int64_t power
) {
  printf("Coef: %ld, Letter: %c, Power: %ld", coef, letter, power);
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

polynomial_t* neg_polynomial(polynomial_t* polynomial) {
  for (int64_t i = 0; i < polynomial->coefs.size; ++i) {
    polynomial->coefs.coefs[i] = -polynomial->coefs.coefs[i];
  }
  return polynomial;
}


polynomial_t* sum_polynomials(
  polynomial_t* polynomial1,
  polynomial_t* polynomial2,
  const char action
) {
  if (action == '-') { neg_polynomial(polynomial2); }

  polynomial_t* min_polynomial = NULL;
  polynomial_t* max_polynomial = NULL;
  
  if (polynomial1->coefs.size >= polynomial2->coefs.size) {
    max_polynomial = polynomial1;
    min_polynomial = polynomial2;
  } else {
    max_polynomial = polynomial2;
    min_polynomial = polynomial1;
  }

  for (int64_t i = 0; i < min_polynomial->coefs.size; ++i) {
    max_polynomial->coefs.coefs[i] += min_polynomial->coefs.coefs[i];
  }

  int64_t new_size = max_polynomial->coefs.size;
  while (new_size) {
    if (max_polynomial->coefs.coefs[new_size - 1] != 0) { break; } 
    --new_size;
  }
  if (new_size == 0) {
    free(max_polynomial->coefs.coefs);
    max_polynomial->coefs.coefs = NULL;
    max_polynomial->coefs.capacity = 0;
  }
  max_polynomial->coefs.size = new_size;

  deallocate_polynomial(min_polynomial);
  return max_polynomial;
}

polynomial_t* mul_polynomials(
  polynomial_t* polynomial1,
  polynomial_t* polynomial2) {

  //printf("\n\n");
  //printf("Polynomial1:\n");
  //debug_polynomial(polynomial1);
  //printf("Polynomial2:\n");
  //debug_polynomial(polynomial2);
  //printf("\n\n");

  int64_t max_cap =
    MAX(polynomial1->coefs.capacity, polynomial2->coefs.capacity);
  int64_t max_power = max_cap - 1;

  //printf("MAX_CAP: %ld\n", max_cap);

  if (polynomial1->coefs.capacity != max_cap) {
    polynomial_t* tmp = allocate_polynomial(max_power);
    memcpy(
      tmp->coefs.coefs,
      polynomial1->coefs.coefs,
      polynomial1->coefs.size * sizeof(int64_t)
    );
    tmp->letter = polynomial1->letter;
    tmp->coefs.size = polynomial1->coefs.size;
    
    polynomial_t* swap_tmp = polynomial1;
    polynomial1 = tmp;
    tmp = swap_tmp;

    deallocate_polynomial(tmp);
  } else if (polynomial2->coefs.capacity != max_cap) {

    polynomial_t* tmp = allocate_polynomial(max_cap - 1);
    memcpy(
      tmp->coefs.coefs,
      polynomial2->coefs.coefs,
      polynomial2->coefs.size * sizeof(int64_t)
    );
    tmp->letter = polynomial2->letter;
    tmp->coefs.size = polynomial2->coefs.size;
    
    polynomial_t* swap_tmp = polynomial2;
    polynomial2 = tmp;
    tmp = swap_tmp;

    deallocate_polynomial(tmp);
  }

  polynomial_t* res_polynomial = allocate_polynomial(2 * max_cap - 1);

  karatsuba(
    polynomial1->coefs.coefs,
    polynomial2->coefs.coefs,
    res_polynomial->coefs.coefs,
    max_cap
  );

  int64_t power = res_polynomial->coefs.capacity - 1;
  while (res_polynomial->coefs.coefs[power] == 0) { --power; }
  res_polynomial->coefs.size = power + 1;
  res_polynomial->letter = (polynomial1->letter != 0) ?
    polynomial1->letter : polynomial2->letter;

  shrink_to_fit_polynomial(res_polynomial);

  //printf("\n\n");
  //printf("Polynomial1:\n");
  //debug_polynomial(polynomial1);
  //printf("Polynomial2:\n");
  //debug_polynomial(polynomial2);
  //printf("Result:\n");
  //debug_polynomial(res_polynomial);
  //printf("\n\n");

  if (polynomial1 != polynomial2) {
    deallocate_polynomial(polynomial1);
    deallocate_polynomial(polynomial2);
  } else {
    deallocate_polynomial(polynomial1);
  }

  return res_polynomial;
}

void karatsuba(
  int64_t* coefs1,
  int64_t* coefs2,
  int64_t* res_coefs,
  int64_t size) {
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

polynomial_t* copy_polynomial(polynomial_t* polynomial) {
  polynomial_t* res_polynomial =
    allocate_polynomial(polynomial->coefs.size - 1);
  for (int64_t i = 0; i < polynomial->coefs.size; ++i) {
    res_polynomial->coefs.coefs[i] = polynomial->coefs.coefs[i];
  }
  res_polynomial->coefs.size = polynomial->coefs.size;
  res_polynomial->letter = polynomial->letter;

  return res_polynomial;
}

polynomial_t* pow_polynomial(polynomial_t* polynomial, int64_t power) {
  if (power < 0) {
    yyerror("Power must be postive number");
    exit(-1);
  }
  if (power == 1) { return polynomial; }
  polynomial_t* copy = NULL;
  if (power % 2 == 1) {copy = copy_polynomial(polynomial); }
  for (; power != 1; power /= 2) {
    polynomial = mul_polynomials(polynomial, polynomial);
  }
  if (copy != NULL) {
    polynomial = mul_polynomials(polynomial, copy);
  }

  return polynomial;
}

void shrink_to_fit_polynomial(polynomial_t* polynomial) {
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

void deallocate_polynomial(polynomial_t* polynomial) {
  free(polynomial->coefs.coefs);
  free(polynomial);
}

void is_valid_operation(
  polynomial_t* polynomial1,
  polynomial_t* polynomial2
) {
  if (polynomial1->letter == 0 || polynomial2->letter == 0) { return; }
  else if (polynomial1->letter != polynomial2->letter) {
    yyerror("Cannot perform operation between polynomials with different"
      " polynomial's variable");
    exit(-1);
  }
}

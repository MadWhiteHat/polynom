#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "polynom.h"
#include "polynom.yacc.h"

void debug_polynomial(polynomial_t* __polynomial) {
  printf("Letter: %c\n", __polynomial->letter);
  printf("size: %ld, capacity: %ld\n", __polynomial->coefs.size, __polynomial->coefs.capacity);
}

int64_t pow2(int64_t __num) {
  int64_t __res = 1;
  do {
    __res *= 2;
  } while (__res < __num);

  return __res;
} 

void print_polynomial(polynomial_t* __polynomial) {
  /*
  printf("ptr: %p\nlength: %ld\ncapacity: %ld\n", __polynomial->coefs.coefs,
      __polynomial->coefs.size, __polynomial->coefs.capacity);
  for (int64_t i = 0; i < __polynomial->coefs.size; ++i) {
    printf("%ld ", __polynomial->coefs.coefs[i]);
  }
  puts("");
  */
  int64_t __size = __polynomial->coefs.size;
  if (__size == 0) {
    puts("0");
  } else if (__size == 1) {
    printf("%ld", __polynomial->coefs.coefs[0]);
  } else if (__size == 2) {
    printf("%ld%c", __polynomial->coefs.coefs[1], __polynomial->letter);
    if (__polynomial->coefs.coefs[0] > 0) {
      printf("+%ld", __polynomial->coefs.coefs[0]);
    } else if (__polynomial->coefs.coefs[0] < 0) {
      printf("%ld", __polynomial->coefs.coefs[0]);
    }
  } else {
    printf("%ld%c^%ld", __polynomial->coefs.coefs[__size - 1],
      __polynomial->letter, __size - 1);
    for (int64_t i = __size - 1; i > 2; --i) {
      int64_t __coef = __polynomial->coefs.coefs[i - 1];
      if (__coef == 0 ) { continue; }
      if (__coef > 0) {
        printf("+%ld%c^%ld", __coef, __polynomial->letter, i - 1);
      } else {
        printf("%ld%c^%ld", __coef, __polynomial->letter, i - 1);
      }
    }
    if (__polynomial->coefs.coefs[1] > 0) {
      printf("+%ld%c", __polynomial->coefs.coefs[1], __polynomial->letter);
    } else if (__polynomial->coefs.coefs[1] < 0) {
      printf("%ld%c", __polynomial->coefs.coefs[1], __polynomial->letter);
    }
    if (__polynomial->coefs.coefs[0] > 0) {
      printf("+%ld", __polynomial->coefs.coefs[0]);
    } else if (__polynomial->coefs.coefs[0] < 0) {
      printf("%ld", __polynomial->coefs.coefs[0]);
    }
  }
  puts("");
}

polynomial_t* allocate_polynomial(int64_t __power) {
  if (__power < 0) {
    yyerror("Power must be postive number");
    exit(-1);
  }

  polynomial_t* __polynomial = calloc(1, sizeof(polynomial_t));
  if (__polynomial == NULL) { return NULL; }

  int64_t __req_cap = __power + 1;
  int64_t __new_cap = pow2(__req_cap);
  __polynomial->coefs.coefs = calloc(__new_cap, sizeof(int64_t));

  if (__polynomial->coefs.coefs == NULL) {
    free(__polynomial);
    __polynomial = NULL;
  }
  __polynomial->coefs.capacity = __new_cap;
  return __polynomial;
}

polynomial_t* init_polynomial(
  int64_t __coef,
  char __letter,
  int64_t __power
) {
  polynomial_t* __polynomial = allocate_polynomial(__power);
  if (__polynomial == NULL) { return NULL; }
  if (__coef != 0) {
    __polynomial->coefs.coefs[__power] = __coef;
    __polynomial->coefs.size = __power + 1;
    __polynomial->letter = __letter;
  } else {
    __polynomial->coefs.size = 0;
    __polynomial->letter = 0;
  }

  return __polynomial;
}

polynomial_t* neg_polynomial(polynomial_t* __polynomial) {
  for (int64_t i = 0; i < __polynomial->coefs.size; ++i) {
    __polynomial->coefs.coefs[i] = -__polynomial->coefs.coefs[i];
  }
  return __polynomial;
}


polynomial_t* sum_polynomials(
  polynomial_t* __polynomial1,
  polynomial_t* __polynomial2,
  const char __action
) {
  if (__action == '-') { neg_polynomial(__polynomial2); }

  polynomial_t* __min_polynomial = NULL;
  polynomial_t* __max_polynomial = NULL;
  
  if (__polynomial1->coefs.size >= __polynomial2->coefs.size) {
    __max_polynomial = __polynomial1;
    __min_polynomial = __polynomial2;
  } else {
    __max_polynomial = __polynomial2;
    __min_polynomial = __polynomial1;
  }

  for (int64_t i = 0; i < __min_polynomial->coefs.size; ++i) {
    __max_polynomial->coefs.coefs[i] += __min_polynomial->coefs.coefs[i];
  }

  int64_t __new_size = __max_polynomial->coefs.size;
  while (__new_size) {
    if (__max_polynomial->coefs.coefs[__new_size - 1] != 0) { break; } 
    --__new_size;
  }
  if (__new_size == 0) {
    free(__max_polynomial->coefs.coefs);
    __max_polynomial->coefs.coefs = NULL;
    __max_polynomial->coefs.capacity = 0;
  }
  __max_polynomial->coefs.size = __new_size;

  deallocate_polynomial(__min_polynomial);
  return __max_polynomial;
}

polynomial_t* mul_polynomials(
  polynomial_t* __polynomial1,
  polynomial_t* __polynomial2) {


  int64_t __max_len;
  if (__polynomial1->coefs.size == 1) {
    __max_len = __polynomial2->coefs.size;
  } else if (__polynomial2->coefs.size == 1) {
    __max_len = __polynomial1->coefs.size;
  } else {
    __max_len = __polynomial1->coefs.size + __polynomial2->coefs.size - 1;
  }
  polynomial_t* __res_polynomial = allocate_polynomial(__max_len - 1);

  karatsuba(
    __polynomial1->coefs.coefs,
    __polynomial2->coefs.coefs,
    __res_polynomial->coefs.coefs,
    __res_polynomial->coefs.capacity
  );

  __res_polynomial->coefs.size = __max_len;
  if (__polynomial1->letter != 0) {
    __res_polynomial->letter = __polynomial1->letter;
  } else {
    __res_polynomial->letter = __polynomial2->letter;
  }

  if (__polynomial1 != __polynomial2) {
    deallocate_polynomial(__polynomial1);
    deallocate_polynomial(__polynomial2);
  } else {
    deallocate_polynomial(__polynomial1);
  }

  return __res_polynomial;
}

void karatsuba(
  int64_t* __coefs1,
  int64_t* __coefs2,
  int64_t* __res_coefs,
  int64_t __size) {
  if (__size <= 64) {
    for (int64_t i = 0; i < __size; ++i) {
      for (int64_t j = 0; j < __size; ++j) {
        __res_coefs[i + j] += __coefs1[i] * __coefs2[j];
      }
    }
  } else {
    int64_t __k =  __size / 2;
    int64_t* __l = calloc(__k, sizeof(int64_t));
    int64_t* __r = calloc(__k, sizeof(int64_t));
    int64_t* __t = calloc(__size, sizeof(int64_t));
    if (__l == NULL || __r == NULL || __t == NULL) {
      return;
    }
    for (int64_t i = 0; i < __k; ++i) {
      __l[i] = __coefs1[i] + __coefs1[i + __k];
      __r[i] = __coefs2[i] + __coefs2[i + __k];
    }
    karatsuba(__l, __r, __t, __k);
    karatsuba(__coefs1, __coefs2, __res_coefs, __k);
    karatsuba(__coefs1 + __k, __coefs2 + __k, __res_coefs + __size, __k);

    int64_t* __t1 = __t;
    int64_t* __t2 = __t + __k;
    int64_t* __s1 = __res_coefs;
    int64_t* __s2 = __res_coefs + __k;
    int64_t* __s3 = __res_coefs + 2 * __k;
    int64_t* __s4 = __res_coefs + 3 * __k;

    for (int64_t i = 0; i < __k; ++i) {
      int64_t c1 = __s2[i] + __t1[i] - __s1[i] - __s3[i];
      int64_t c2 = __s3[i] + __t2[i] - __s2[i] - __s4[i];
      __res_coefs[__k + i] = c1;
      __res_coefs[__size + i] = c2;
    }

    free(__l);
    free(__r);
    free(__t);
  }
}

polynomial_t* copy_polynomial(polynomial_t* __polynomial) {
  polynomial_t* __res_polynomial =
    allocate_polynomial(__polynomial->coefs.size - 1);
  for (int64_t i = 0; i < __polynomial->coefs.size; ++i) {
    __res_polynomial->coefs.coefs[i] = __polynomial->coefs.coefs[i];
  }
  __res_polynomial->coefs.size = __polynomial->coefs.size;
  __res_polynomial->letter = __polynomial->letter;

  return __res_polynomial;
}
extern polynomial_t* pow_polynomial(polynomial_t* __polynomial, int64_t __power)
{
  if (__power < 0) {
    yyerror("Power must be postive number");
    exit(-1);
  }
  polynomial_t* __tmp = __polynomial;
  for (; __power >= 0; --__power) {
    __tmp = mul_polynomials(__tmp, __polynomial);
  }
  return __tmp;
}

void deallocate_polynomial(polynomial_t* __polynomial) {
  free(__polynomial->coefs.coefs);
  free(__polynomial);
}

void is_valid_operation(
  polynomial_t* __polynomial1,
  polynomial_t* __polynomial2
) {
  if (__polynomial1->letter == 0 || __polynomial2->letter == 0) { return; }
  else if (__polynomial1->letter != __polynomial2->letter) {
    yyerror("Cannot perform operation between polynomials with different"
      " polynomial's variable");
    exit(-1);
  }
}

void yyerror(const char* __msg) {
  printf("ERROR: %s\n", __msg);
}

extern FILE* yyin;

int32_t main(int32_t argc, const char** argv) {
  if (argc == 2) {
    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
      puts("Cannot open specified file");
      return 1;
    }
    yyparse();
    fclose(yyin);
  } else {
    yyin = stdin;
    yyparse();
  }
}

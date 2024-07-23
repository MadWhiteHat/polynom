#ifndef _UTILITY_H

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) \
  printf(fmt, __VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

#define ERROR_BUFFER_SIZE 0x0400
#define LEXICAL 0x00
#define SYNTAX 0x01
#define SEMANTICS 0x02

#define ERROR_SUCCESS 0x00
#define ERROR_FAILURE_STATE (0x01 << 31)
#define ERROR_POWER_BELOW_ZERO (0x01 | ERROR_FAILURE_STATE)
#define ERROR_MEMORY_ALLOCATION (0x02 | ERROR_FAILURE_STATE)
#define ERROR_INVALID_POLYNOMIAL (0x03 | ERROR_FAILURE_STATE)
#define ERROR_INVALID_OPERATION (0x04 | ERROR_FAILURE_STATE)
#define ERROR_UNDEFINED_BEHAVIOR (0x05 | ERROR_FAILURE_STATE)
#define ERROR_POLYNOMIAL_POWER (0x06 | ERROR_FAILURE_STATE)
#define ERROR_INVALID_VARIABLE (0x07 | ERROR_FAILURE_STATE)
#define ERROR_UNINITIALIZED_VARIABLE (0x08 | ERROR_FAILURE_STATE)
#define ERROR_RVALUE_ASSIGNMENT (0x09 | ERROR_FAILURE_STATE)
#define ERROR_INVALID_TREE (0x0a| ERROR_FAILURE_STATE)
#define ERROR_INVALID_VARIABLE_NAME (0x0b| ERROR_FAILURE_STATE)
#define ERROR_INVALID_SYNTAX (0x0c| ERROR_FAILURE_STATE)

#define FAILED(status) (status & ERROR_FAILURE_STATE)
#define SUCCESS(status) !FAILED(status)

// Current line number
extern int line;

// Current error code
extern int err;

void
yyerror(const char* msg);

void
print_error(int error_type, const char* fmt, ...);

#endif // _UTILITY_H

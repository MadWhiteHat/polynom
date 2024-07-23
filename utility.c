#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "utility.h"

int line = 1;
int err = ERROR_SUCCESS;

void
yyerror(const char* msg) { (void)(msg); }

void
print_error(int err_type, const char* fmt, ...) {
  va_list va_args;
  va_start(va_args, fmt);

  char buffer[ERROR_BUFFER_SIZE] = { 0x00 };
  size_t buffer_len;

  switch (err_type) {
    case LEXICAL:
      snprintf(buffer, ERROR_BUFFER_SIZE, "Lexical error in line %d: ", line);
      break;
    case SYNTAX:
      snprintf(buffer, ERROR_BUFFER_SIZE, "Syntax error in line %d: ", line);
      break;
    case SEMANTICS:
      snprintf(buffer, ERROR_BUFFER_SIZE, "Semantics error in line %d: ", line);
      break;
  }

  buffer_len = strlen(buffer);

  vsnprintf(buffer + buffer_len, ERROR_BUFFER_SIZE - buffer_len, fmt, va_args);

  va_end(va_args);

  puts(buffer);
}

#ifndef _ERRORS_H

#define ERROR_BUFFER_SIZE 1024
#define LEXICAL 0
#define SYNTAX 1
#define SEMANTICS 2

extern int line;

void
yyerror(const char* msg);

void
print_error(int error_type, const char* fmt, ...);

#endif // _ERRORS_H

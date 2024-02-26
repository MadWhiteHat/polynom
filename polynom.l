%{

#include <stdlib.h>
#include <stdint.h>

#include "variable_list.h"
#include "polynom.yacc.h"

// Buffer to convert input
char convert_buf[YY_BUF_SIZE] = { 0 };
YY_BUFFER_STATE buf_states[2] = { 0 };

// src = input buffer contatining: xxyyyxqe
// dest = output buffer contatining: (e^1)(q^1)(x^3)(y^3)
// size = size of output buffer
// returns the count of bytes in buffer contatining 2 triling
// YY_END_OF_BUFFER_CHAR; 0 = failure
yy_size_t convert_input(
  const char* src, yy_size_t src_size, char* dest, yy_size_t dest_size
);


%}

%array

%option noyywrap
%option outfile="polynom.lex.c" header-file="polynom.lex.h"

DIGIT [0-9]
VAR [a-z]
ID [_a-zA-Z0-9]+
SKIPPABLE [ \t\r]
VARS {VAR}{VAR}+

%x MULT_VARS

%%

<*>{SKIPPABLE}+           ; // whitespace
<*>{DIGIT}+               { ECHO; yylval.num = atoi(yytext); return NUMBER; }
<*>{VAR}                  { ECHO; yylval.letter = *yytext; return LETTER; }
{VAR}{VAR}+               {
                            if(yyleng == 4 && !strncmp(yytext, "exit", 4)) {
                              return EXIT;
                            }
                            if(yyleng == 5 && !strncmp(yytext, "print", 5)) {
                              return PRINT;
                            }
                            yy_size_t size = convert_input(
                              yytext, yyleng, convert_buf, YY_BUF_SIZE
                            );
                            if(size == 0) {
                              YY_FATAL_ERROR("failed to convert input");
                            }

                            buf_states[0] = YY_CURRENT_BUFFER;
                            buf_states[1] = yy_scan_buffer(
                              convert_buf, size
                            );
                            if(buf_states[1] == NULL) {
                              YY_FATAL_ERROR("failed to create buffer state");
                            }

                            yy_switch_to_buffer(buf_states[1]);
                            BEGIN(MULT_VARS);
                          }
\${ID}                    {
                            yylval.variable = create_variable(
                              yytext + 1,
                              yyleng - 1
                            );
                            BEGIN(INITIAL);
                            return VARIABLE;
                          }
<MULT_VARS><<EOF>>        {
                            yy_switch_to_buffer(buf_states[0]);
                            yy_delete_buffer(buf_states[1]);
                            BEGIN(INITIAL);
                          }
\n                        { return EOL; }
"print vars"              { return PRINT_VARS;  }
<*>[^\*\/\+\-\r\n\(= ]\(  {
                            char tmp = yytext[0];
                            unput('(');
                            unput('*');
                            unput(tmp);
                          }
<*>\)[^\*\/\+\-\^\r\n\)]  {
                            yyless(1);
                            unput('*');
                            unput(')');
                          }
<*>.                      { ECHO; return *yytext; }

%%

yy_size_t convert_input(
  const char* src, yy_size_t src_size, char* dest, yy_size_t dest_size
) {
  int64_t pows[26] = { 0 };
  char* tmp_buff = NULL;
  yy_size_t total = 0;

  if(src == NULL || src_size == 0 || dest == NULL || dest_size == 0) {
    return 0;
  }

  tmp_buff = (char*)malloc(dest_size);
  if(tmp_buff == NULL) { return 0; }

  for (yy_size_t i = 0; i < src_size; ++i) {
    ++pows[yytext[i] - 'a'];
  }

  for (int i = 0; i < 26; ++i) {
    if(pows[i] > 0 && total < dest_size - 1) {
      total += snprintf(
        tmp_buff + total, dest_size - total, "(%c^%lld)", i + 'a', pows[i]
      );
    }
  }

  if(total + 2 < dest_size) {
    tmp_buff[total++] = YY_END_OF_BUFFER_CHAR;
    tmp_buff[total++] = YY_END_OF_BUFFER_CHAR;
  } else { 
    free(tmp_buff);
    return 0;
  }

  memcpy(dest, tmp_buff, total);
  free(tmp_buff);

  return total;
}

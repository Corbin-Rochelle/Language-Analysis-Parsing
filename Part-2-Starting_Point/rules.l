/******************************************************************* 

Use corrected rules from your Project Part 1

Replace this header comment....

*******************************************************************/
%option yylineno
%option noyywrap
%{
#include "lexer.h"

%}

%%





 /* Ignore whitespace */

[ \n\t\r]+               /* nop */

 /* Found an unknown character */

.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }

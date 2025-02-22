%{
#include "parser.tab.h"  // Include parser declarations
#include <string.h>
#include <stdlib.h>

%}

%%

"strlen"|"strcpy"|"strcat"|"char"      { yylval.strval = strdup(yytext); return STRING; }
"sin"|"cos"|"sqrt"              { yylval.strval = strdup(yytext); return MATH;  }
"malloc"|"free"|"atoi"|"atoi"|"atof"|"strtol"          { yylval.strval = strdup(yytext); return STDLIB; }
"printf"|"scanf"                { yylval.strval = strdup(yytext); return STDIO; }
"isalpha"|"isdigit"|"toupper"|"tolower" { return CTYPE; }
"time"|"localtime"|"strftime"   { yylval.strval = strdup(yytext); return TIME; }
"fork"|"exec"|"getpid"          { yylval.strval = strdup(yytext); return UNISTD; }
"assert"                        { yylval.strval = strdup(yytext); return ASSERT; }
"open"|"fcntl"                  { yylval.strval = strdup(yytext); return FCNTL; }
"pthread_create"|"pthread_join" { yylval.strval = strdup(yytext); return PTHREAD; }
"errno"                         { yylval.strval = strdup(yytext); return ERRNO; }
"signal"|"raise"                { yylval.strval = strdup(yytext); return SIGNAL; }
.|[ \t\n]+                      { yylval.strval = strdup(yytext); return IDENTIFIER; }

%%

int yywrap() {
    return 1;
}

%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
%}

%%
"strlen"|"strcpy"|"strcat"      { return STRING; }
"sin"|"cos"|"sqrt"              { return MATH;  }
"malloc"|"free"|"atoi"          { return  STDLIB;  }
"printf"|"scanf"                { return STDIO; }
"isalpha"|"isdigit"|"toupper"|"tolower" {return CTYPE; }
"time"|"localtime"|"strftime"   { return TIME; }
"fork"|"exec"|"getpid"          { return UNISTD;  }
"assert"                        { return ASSERT;  }
"open"|"fcntl"                  { return FCNTL;  }
"pthread_create"|"pthread_join" { return PTHREAD; }
"errno"                         { return ERRNO;  }
"signal"|"raise"                { return SIGNAL; }
"component"                     { return COMPONENT; }
"render"                        { return RENDER; }
"return"                        { return RETURN; }
"("                             { return LPAREN; }
")"                             { return RPAREN; }
"<"                             {return LT;}
">"                             {return GT;}
"/"                             {return SLASH;}
"{"                             { return LBRACE; }
"}"                             { return RBRACE; }
","                             { return COMMA; }
":"                             { return COLON; }
[a-zA-Z_][a-zA-Z0-9_]*          { yylval.strval = strdup(yytext); return IDENTIFIER; }
.                               { /* Unexpected character */ }
[ \t\n]+ { /* ignore whitespace */ }


%%

int yywrap() {
    return 1;
}

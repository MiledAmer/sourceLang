%{
#include "parser.tab.h"
#include <string.h>

void store_string(char *text);
char prev_token[100] = "";
%}

%%
import                      { 
                            strcpy(prev_token, "import"); 
                            return IMPORT; 
                            }
"from"                     { return FROM; }
"component"                { return COMPONENT; }
"render"                   { return RENDER; }
"return"                   { return RETURN; }
"route"                    { return ROUTE; }
"get"                      { return GET; }
"const"                    { return CONST; }
"find"                     { return FIND; }
"findAll"                  { return FINDALL; }
"send"                     { return SEND; }
"json"                     { return JSON; }




[a-zA-Z_][a-zA-Z0-9_]*    { 
                            yylval.string = strdup(yytext);

                            if (strcmp(yytext, "db") == 0) {
                                if (strcmp(prev_token, "import") == 0) {
                                    return IDENTIFIER;
                                } else {
                                    return DB;
                                }
                            }

                            // Store the current token for future checks
                            strcpy(prev_token, yytext); 
                            return IDENTIFIER;
                          }
\"[^\"]*\"                 { 
                            yylval.string = strdup(yytext+1);
                            yylval.string[strlen(yylval.string)-1] = '\0';
                            return STRING; 
                          }

"{"                        { return LBRACE; }
"}"                        { return RBRACE; }
"("                        { return LPAREN; }
")"                        { return RPAREN; }
"<"                        { return LT; }
">"                        { return GT; }
"="                        { return EQUALS; }
"."                        { return DOT; }
","                        { return COMMA; }
";"                        { return SEMICOLON; }
"/"                        { return SLASH; }
":"                        { return COLON; }  /* Added COLON token */

[ \t\n]+                   ; /* Skip whitespace */
.                          { printf("Unexpected character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}
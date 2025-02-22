%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%option yylineno

%%

[0-9]+                                        { yylval.intVal = atoi(yytext); return INT_LITERAL; }
[0-9]*"."[0-9]+([eE][-+]?[0-9]+)?             { yylval.str = strdup(yytext); return FLOAT_LITERAL; }


"void"                { return VOID; }
"char"                { return CHAR; }
"short"               { return SHORT; }
"int"                 { return INT_TYPE; }
"long"                { return LONG; }
"float"               { return FLOAT; }
"double"              { return DOUBLE; }
"bool"                { return BOOL_TYPE; }
"unsigned"            { return UNSIGNED; }
"signed"              { return SIGNED; }
"struct"              { return STRUCT; }
"union"               { return UNION; }
"enum"                { return ENUM; }
"typedef"             { return TYPEDEF; }
"const"               { return CONST; }
"static"              { return STATIC; }
"extern"              { return EXTERN; }
"register"            { return REGISTER; }
"volatile"            { return VOLATILE; }
"function"            { return FUNCTION;}


"return"              { return RETURN; }
"true"                { return TRUE; }
"false"               { return FALSE; }


"("                  { return '('; }
")"                  { return ')'; }
"{"                  { return '{'; }
"}"                  { return '}'; }
"["                  { return '['; }
"]"                  { return ']'; }
","                  { return ','; }
":"                  { return ':'; }
";"                  { return ';'; }
"."                  { return '.'; }
"*"                  { return '*'; }
"="                  { return '='; }
"+"                  { return '+'; }
"-"                  { return '-'; }
"/"                  { return '/'; }
"%"                  { return '%'; }
"&&"                 { return AND_OP; }
"||"                 { return OR_OP; }
"!"                  { return '!'; }
"=="                 { return EQ_OP; }
"!="                 { return NE_OP; }
"<"                  { return '<'; }
">"                  { return '>'; }
"<="                 { return LE_OP; }
">="                 { return GE_OP; }

\"([^\\\"]|\\.)*\"          { yylval.str = strdup(yytext); return STRING_LITERAL; }


[a-zA-Z_][a-zA-Z0-9_]*      { yylval.str = strdup(yytext); return IDENTIFIER; }

"//".*                      { /* Ignore single-line comments */ }
"/*"[^*]*"*"*[^*/]*"*/"     { /* Ignore multi-line comments */ }

[ \t\r\n]+                  { /* Ignore whitespace */ }



.                           { return yytext[0]; }

%%

int yywrap() {
    return 1;
}

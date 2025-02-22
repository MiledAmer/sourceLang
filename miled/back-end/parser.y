%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;
extern char *yytext;

void yyerror(const char *s);
int yylex(void);
%}

%debug
%union {
    int intVal;
    float floatVal;
    char *str;
}

%token VOID CHAR SHORT INT_TYPE LONG FLOAT DOUBLE BOOL_TYPE UNSIGNED SIGNED STRUCT UNION ENUM TYPEDEF CONST 
%token STATIC EXTERN REGISTER VOLATILE FUNCTION RETURN TRUE FALSE AND_OP OR_OP EQ_OP NE_OP LE_OP GE_OP

%token <str> IDENTIFIER STRING_LITERAL
%token <intVal> INT_LITERAL
%token <floatVal> FLOAT_LITERAL

%%

program:
      program function_declaration
    | function_declaration
    | 
    ;

function_declaration:
      FUNCTION IDENTIFIER '(' params ')' ':' type  function_body
      {
        printf("Parsed function: %s\n", $2);
      }
    ;

params:
      /* empty */
    | param_list
    ;

param_list:
      param
    | param_list ',' param
    ;

param:
      IDENTIFIER ':' type
      {
          /* You can process parameter info here if needed */
      }
    ;

type:
      VOID
    | CHAR
    | SHORT
    | INT_TYPE
    | LONG
    | FLOAT
    | DOUBLE
    | BOOL_TYPE
    | UNSIGNED
    | SIGNED
    | ENUM
    | FUNCTION
    | IDENTIFIER

function_body:
      '{' statement_list '}'
    ;

statement_list:
      /* empty */
    | statement_list statement
    ;

statement:
      RETURN expression ';'
    | variable_declaration ';'
    | expression ';'
    ;

variable_declaration:
      type IDENTIFIER '=' expression
    ;

expression:
      IDENTIFIER
    | STRING_LITERAL
    | INT_LITERAL
    | FLOAT_LITERAL
    | TRUE
    | FALSE
    | function_call
    | IDENTIFIER '=' expression
    ;

function_call:
      IDENTIFIER '(' params ')'
    ;

%%

void yyerror(const char *s) {
    if (yytext && *yytext) {
        fprintf(stderr, "Syntax error: %s at line %d, near token '%s'\n", s, yylineno, yytext);
    } else {
        fprintf(stderr, "Syntax error: %s at line %d, near unexpected end of input\n", s, yylineno);
    }
}

int main(void) {
    if (yyparse() != 0) {
        fprintf(stderr, "Error during transpilation.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

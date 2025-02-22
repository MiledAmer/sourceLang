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
    char *str;
}

%token <str> IDENTIFIER STRING_LITERAL
%token <intVal> INT_LITERAL
%token RETURN VOID
%token USER_TYPE BOOL_TYPE INT_TYPE REQUEST_TYPE USERDATA_TYPE TRUE

%type <str> type_spec route_object route_method route_expr route_function_expr


%%

program:
      program element
    | element
    | 
    ;

element:
      function_declaration
    | route_registration
    ;

function_declaration:
      type_spec IDENTIFIER '(' param_list_opt ')' '{' function_body '}'
      {
          /* Generate corresponding C function signature and body */
          printf("Parsed function: %s returns %s\n", $2, $1);
      }
    ;

type_spec:
      USER_TYPE         { $$ = "User"; }
    | BOOL_TYPE         { $$ = "bool"; }
    | INT_TYPE          { $$ = "int"; }
    | REQUEST_TYPE      { $$ = "Request"; }
    | USERDATA_TYPE     { $$ = "UserData"; }
    | IDENTIFIER        { $$ = $1; }
    ;

param_list_opt:
      /* empty */
    | param_list
    ;

param_list:
      param
    | param_list ',' param
    ;

param:
      type_spec IDENTIFIER
      {
          /* You can process parameter info here if needed */
      }
    ;

function_body:
       body_content 
    ;

/* For MVP, simply consume tokens in the function body. 
   You can later replace this with a detailed grammar for statements. */

statement:
      RETURN TRUE ';'
    | IDENTIFIER '(' param_list_opt ')' ';'
    | ';'  /* empty statement */
    ;



body_content:
      /* empty */
    | body_content statement
    ;



/* --- Route Registration Parsing Rules --- */

/* A route registration is a complete expression ending with a semicolon */
route_registration:
      route_expr ';'
      ;

route_expr:
      route_object '.' route_method '(' STRING_LITERAL ',' route_function_expr ')'
      {
          /* $1: object (e.g. Route)
             $3: HTTP method (e.g. get, post)
             $5: route path (string literal)
             $7: function reference (identifier) */
          printf("Parsed route registration: %s.%s(\"%s\", %s)\n", $1, $3, $5, $7);
      }
      ;

/* The object that provides the route method */
route_object:
      IDENTIFIER
      ;

/* The HTTP method as an identifier (e.g. get, post) */
route_method:
      IDENTIFIER
      ;

/* The function reference may be either a plain identifier or a cast expression */
route_function_expr:
      IDENTIFIER
      { $$ = $1; }
    | '(' VOID ')' IDENTIFIER
      { $$ = $4; }
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

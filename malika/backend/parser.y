%{
#include <stdio.h>
#include <string.h>

extern int yylex();
void yyerror(const char *s);

#define YYDEBUG 1
char current_component[256];
%}

%union {
    int intval;   // For numeric values
    char* strval; // For strings like IDENTIFIER
}


%token COMPONENT LBRACE RBRACE LPAREN RPAREN LT GT COMMA COLON
%token <strval> IDENTIFIER
%type <strval> element parameters typed_param_list typed_param
%start program

%%

program:
      element
    ;

element:
      COMPONENT IDENTIFIER LPAREN parameters RPAREN LBRACE RBRACE
      { 
          /* $2 is the component name and $4 is the parameter list */
          printf("void render%s(%s) {}\n", $2, $4);
          free($4);
      }
    ;

parameters:
      /* empty */ { $$ = strdup(""); }
    | typed_param_list { $$ = $1; }
    ;

typed_param_list:
      typed_param 
      { $$ = $1; }
    | typed_param_list COMMA typed_param
      {
          /* Concatenate the previous list with ", " and the new parameter */
          char* tmp = malloc(strlen($1) + strlen($3) + 3); // extra space for comma, space, and '\0'
          sprintf(tmp, "%s, %s", $1, $3);
          free($1);
          $$ = tmp;
      }
    ;

typed_param:
      IDENTIFIER COLON IDENTIFIER
      {
          /* The DSL expects parameters as "var : type".
             In C, parameters are declared as "type var". 
             So $1 is the variable name and $3 is its type.
          */
          char* res = malloc(strlen($1) + strlen($3) + 2);
          sprintf(res, "%s %s", $3, $1);
          $$ = res;
      }
    ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}


int main() {
    yydebug = 1;
    int result = yyparse();
    
    if (result == 0) {
        return 0;
    } else {
        printf("Parsing failed\n");
    }
    return 0;
}   
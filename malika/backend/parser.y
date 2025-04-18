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


%token COMPONENT LBRACE RBRACE LPAREN RPAREN COLON TYPE SEMICOLON FROM IMPORT COMMA LT GT SLASH DOT LBRACKET RBRACKET RETURN EQUALS FUNCTION
%token <strval> IDENTIFIER STRING_LITERAL NUMBER_LITERAL BOOLEAN_LITERAL 
%type <strval> element parameters typed_param_list typed_param function_content list_function function program
%start program

%%

program:
      element
      |function
    ;

element:
      COMPONENT IDENTIFIER LPAREN parameters RPAREN LBRACE RBRACE
      { 
          /* $2 is the component name and $4 is the parameter list */
          printf("void %s(%s) {}\n", $2, $4);
          free($4);
      }
    ;

list_function:
    function{
        /* The first function in the list */
        $$ = strdup($1);
    }
    |list_function function
    { 
        /* Concatenate the previous list with the new function */
        char* tmp = malloc(strlen($1) + strlen($2) + 3); // extra space for comma, space, and '\0'
        sprintf(tmp, "%s\n %s", $1, $2);
        free($1);
        $$ = tmp;
    }


function:
    FUNCTION IDENTIFIER LPAREN parameters RPAREN COLON IDENTIFIER LBRACE function_content RBRACE
    {
        /* $2 is the function name and $4 is the parameter list */
        printf("%s %s(%s) {\n\t%s\n}\n",$7, $2, $4, $9);
        free($4);
    }
;

function_content:
    /* empty */ { $$ = strdup(""); }
    | IDENTIFIER function_content  
    {
        /* Just append any identifier to the content */
        char* tmp = malloc(strlen($1) + strlen($2) + 2);
        if (tmp) {
            sprintf(tmp, "%s %s", $1, $2);
            free($1);
            $$ = tmp;
        } else {
            $$ = $1;
        }
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
        printf( "✅ OK\n");
    } else {
        printf( "❌ Erreur\n ");
    }
    return 0;
}   
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


%token COMPONENT LBRACE RBRACE LPAREN RPAREN LT GT COMMA COLON SEMICOLON SLASH RETURN
%token <strval> IDENTIFIER
%type <strval> element parameters typed_param_list typed_param
%type <strval> function_body return_stmt
%type <strval> jsx_element jsx_children jsx_child
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

function_body:
      LBRACE return_stmt RBRACE
      { 
          /* For simplicity, we assume the only statement in the body is the return statement.
             You could extend this with additional statements if needed. */
          $$ = $2;
      }
    ;

return_stmt:
      RETURN jsx_element SEMICOLON
      { $$ = $2; }
    ;

jsx_element:
      LT IDENTIFIER GT jsx_children LT SLASH IDENTIFIER GT
      {
         /* Ensure that the opening and closing tags match */
         if(strcmp($2, $7) != 0)
             yyerror("Mismatched JSX tags");
         /* Produce a string representation of the JSX element.
            (This is a simplistic code-generation; you might want to build an AST instead.) */
         int len = strlen($2) + strlen($4) + strlen($7) + 10;
         $$ = malloc(len);
         sprintf($$, "<%s>%s</%s>", $2, $4, $7);
         free($4);
      }
    ;

jsx_children:
      /* empty */ { $$ = strdup(""); }
    | jsx_children jsx_child
      {
         char* tmp = malloc(strlen($1) + strlen($2) + 1);
         sprintf(tmp, "%s%s", $1, $2);
         free($1);
         $$ = tmp;
      }
    ;

jsx_child:
      IDENTIFIER { $$ = strdup($1); }
    | jsx_element { $$ = $1; }
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
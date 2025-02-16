%{
#include <stdio.h>
#include <string.h>

extern int yylex();
void yyerror(const char *s);

#define YYDEBUG 1
char current_component[256];
%}

%union {
    char *string;
}

%token IMPORT FROM COMPONENT RENDER RETURN ROUTE GET
%token CONST DB FIND FINDALL SEND JSON USERS
%token LBRACE RBRACE LPAREN RPAREN LT GT
%token EQUALS DOT COMMA SEMICOLON SLASH COLON  /* Added COLON token */
%token <string> IDENTIFIER STRING

%%

program
    : import_statement component_declaration route_statements
    ;

import_statement
    : IMPORT IDENTIFIER FROM STRING SEMICOLON
    ;

component_declaration
    : COMPONENT IDENTIFIER LBRACE render_method RBRACE
    {
        strcpy(current_component, $2);
        printf("// Component %s template\n", $2);
        printf("void render%s(char *buffer, const char *name, const char *email) {\n", $2);
        printf("    sprintf(buffer, \"<div>\\n\");\n");
        printf("    sprintf(buffer + strlen(buffer), \"  <h1>%%s</h1>\\n\", name);\n");
        printf("    sprintf(buffer + strlen(buffer), \"  <p>%%s</p>\\n\", email);\n");
        printf("    sprintf(buffer + strlen(buffer), \"</div>\");\n");
        printf("}\n\n");
    }
    ;

render_method
    : RENDER LPAREN IDENTIFIER RPAREN LBRACE
      RETURN jsx_element SEMICOLON
      RBRACE
    ;

jsx_element
    : LT IDENTIFIER GT jsx_content LT SLASH IDENTIFIER GT
    ;

jsx_content
    : jsx_child
    | jsx_content jsx_child
    ;

jsx_child
    : LT IDENTIFIER GT jsx_expression LT SLASH IDENTIFIER GT
    | LT IDENTIFIER GT IDENTIFIER DOT IDENTIFIER LT SLASH IDENTIFIER GT
    ;

jsx_expression
    : LBRACE IDENTIFIER DOT IDENTIFIER RBRACE
    ;

route_statements
    : route_statement
    | route_statements route_statement
    ;

route_statement
    : ROUTE STRING LBRACE route_handler RBRACE
    {
        if (strcmp($2, "/profile") == 0) {
            printf("void profileHandler(HttpRequest *req, HttpResponse *res) {\n");
            printf("    char buffer[1024];\n");
            printf("    User user = db_find(\"users\", \"id\", req->params[\"id\"]);\n");
            printf("    render%s(buffer, user.name, user.email);\n", current_component);
            printf("    http_send(res, buffer);\n");
            printf("}\n\n");
        } else if (strcmp($2, "/api/users") == 0) {
            printf("void apiUsersHandler(HttpRequest *req, HttpResponse *res) {\n");
            printf("    User *users = db_find_all(\"users\");\n");
            printf("    http_send_json(res, users);\n");
            printf("}\n\n");
        }
    }
    ;

route_handler
    : GET LPAREN IDENTIFIER COMMA IDENTIFIER RPAREN LBRACE
      route_body
      RBRACE
    ;

route_body
    : statements
    ;

statements
    : statement
    | statements statement
    ;

statement
    : const_declaration
    | response_statement
    ;

const_declaration
    : CONST IDENTIFIER EQUALS expression SEMICOLON
    ;

expression
    : IDENTIFIER
    | db_operation
    | member_expression
    ;

member_expression
    : IDENTIFIER DOT IDENTIFIER
    | member_expression DOT IDENTIFIER
    ;

db_operation
    : DB DOT FIND LPAREN STRING COMMA object_literal RPAREN
    | DB DOT FINDALL LPAREN STRING RPAREN
    ;

object_literal
    : LBRACE object_properties RBRACE
    ;

object_properties
    : IDENTIFIER COLON property_value
    ;

property_value
    : IDENTIFIER DOT IDENTIFIER DOT IDENTIFIER
    | IDENTIFIER
    ;


response_statement
    : IDENTIFIER DOT SEND LPAREN jsx_component RPAREN SEMICOLON
    | IDENTIFIER DOT JSON LPAREN IDENTIFIER RPAREN SEMICOLON
    ;

jsx_component
    : LT IDENTIFIER IDENTIFIER EQUALS LBRACE IDENTIFIER RBRACE SLASH GT
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}


int main() {
    printf("//starting parse");
    yydebug = 1;
    printf("Starting parse\n");
    int result = yyparse();
    
    if (result == 0) {
        printf("Parsing completed successfully\n");
    } else {
        printf("Parsing failed\n");
    }
    return 0;


}    
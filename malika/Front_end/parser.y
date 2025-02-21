%{
#include <stdio.h>
#include <string.h>

extern int yylex();
void yyerror(const char *s);

#define YYDEBUG 1
char identifiers[100][50];      // Stocke les identifiants rencontrés
char types[100][50];            // Stocke les types rencontrés
int id_count = 1;               // Compteur des identifiants
%}

%union {
    int intval;   // For numeric values
    char* strval; // For strings like IDENTIFIER
}


%token COMPONENT LBRACE RBRACE LPAREN RPAREN LT GT SLASH COMMA COLON
%token <strval> IDENTIFIER RENDER RETURN 
%type <strval> element parameters typed_param_list typed_param function html_content html_balise_open html_balise_close html_inner

%start program

%%

program:
      element
    ;

element:
      COMPONENT IDENTIFIER LPAREN parameters RPAREN LBRACE function RBRACE
      { 
          /* $2 is the component name and $4 is the parameter list */
          printf("void render%s(%s) {%s}\n", $2, $4,$7);
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
        
        strcpy(identifiers[id_count], $1);
        strcpy(types[id_count], $3); // Sauvegarde du type
        /* The DSL expects parameters as "var : type".
            In C, parameters are declared as "type var". 
            So $1 is the variable name and $3 is its type.
        */

        char* res = malloc(strlen($1) + strlen($3) + 2);
        sprintf(res, "%s %s", $3, $1);
        $$ = res;
      }
    ;

function:
    /* empty */ { $$ = strdup(""); }
    |RENDER LPAREN RPAREN LBRACE RETURN html_content RBRACE 
    { 
        char* tmp = malloc(strlen($6) + 50); // Allouer mémoire pour printf
        sprintf(tmp, "\n\tprintf(\"%s\"", $6);
        free($6);

        // Ajouter les arguments à printf
        if (id_count > 0) {
            strcat(tmp, ", ");
            for (int i = 0; i <= id_count; i++) {
                strcat(tmp, identifiers[i]);
                if (i < id_count - 1) strcat(tmp, ", ");
            }
        }
        strcat(tmp, ");\n");

        $$ = tmp;  // Retourner la chaîne générée
    }
    ;

html_content:
    html_balise_open html_content html_balise_close
    { 
        char* tmp = malloc(strlen($1) + strlen($2) + strlen($3) + 1);
        if (strcmp($1, $3) == 0){
            sprintf(tmp, "<%s> %s </%s>", $1, $2, $3);
            free($1);
            free($2);
            free($3);
            $$ = tmp;
        }
        else {
            yyerror("Balises non correspondantes");
            $$ = strdup("");
        }
    }
    |
    html_inner
    { 
        char* tmp = malloc( strlen($1) + 5);
        sprintf(tmp, "%s", $1);
        free($1);
        $$ = tmp;
        
    }
    ;

html_balise_open:
    LT IDENTIFIER GT
    {
        char* tmp = malloc(strlen($2) + 3); // "<tag>"
        sprintf(tmp, "%s", $2);
        free($2);
        $$ = tmp;
    }
    ;

html_balise_close:
    LT SLASH IDENTIFIER GT
    {
        char* tmp = malloc(strlen($3) + 4); // "</tag>"
        sprintf(tmp, "%s", $3);
        free($3);
        $$ = tmp;
    }
    ;

html_inner:
    IDENTIFIER
    { 
        // Trouver le type de l'identifiant
        char format[10] = "%s"; // Par défaut, on suppose une string
        for (int i = 0; i <= id_count; i++) { 
            if (strcmp(identifiers[i+1], $1) == 0) {
                if (strcmp(types[i+1], "int") == 0) {
                    strcpy(format, "%d");
                } else if (strcmp(types[i+1], "float") == 0) {
                    strcpy(format, "%f");
                }
                break;
            }
        }

        // Stocker l'identifiant
        strcpy(identifiers[id_count], $1);
        id_count++;
        // Retourner le bon placeholder
        $$ = strdup(format);
        free($1);
    }
    | LBRACE IDENTIFIER RBRACE 
    { 
       // Trouver le type de l'identifiant
        char format[10] = "%s"; // Par défaut, on suppose une string
        for (int i = 0; i < id_count; i++) {
            if (strcmp(identifiers[i+1], $2) == 0) {
                if (strcmp(types[i+1], "int") == 0) {
                    strcpy(format, "%d");
                } else if (strcmp(types[i+1], "float") == 0) {
                    strcpy(format, "%f");
                }
                break;
            }
        }

        // Stocker l'identifiant
        strcpy(identifiers[id_count], $2);
        id_count++;

        // Retourner le bon placeholder
        $$ = strdup(format);

        free($2);
    }
    | /* empty */
    { 
        $$ = strdup("");
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
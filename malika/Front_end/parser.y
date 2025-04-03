%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int yylex();
void yyerror(const char *s);

#define YYDEBUG 1
char identifiers[100][50];      // Stocke les identifiants rencontrés
char types[100][50];            // Stocke les types rencontrés
int id_count = 0;               // Compteur des identifiants
int found = 0;

char output_buffer[10000]; // Buffer pour le code généré
int buffer_index = 0; // Index pour le buffer

int needs_assert = 0;
int needs_complex = 0;
int needs_ctype = 0;
int needs_errno = 0;
int needs_fenv = 0;
int needs_float = 0;
int needs_inttypes = 0;
int needs_limits = 0;
int needs_locale = 0;
int needs_math = 0;
int needs_setjmp = 0;
int needs_signal = 0;
int needs_stdio = 0;
int needs_stdlib = 0;
int needs_string = 0;
int needs_time = 0;
int needs_wchar = 0;
int needs_wctype = 0;
int needs_tgmath = 0;
int needs_stddef = 0;
int needs_stdbool = 0;
int needs_stdarg = 0;
int needs_stdalign = 0;
int needs_iso646 = 0;
int needs_unistd = 0;
int needs_fcntl = 0;
int needs_threads = 0;

/* Structure de la pile pour les tags HTML */
typedef struct node_tag {
    char* tag_name;
    struct node_tag* next;
} node_tag;

node_tag* pile_tags = NULL;


// Fonction pour ajouter du texte au buffer
void append_to_buffer(const char *text) {
    snprintf(output_buffer + buffer_index, sizeof(output_buffer) - buffer_index, "%s", text);
    buffer_index += strlen(text);
}

// Fonction pour générer les includes
void generate_includes() {
    printf("/* Includes automatiques */\n");

    if (needs_assert) printf("#include <assert.h>\n");
    if (needs_complex) printf("#include <complex.h>\n");
    if (needs_ctype) printf("#include <ctype.h>\n");
    if (needs_errno) printf("#include <errno.h>\n");
    if (needs_fenv) printf("#include <fenv.h>\n");
    if (needs_float) printf("#include <float.h>\n");
    if (needs_inttypes) printf("#include <inttypes.h>\n");
    if (needs_limits) printf("#include <limits.h>\n");
    if (needs_locale) printf("#include <locale.h>\n");
    if (needs_math) printf("#include <math.h>\n");
    if (needs_setjmp) printf("#include <setjmp.h>\n");
    if (needs_signal) printf("#include <signal.h>\n");
    if (needs_stdio) printf("#include <stdio.h>\n");
    if (needs_stdlib) printf("#include <stdlib.h>\n");
    if (needs_string) printf("#include <string.h>\n");
    if (needs_threads) printf("#include <threads.h>\n");
    if (needs_time) printf("#include <time.h>\n");
    if (needs_wchar) printf("#include <wchar.h>\n");
    if (needs_wctype) printf("#include <wctype.h>\n");
    if (needs_tgmath) printf("#include <tgmath.h>\n");
    if (needs_stddef) printf("#include <stddef.h>\n");
    if (needs_stdbool) printf("#include <stdbool.h>\n");
    if (needs_stdarg) printf("#include <stdarg.h>\n");
    if (needs_stdalign) printf("#include <stdalign.h>\n");
    if (needs_iso646) printf("#include <iso646.h>\n");


    printf("\n"); // Space between includes and code
}

// Analyse des dépendances en fonction du contenu des identifiants et types
void analyze_dependencies() {
    // Dépendances essentielles pour le fonctionnement de base
    needs_stdio = 1;  // Pour printf
    needs_stdlib = 1; // Pour malloc/free
    needs_string = 1; // Pour manipulations de chaînes
    
    // Analyse des types pour détecter des dépendances spécifiques
    for (int i = 0; i < id_count; i++) {
        if (strcmp(types[i], "float") == 0 || strcmp(types[i], "double") == 0) {
            needs_math = 1;
        }
        else if (strcmp(types[i], "complex") == 0) {
            needs_complex = 1;
        }
        else if (strcmp(types[i], "bool") == 0) {
            needs_stdbool = 1;
        }
        else if (strcmp(types[i], "wchar_t") == 0) {
            needs_wchar = 1;
        }
        else if (strstr(types[i], "time") != NULL) {
            needs_time = 1;
        }
        else if (strstr(types[i], "int") != NULL) {
            needs_limits = 1; // Pour les limites de int
        }
    }
    
    // Analyse du contenu du buffer pour détecter d'autres dépendances
    if (strstr(output_buffer, "isalpha") != NULL || 
        strstr(output_buffer, "isdigit") != NULL || 
        strstr(output_buffer, "tolower") != NULL) {
        needs_ctype = 1;
    }
    
    if (strstr(output_buffer, "malloc") != NULL || 
        strstr(output_buffer, "free") != NULL || 
        strstr(output_buffer, "exit") != NULL) {
        needs_stdlib = 1;
    }
    
    if (strstr(output_buffer, "sin") != NULL || 
        strstr(output_buffer, "cos") != NULL || 
        strstr(output_buffer, "sqrt") != NULL) {
        needs_math = 1;
    }
    
    if (strstr(output_buffer, "printf") != NULL || 
        strstr(output_buffer, "scanf") != NULL || 
        strstr(output_buffer, "fprintf") != NULL) {
        needs_stdio = 1;
    }
    
    if (strstr(output_buffer, "strcpy") != NULL || 
        strstr(output_buffer, "strlen") != NULL || 
        strstr(output_buffer, "strcat") != NULL) {
        needs_string = 1;
    }
    
    if (strstr(output_buffer, "assert") != NULL) {
        needs_assert = 1;
    }
    
    if (strstr(output_buffer, "errno") != NULL) {
        needs_errno = 1;
    }
    
    if (strstr(output_buffer, "setjmp") != NULL || 
        strstr(output_buffer, "longjmp") != NULL) {
        needs_setjmp = 1;
    }
    
    if (strstr(output_buffer, "signal") != NULL) {
        needs_signal = 1;
    }
    
    if (strstr(output_buffer, "open") != NULL || 
        strstr(output_buffer, "close") != NULL || 
        strstr(output_buffer, "read") != NULL || 
        strstr(output_buffer, "write") != NULL) {
        needs_unistd = 1;
    }
    
    if (strstr(output_buffer, "O_RDONLY") != NULL || 
        strstr(output_buffer, "O_WRONLY") != NULL || 
        strstr(output_buffer, "O_CREAT") != NULL) {
        needs_fcntl = 1;
    }
    
    if (strstr(output_buffer, "thrd_") != NULL || 
        strstr(output_buffer, "mtx_") != NULL || 
        strstr(output_buffer, "cnd_") != NULL) {
        needs_threads = 1;
    }
}

/* Fonction pour empiler un tag */
void empiler_tag(char* tag_name) {
    node_tag* nouveau = (node_tag*)malloc(sizeof(node_tag));
    nouveau->tag_name = strdup(tag_name);  // Dupliquer la chaîne pour la stocker
    nouveau->next = pile_tags;
    pile_tags = nouveau;
    // printf("Tag empilé: %s\n", tag_name);  // Pour déboguer
}

/* Fonction pour dépiler et vérifier un tag */
int verifier_tag_fermant(char* tag_name) {
    if (pile_tags == NULL) {
        // printf("Erreur: balise fermante %s sans balise ouvrante correspondante\n", tag_name);
        return 0;
    }
    
    if (strcmp(pile_tags->tag_name, tag_name) == 0) {
        node_tag* tmp = pile_tags;
        pile_tags = pile_tags->next;
        free(tmp->tag_name);
        free(tmp);
        // printf("Tag vérifié et dépilé: %s\n", tag_name);  // Pour déboguer
        return 1;
    } else {
        // printf("Erreur: balise fermante %s ne correspond pas à la dernière balise ouvrante %s\n", 
            //    tag_name, pile_tags->tag_name);
        return 0;
    }
}

/* Fonction pour libérer la pile à la fin */
void liberer_pile() {
    while (pile_tags != NULL) {
        node_tag* tmp = pile_tags;
        pile_tags = pile_tags->next;
        free(tmp->tag_name);
        free(tmp);
    }
}
%}


%union {
    int intval;   // For numeric values
    char* strval; // For strings like IDENTIFIER
}


%token COMPONENT LBRACE RBRACE LPAREN RPAREN LT GT SLASH COMMA COLON
%token <strval> IDENTIFIER RENDER RETURN CLASSNAME DOUBLE_QUOTE EQUALS
%type <strval> element parameters typed_param_list typed_param function html_content html_balise_open html_balise_close html_inner html_balise_autoferme
%start program

%%

program:
      element
      {
        // Analyser les dépendances après le parsing
        analyze_dependencies();
        
        // Générer les includes en premier
        generate_includes();
        
        // Afficher le code généré
        printf("%s", output_buffer);

      }
    ;

element:
      COMPONENT IDENTIFIER LPAREN parameters RPAREN LBRACE function RBRACE
      { 
          /* $2 is the component name and $4 is the parameter list */
          char buffer[1000];
          sprintf(buffer, "void render%s(%s) {%s}\n", $2, $4, $7);
          append_to_buffer(buffer);
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
        id_count++;
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
            for (int i = 0; i < id_count; i++) {
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
        sprintf(tmp, "<%s> %s </%s>", $1, $2, $3);
        free($1);
        free($2);
        free($3);
        $$ = tmp;
    
    }
    |
    html_inner
    { 
        char* tmp = malloc( strlen($1) + 5);
        sprintf(tmp, "%s", $1);
        free($1);
        $$ = tmp;
        
    }
    |
    html_balise_autoferme html_content
    { 
        char* tmp = malloc(strlen($1) + strlen($2) + 1); // "<tag/>"
        sprintf(tmp, "<%s/> %s ", $1, $2);
        free($1);
        free($2);
        $$ = tmp;
    }
    ;

html_balise_autoferme:
    LT IDENTIFIER SLASH GT
    {
        char* tmp = malloc(strlen($2) + 3); // "<tag/>"
        sprintf(tmp, "%s", $2);
        free($2);
        $$ = tmp;
    }
    ;

html_balise_open:
    LT GT
    {
        char* tmp = malloc(1); // "<tag>"
        sprintf(tmp, "");
        empiler_tag("empty");
        $$ = tmp;
        
    }
    |
    LT IDENTIFIER GT
    {
        char* tmp = malloc(strlen($2) + 3); // "<tag>"
        sprintf(tmp, "%s", $2);
        
        // Empiler l'identifiant pour vérification ultérieure
        empiler_tag($2);
        
        free($2);
        $$ = tmp;
    }
    |
    LT IDENTIFIER CLASSNAME EQUALS DOUBLE_QUOTE IDENTIFIER DOUBLE_QUOTE GT
    {
        char* tmp = malloc(strlen($2) + strlen($6) + 15); //<tag className="..." >
        sprintf(tmp, "%s classname=\"%s\"", $2, $6);
        
        // Empiler l'identifiant pour vérification ultérieure
        empiler_tag($2);
        
        free($2);
        free($6);
        $$ = tmp;
    }
    ;
html_balise_close:
    LT SLASH GT
    {
        char* tmp = malloc(1); // "</>"
        sprintf(tmp, "");
        verifier_tag_fermant("empty");
        $$ = tmp;
    }
    |
    LT SLASH IDENTIFIER GT
    {
        char* tmp = malloc(1); // "</tag>"
        sprintf(tmp, "%s", $3);
        verifier_tag_fermant($3);
        free($3);
        $$ = tmp;
    }
    ;

html_inner:
    IDENTIFIER{
        char* tmp = malloc(strlen($1) + 1); 
        sprintf(tmp, "%s", $1);
        free($1);
        $$ = tmp;
    }
    |
    LBRACE IDENTIFIER RBRACE 
    { 
        found = 0;
        char format[10] = "%s";  // Format par défaut pour string
        // Vérifier si l'identifiant existe dans le tableau des identifiants
        for (int i = 0; i < id_count; i++) {
            if (strcmp(identifiers[i], $2) == 0) {
                found = 1;
                break;
            }
        }

        if (found==0) {
            char error_msg[100];
            sprintf(error_msg, "//Erreur : L'identifiant %s n'est pas un paramètre.", $2);
            append_to_buffer(error_msg);
            yyerror("Erreur : L'identifiant n'est pas un paramètre.");
            YYERROR;
        } else {
            // L'identifiant est trouvé, maintenant vérifier son type
            for (int i = 0; i < id_count; i++) { 
                if (strcmp(identifiers[i], $2) == 0) {
                    if (strcmp(types[i], "int") == 0) {
                        strcpy(format, "%d");
                    } else if (strcmp(types[i], "float") == 0) {
                        strcpy(format, "%f");
                    }
                    break;
                }
            }
        }

        // Return just the format specifier, not the variable name
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
    exit(1);  // Or set a global error flag
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
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
void yyerror(const char *s);

#define YYDEBUG 1

char output_buffer[10000];
char interfaces_buffer[1000];  // Buffer pour les interfaces TypeScript
int id_count = 0;
int found = 0;

typedef struct {
    char* name;
    char* type;
} TypedIdentifier;

TypedIdentifier identifiers[100];

void append_to_buffer(const char *str) {
    // Make sure we don't overflow the buffer
    size_t current_len = strlen(output_buffer);
    size_t str_len = strlen(str);
    
    if (current_len + str_len < sizeof(output_buffer) - 1) {
        strcat(output_buffer, str);
    } else {
        fprintf(stderr, "Warning: Output buffer overflow prevented\n");
    }
}

// Fonction pour générer un fichier HTML avec le contenu du buffer
void generate_html(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open file %s for writing\n", filename);
        return;
    }
    
    // Écrire l'en-tête HTML standard
    fprintf(file, "<!DOCTYPE html>\n");
    fprintf(file, "<html lang=\"en\">\n");
    fprintf(file, "<head>\n");
    fprintf(file, "    <meta charset=\"UTF-8\">\n");
    fprintf(file, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(file, "    <title>Generated Component</title>\n");
    fprintf(file, "</head>\n");
    fprintf(file, "<body>\n");
    
    // Écrire le contenu du buffer
    fprintf(file, "%s\n", output_buffer);
    
    // Fermer le document HTML
    fprintf(file, "</body>\n");
    fprintf(file, "</html>\n");
    
    fclose(file);
    printf("HTML file generated successfully: %s\n", filename);
}

// Fonction pour générer le code C avec la fonction generate_html
void generate_c_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open file %s for writing\n", filename);
        return;
    }
    
    // Écrire les en-têtes nécessaires
    fprintf(file, "#include <stdio.h>\n");
    fprintf(file, "#include <stdlib.h>\n");
    fprintf(file, "#include <string.h>\n\n");
    
    // Déclarer le buffer global
    fprintf(file, "char output_buffer[10000] = {\n");
    
    // Remplir le buffer avec le contenu HTML généré
    fprintf(file, "    \"");
    for (size_t i = 0; i < strlen(output_buffer); i++) {
        if (output_buffer[i] == '\n') {
            fprintf(file, "\\n\"\n    \"");
        } else if (output_buffer[i] == '"') {
            fprintf(file, "\\\"");
        } else if (output_buffer[i] == '\\') {
            fprintf(file, "\\\\");
        } else {
            fprintf(file, "%c", output_buffer[i]);
        }
    }
    fprintf(file, "\"\n};\n\n");
    
    // Écrire la fonction generate_html
    fprintf(file, "// Fonction pour générer un fichier HTML avec le contenu du buffer\n");
    fprintf(file, "void generate_html(const char *filename) {\n");
    fprintf(file, "    FILE *file = fopen(filename, \"w\");\n");
    fprintf(file, "    if (file == NULL) {\n");
    fprintf(file, "        fprintf(stderr, \"Error: Failed to open file %%s for writing\\n\", filename);\n");
    fprintf(file, "        return;\n");
    fprintf(file, "    }\n");
    fprintf(file, "    \n");
    fprintf(file, "    // Écrire l'en-tête HTML standard\n");
    fprintf(file, "    fprintf(file, \"<!DOCTYPE html>\\n\");\n");
    fprintf(file, "    fprintf(file, \"<html lang=\\\"en\\\">\\n\");\n");
    fprintf(file, "    fprintf(file, \"<head>\\n\");\n");
    fprintf(file, "    fprintf(file, \"    <meta charset=\\\"UTF-8\\\">\\n\");\n");
    fprintf(file, "    fprintf(file, \"    <meta name=\\\"viewport\\\" content=\\\"width=device-width, initial-scale=1.0\\\">\\n\");\n");
    fprintf(file, "    fprintf(file, \"    <title>Generated Component</title>\\n\");\n");
    fprintf(file, "    fprintf(file, \"</head>\\n\");\n");
    fprintf(file, "    fprintf(file, \"<body>\\n\");\n");
    fprintf(file, "    \n");
    fprintf(file, "    // Écrire le contenu du buffer\n");
    fprintf(file, "    fprintf(file, \"%%s\\n\", output_buffer);\n");
    fprintf(file, "    \n");
    fprintf(file, "    // Fermer le document HTML\n");
    fprintf(file, "    fprintf(file, \"</body>\\n\");\n");
    fprintf(file, "    fprintf(file, \"</html>\\n\");\n");
    fprintf(file, "    \n");
    fprintf(file, "    fclose(file);\n");
    fprintf(file, "    printf(\"HTML file generated successfully: %%s\\n\", filename);\n");
    fprintf(file, "}\n\n");
    
    // Ajouter une fonction main pour tester
    fprintf(file, "int main(int argc, char *argv[]) {\n");
    fprintf(file, "    const char *output_file = (argc > 1) ? argv[1] : \"output.html\";\n");
    fprintf(file, "    generate_html(output_file);\n");
    fprintf(file, "    return 0;\n");
    fprintf(file, "}\n");
    
    fclose(file);
    printf("C file generated successfully: %s\n", filename);
}

// Fonction pour ajouter un identifiant et son type
void add_identifier(const char* name, const char* type) {
    identifiers[id_count].name = strdup(name);
    identifiers[id_count].type = strdup(type);
    id_count++;
}

// Fonction pour obtenir le type d'un identifiant
char* get_identifier_type(const char* name) {
    for (int i = 0; i < id_count; i++) {
        if (strcmp(identifiers[i].name, name) == 0) {
            return identifiers[i].type;
        }
    }
    return NULL;
}

// Fonction pour générer le code HTML pur selon le type
char* generate_html_code(const char* id_name) {
    char* id_type = get_identifier_type(id_name);
    char* buffer = malloc(1000);

    if (id_type != NULL) {
        if (strcmp(id_type, "number") == 0) {
            sprintf(buffer, "{%s}", id_name);
        } else if (strcmp(id_type, "boolean") == 0) {
            sprintf(buffer, "{%s}", id_name);
        } else if (strcmp(id_type, "string") == 0) {
            sprintf(buffer, "{%s}", id_name);
        } else {
            // Type complexe ou inconnu
            sprintf(buffer, "{%s}", id_name);
        }
    } else {
        // Type non trouvé, traite comme un littéral de texte
        sprintf(buffer, "{%s}", id_name);
    }
    
    return buffer;
}

void liberer_pile() {
    for (int i = 0; i < id_count; i++) {
        free(identifiers[i].name);
        free(identifiers[i].type);
    }
    id_count = 0;
}
%}

%union {
    int intval;
    char* strval;
}

%token COMPONENT RETURN EQUALS 
%token LBRACE RBRACE LPAREN RPAREN COLON TYPE SEMICOLON FROM IMPORT COMMA
%token LT GT SLASH
%token <strval> IDENTIFIER STRING_LITERAL
%type <strval> element parameters parameter function html_content html_inner attributes attribute html_element function_body
%type <strval> type_instruction type_properties type_property import_instruction return_instruction instructions instruction
%%

program:
    element {
        liberer_pile();
        // Output order: HTML code
        printf("%s\n", output_buffer);
        
        // Générer un fichier C avec la fonction generate_html
        generate_c_file("output.c");
    }
;

element:
    COMPONENT IDENTIFIER LPAREN parameters RPAREN function {
        // Create a buffer with sufficient space
        char buffer[10000] = {0};  // Initialize to zero
        
        // Reset the output buffer before processing this component
        output_buffer[0] = '\0';
        
        // Generate HTML component wrapper
        sprintf(buffer, "<div class='%s'>\n%s\n</div>", $2, $6);
        
        // Append to the global output buffer
        append_to_buffer(buffer);
        
        // Set the return value for this rule
        $$ = strdup(buffer);
        
        // Free allocated memory
        free($2);
        if ($6) free($6);
    }
    | /* empty */ { $$ = strdup(""); }  // Ajout d'une règle vide pour gérer le cas de fin de fichier
;

parameters:
    /* empty */ { $$ = strdup(""); }
    | parameter { $$ = $1; }
    | parameter COMMA parameters {
        char *tmp = malloc(strlen($1) + strlen($3) + 3);
        sprintf(tmp, "%s, %s", $1, $3);
        free($1); free($3);
        $$ = tmp;
    }
;

parameter:
    IDENTIFIER COLON IDENTIFIER {
        char* tmp = malloc(strlen($1) + strlen($3) + 3);
        add_identifier($1, $3);
        sprintf(tmp, "%s: %s", $1, $3);
        $$ = tmp;
        free($1); free($3);
    }
;

function:
    LBRACE function_body RBRACE {
        $$ = $2; // Simply pass the correctly formatted body up the parse tree
    }
;

function_body:
    /* empty */ { $$ = strdup(""); }
    | instructions {
        $$ = $1; // No need for additional processing, just pass up the instructions
    }
;

instructions:
    instruction { $$ = $1; }
    | instruction instructions {
        char *buffer = malloc(strlen($1) + strlen($2) + 2);
        sprintf(buffer, "%s\n%s", $1, $2);
        $$ = buffer;
        free($1); free($2);
    }
;

instruction:
    type_instruction {
        // Générer une instruction de type
        $$ =$1; // Store the type name
    }
    | import_instruction {
        // Générer une instruction d'import
        $$ = $1; // Store the imported component name
    }
    | return_instruction {
        // Générer une instruction de retour
        $$ = $1; // Store the return value
    }
;

type_instruction:
    TYPE IDENTIFIER EQUALS LBRACE type_properties RBRACE SEMICOLON {
        // Générer une instruction de type
        $$ = $2; // Store the type name
    }
;

type_properties:
    type_properties type_property {
        // Combine properties
        char *buffer = malloc(strlen($1) + strlen($2) + 2);
        sprintf(buffer, "%s%s", $1, $2);
        $$ = buffer;
        free($1); free($2);
    }
    | type_property { $$ = $1; }
;

type_property:
    IDENTIFIER COLON IDENTIFIER SEMICOLON { 
        // Propriété du type : <nom>: <type>
        // Return formatted property
        char *buffer = malloc(strlen($1) + strlen($3) + 10);
        sprintf(buffer, "%s: %s;\n", $1, $3);
        $$ = buffer;
        free($1); free($3);
    }
;

import_instruction:
    IMPORT IDENTIFIER FROM STRING_LITERAL SEMICOLON {
        // Générer une instruction d'import
        $$ = $2; // Store the imported component name
    }
;

return_instruction:
    RETURN LPAREN html_content RPAREN SEMICOLON {
        $$ = $3; // Store the HTML content
    }
;

html_content:
    /* empty */ { $$ = strdup(""); }
    | html_content html_element {
        char *buffer = malloc(strlen($1) + strlen($2) + 2);
        sprintf(buffer, "%s%s", $1, $2);
        $$ = buffer;
        free($1); 
        free($2);
    }
    | html_content html_inner {
        char *buffer = malloc(strlen($1) + strlen($2) + 2);
        sprintf(buffer, "%s%s", $1, $2);
        $$ = buffer;
        free($1); free($2);
    }
    | html_element {
        $$ = $1;
    }
;

html_element:
    LT IDENTIFIER attributes GT html_content LT SLASH IDENTIFIER GT {
        // Vérifier que les balises ouvrantes et fermantes correspondent
        if (strcmp($2, $8) != 0) {
            char error_msg[100];
            sprintf(error_msg, "Erreur: Les balises <%s> et </%s> ne correspondent pas", $2, $8);
            yyerror(error_msg);
            YYERROR;
        }
        
        size_t buffer_size = strlen($2) * 2 + strlen($3) + strlen($5) + 100;
        char *buffer = malloc(buffer_size);
        
        if (buffer == NULL) {
            yyerror("Memory allocation failed");
            YYERROR;
        }
        
        // Generate HTML code with the element tag
        sprintf(buffer, "<%s", $2);
        
        // Ajouter les attributs si présents
        if (strlen($3) > 0) {
            char *attr_buffer = malloc(strlen($3) + 1);
            strcpy(attr_buffer, $3);
            char *attr_token = strtok(attr_buffer, ";");
            
            while (attr_token != NULL) {
                char *attr_name = strtok(attr_token, "=");
                char *attr_value = strtok(NULL, "=");
                
                if (attr_name && attr_value) {
                    // Enlève les espaces de l'attribut
                    while (*attr_name == ' ') attr_name++;
                    while (*attr_value == ' ') attr_value++;
                    
                    // Ajoute l'attribut au buffer dans la balise ouverte
                    sprintf(buffer + strlen(buffer), " %s=%s", attr_name, attr_value);
                }
                
                attr_token = strtok(NULL, ";");
            }
            
            free(attr_buffer);
        }
        
        // Ajouter le contenu de l'élément HTML si nécessaire
        sprintf(buffer + strlen(buffer), ">%s</%s>", $5, $2);
        
        $$ = buffer;
        free($2); free($3); free($5); free($8); 
    }
    | LT IDENTIFIER attributes SLASH GT {
        char *buffer = malloc(1000);
        
        // Générer le code HTML pour un élément auto-fermant
        sprintf(buffer, "<%s", $2);  // Commence par la balise d'ouverture
        
        // Traitement des attributs similaire à ci-dessus
        if (strlen($3) > 0) {
            char *attr_buffer = malloc(strlen($3) + 1);
            strcpy(attr_buffer, $3);
            char *attr_token = strtok(attr_buffer, ";");
            
            while (attr_token != NULL) {
                char *attr_name = strtok(attr_token, "=");
                char *attr_value = strtok(NULL, "=");
                
                if (attr_name && attr_value) {
                    // Enlève les espaces
                    while (*attr_name == ' ') attr_name++;
                    while (*attr_value == ' ') attr_value++;
                    
                    // Ajoute l'attribut au buffer dans la balise ouverte
                    sprintf(buffer + strlen(buffer), " %s=%s", attr_name, attr_value);
                }
                
                attr_token = strtok(NULL, ";");
            }
            
            free(attr_buffer);
        }
        
        // Ajouter la balise auto-fermante
        sprintf(buffer + strlen(buffer), " />");
        
        $$ = buffer;
        free($2); free($3);
    }
    | html_inner {
        $$ = $1;
    }
;

attributes:
    /* empty */ { $$ = strdup(""); }
    | attribute attributes {
        char *buffer = malloc(strlen($1) + strlen($2) + 2);
        sprintf(buffer, "%s;%s", $1, $2);
        $$ = buffer;
        free($1); free($2);
    }
;

attribute:
    IDENTIFIER EQUALS STRING_LITERAL {
        char *buffer = malloc(strlen($1) + strlen($3) + 5);
        sprintf(buffer, "%s=%s", $1, $3);
        $$ = buffer;
        free($1); free($3);
    }
;

html_inner:
    IDENTIFIER {
        char* type = get_identifier_type($1);
        if (type != NULL) {
            // C'est un identifiant connu avec un type
            $$ = generate_html_code($1);
        } else {
            // C'est un littéral de texte
            char* tmp = malloc(strlen($1) + 100);
            sprintf(tmp, "%s", $1);
            $$ = tmp;
        }

        free($1);
    }
    | LBRACE IDENTIFIER RBRACE {
        char* type = get_identifier_type($2);
        if (type == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : L'identifiant %s n'est pas un paramètre.", $2);
            append_to_buffer(error_msg);
            yyerror(error_msg);
            YYERROR;
        } else {
            $$ = generate_html_code($2);
        }

        free($2);
    }
;

%%

void yyerror(const char *s) {
    extern char *yytext;  // yytext donne le token actuel
    fprintf(stderr, "Erreur de syntaxe : %s\n", s);
    fprintf(stderr, "Problème avec le token: '%s'\n", yytext);
    exit(1);
}

int main() {
    yydebug = 1;
    output_buffer[0] = '\0';
    return yyparse();
}
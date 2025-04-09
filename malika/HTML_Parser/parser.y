%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
void yyerror(const char *s);
extern FILE *yyin;  
extern void yyrestart(FILE* input_file);
#define YYDEBUG 1

char output_buffer[10000];
char interfaces_buffer[1000];  // Buffer pour les interfaces TypeScript
int id_count = 0;
int found = 0;

// Structure pour stocker les composants importés
typedef struct {
    char* name;
    char* html_content;
} ImportedComponent;

#define MAX_IMPORTED_COMPONENTS 50
ImportedComponent imported_components[MAX_IMPORTED_COMPONENTS];
int imported_count = 0;

// Structure pour les identifiants typés
typedef struct {
    char* name;
    char* type;
} TypedIdentifier;

TypedIdentifier identifiers[100];
char current_component_name[100] = ""; // Nom du composant en cours d'analyse

typedef struct {
    FILE* yyin_backup;
    char output_buffer_backup[10000];
    char current_component_name_backup[100];
    TypedIdentifier identifiers_backup[100];
    int id_count_backup;
} ParserState;

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

// Fonction pour libérer la mémoire des identifiants
void liberer_pile() {
    for (int i = 0; i < id_count; i++) {
        free(identifiers[i].name);
        free(identifiers[i].type);
    }
    id_count = 0;
}

// Fonction pour trouver un composant importé par son nom
char* find_imported_component(const char* name) {
    // printf("Recherche du composant importé: %s\n", name);
    for (int i = 0; i < imported_count; i++) {
        if (strcmp(imported_components[i].name, name) == 0) {
            // printf("Composant trouvé: %s\n", name);
            // printf("Contenu: %s\n", imported_components[i].html_content);
            return imported_components[i].html_content;
        }
    }
    return NULL;
}

// Fonction pour ajouter un composant importé
void add_imported_component(const char* name, const char* html_content) {
    // Vérifier si le composant existe déjà
    for (int i = 0; i < imported_count; i++) {
        if (strcmp(imported_components[i].name, name) == 0) {
            // Remplacer le contenu existant
            free(imported_components[i].html_content);
            imported_components[i].html_content = strdup(html_content);
            return;
        }
    }
    
    // Ajouter un nouveau composant
    if (imported_count < MAX_IMPORTED_COMPONENTS) {
        imported_components[imported_count].name = strdup(name);
        imported_components[imported_count].html_content = strdup(html_content);
        imported_count++;
    } else {
        fprintf(stderr, "Error: Maximum number of imported components reached\n");
    }
}
void process_import(char* component, char* path) {
    // Enlever les guillemets du chemin
    char real_path[256];
    strncpy(real_path, path + 1, strlen(path) - 2);  // Remove beginning and ending quotes
    real_path[strlen(path) - 2] = '\0';
    
    // Vérifier si le chemin est relatif et ajouter le ./ si nécessaire
    if (real_path[0] != '/' && strncmp(real_path, "./", 2) != 0) {
        char temp[256];
        sprintf(temp, "./%s", real_path);
        strcpy(real_path, temp);
    }
    
    // printf("Traitement de l'import du composant %s depuis %s\n", component, real_path);
    
    // Ouvrir le fichier source
    FILE* imported_file = fopen(real_path, "r");
    if (!imported_file) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", real_path);
        exit(1);
    }
    
    // Lire le contenu du fichier
    char buffer[10000] = {0};
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, imported_file);
    buffer[bytes_read] = '\0';  // Assurer que la chaîne est terminée par null
    
    // printf("Contenu brut du fichier importé:\n%s\n", buffer);
    
    // Analyser manuellement le contenu pour extraire le HTML du composant
    // Ceci est une version simplifiée qui suppose que le composant est correctement formaté
    char* start = strstr(buffer, "component");
    if (start) {
        start = strstr(start, "{");
        if (start) {
            char* return_stmt = strstr(start, "return");
            if (return_stmt) {
                char* open_paren = strstr(return_stmt, "(");
                if (open_paren) {
                    char* close_paren = strrchr(buffer, ')');  // Trouver la dernière parenthèse fermante
                    if (close_paren) {
                        // Extraire le contenu HTML entre les parenthèses de l'instruction return
                        int html_length = close_paren - (open_paren + 1);
                        if (html_length > 0 && html_length < 9000) {
                            char html_content[10000] = {0};
                            strncpy(html_content, open_paren + 1, html_length);
                            html_content[html_length] = '\0';
                            
                            // Créer un div englobant avec le nom du composant comme classe
                            char final_html[10000] = {0};
                            sprintf(final_html, "<div class='%s'>\n%s\n</div>", component, html_content);
                            
                            // Ajouter le composant importé à notre registre
                            add_imported_component(component, final_html);
                            // printf("Composant importé %s ajouté avec le contenu:\n%s\n", component, final_html);
                        }
                    }
                }
            }
        }
    }
    
    // Si l'analyse manuelle n'a pas fonctionné, créer un composant minimal
    if (find_imported_component(component) == NULL) {
        char minimal_html[1000];
        sprintf(minimal_html, "<div class='%s'><!-- Contenu du composant %s non analysé --></div>", 
                component, component);
        add_imported_component(component, minimal_html);
        // printf("Impossible d'analyser le composant %s. Utilisation d'un composant minimal.\n", component);
    }
    
    // Fermer le fichier
    fclose(imported_file);
    
    // printf("Import du composant %s terminé\n", component);
}

// Alternative: Utiliser une approche par système de commande externe
void process_import_external(char* component, char* path) {
    // Enlever les guillemets du chemin
    char real_path[256];
    strncpy(real_path, path + 1, strlen(path) - 2);
    real_path[strlen(path) - 2] = '\0';
    
    // Vérifier si le chemin est relatif
    if (real_path[0] != '/' && strncmp(real_path, "./", 2) != 0) {
        char temp[256];
        sprintf(temp, "./%s", real_path);
        strcpy(real_path, temp);
    }
    
    printf("Traitement externe du composant %s depuis %s\n", component, real_path);
    
    // Créer un fichier temporaire pour stocker le composant
    char temp_file[256];
    sprintf(temp_file, "/tmp/%s_component.html", component);
    
    // Lancer un processus externe pour traiter le fichier composant
    char command[1024];
    sprintf(command, "cp %s /tmp/temp_component.src && ./parser /tmp/temp_component.src > %s", 
            real_path, temp_file);
    
    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Erreur lors de l'exécution de la commande: %s\n", command);
        // Créer un composant minimal en cas d'échec
        char minimal_html[1000];
        sprintf(minimal_html, "<div class='%s'><!-- Échec du traitement du composant %s --></div>", 
                component, component);
        add_imported_component(component, minimal_html);
    } else {
        // Lire le fichier HTML généré
        FILE* html_file = fopen(temp_file, "r");
        if (html_file) {
            char html_content[10000] = {0};
            size_t bytes_read = fread(html_content, 1, sizeof(html_content) - 1, html_file);
            html_content[bytes_read] = '\0';
            
            // Extraire le contenu entre <body> et </body>
            char* body_start = strstr(html_content, "<body>");
            char* body_end = strstr(html_content, "</body>");
            
            if (body_start && body_end) {
                body_start += 6;  // Passer après <body>
                *body_end = '\0'; // Terminer la chaîne à </body>
                
                // Ajouter le composant importé à notre registre
                add_imported_component(component, body_start);
                // printf("Composant importé %s ajouté avec le contenu:\n%s\n", component, body_start);
            } else {
                // Si on ne trouve pas les balises body, utiliser tout le contenu
                add_imported_component(component, html_content);
                // printf("Composant importé %s ajouté (sans balises body)\n", component);
            }
            
            fclose(html_file);
            
            // Nettoyer le fichier temporaire
            remove(temp_file);
        }
    }
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
%type <strval> type_instruction type_properties type_property import_instruction return_instruction instructions instruction import_instructions
%%

program:
    element {
        liberer_pile();
        
        // Écrire les en-têtes nécessaires
        printf("#include <stdio.h>\n");
        printf("#include <stdlib.h>\n");
        printf("#include <string.h>\n\n");
        
        // Déclarer le buffer global
        printf("char output_buffer[10000] = {\n");
        
        // Remplir le buffer avec le contenu HTML généré
        printf("    \"");
        for (size_t i = 0; i < strlen(output_buffer); i++) {
            if (output_buffer[i] == '\n') {
                printf("\\n\"\n    \"");
            } else if (output_buffer[i] == '"') {
                printf("\\\"");
            } else if (output_buffer[i] == '\\') {
                printf("\\\\");
            } else {
                printf("%c", output_buffer[i]);
            }
        }
        printf("\"\n};\n\n");
        
        // Écrire la fonction generate_html
        printf("// Fonction pour générer un fichier HTML avec le contenu du buffer\n");
        printf("void generate_html(const char *filename) {\n");
        printf("    FILE *file = fopen(filename, \"w\");\n");
        printf("    if (file == NULL) {\n");
        printf("        fprintf(stderr, \"Error: Failed to open file %%s for writing\\n\", filename);\n");
        printf("        return;\n");
        printf("    }\n");
        printf("    \n");
        printf("    // Écrire l'en-tête HTML standard\n");
        printf("    fprintf(file, \"<!DOCTYPE html>\\n\");\n");
        printf("    fprintf(file, \"<html lang=\\\"en\\\">\\n\");\n");
        printf("    fprintf(file, \"<head>\\n\");\n");
        printf("    fprintf(file, \"    <meta charset=\\\"UTF-8\\\">\\n\");\n");
        printf("    fprintf(file, \"    <meta name=\\\"viewport\\\" content=\\\"width=device-width, initial-scale=1.0\\\">\\n\");\n");
        printf("    fprintf(file, \"    <title>Generated Component</title>\\n\");\n");
        printf("    fprintf(file, \"</head>\\n\");\n");
        printf("    fprintf(file, \"<body>\\n\");\n");
        printf("    \n");
        printf("    // Écrire le contenu du buffer\n");
        printf("    fprintf(file, \"%%s\\n\", output_buffer);\n");
        printf("    \n");
        printf("    // Fermer le document HTML\n");
        printf("    fprintf(file, \"</body>\\n\");\n");
        printf("    fprintf(file, \"</html>\\n\");\n");
        printf("    \n");
        printf("    fclose(file);\n");
        printf("    printf(\"HTML file generated successfully: %%s\\n\", filename);\n");
        printf("}\n\n");
        
        // Ajouter une fonction main pour tester
        printf("int main(int argc, char *argv[]) {\n");
        printf("    const char *output_file = (argc > 1) ? argv[1] : \"output.html\";\n");
        printf("    generate_html(output_file);\n");
        printf("    return 0;\n");
        printf("}\n");
    }
    |import_instructions element{
        liberer_pile();
        
        // Écrire les en-têtes nécessaires
        printf("#include <stdio.h>\n");
        printf("#include <stdlib.h>\n");
        printf("#include <string.h>\n\n");
        
        // Déclarer le buffer global
        printf("char output_buffer[10000] = {\n");
        
        // Remplir le buffer avec le contenu HTML généré
        printf("    \"");
        for (size_t i = 0; i < strlen(output_buffer); i++) {
            if (output_buffer[i] == '\n') {
                printf("\\n\"\n    \"");
            } else if (output_buffer[i] == '"') {
                printf("\\\"");
            } else if (output_buffer[i] == '\\') {
                printf("\\\\");
            } else {
                printf("%c", output_buffer[i]);
            }
        }
        printf("\"\n};\n\n");
        
        // Écrire la fonction generate_html
        printf("// Fonction pour générer un fichier HTML avec le contenu du buffer\n");
        printf("void generate_html(const char *filename) {\n");
        printf("    FILE *file = fopen(filename, \"w\");\n");
        printf("    if (file == NULL) {\n");
        printf("        fprintf(stderr, \"Error: Failed to open file %%s for writing\\n\", filename);\n");
        printf("        return;\n");
        printf("    }\n");
        printf("    \n");
        printf("    // Écrire l'en-tête HTML standard\n");
        printf("    fprintf(file, \"<!DOCTYPE html>\\n\");\n");
        printf("    fprintf(file, \"<html lang=\\\"en\\\">\\n\");\n");
        printf("    fprintf(file, \"<head>\\n\");\n");
        printf("    fprintf(file, \"    <meta charset=\\\"UTF-8\\\">\\n\");\n");
        printf("    fprintf(file, \"    <meta name=\\\"viewport\\\" content=\\\"width=device-width, initial-scale=1.0\\\">\\n\");\n");
        printf("    fprintf(file, \"    <title>Generated Component</title>\\n\");\n");
        printf("    fprintf(file, \"</head>\\n\");\n");
        printf("    fprintf(file, \"<body>\\n\");\n");
        printf("    \n");
        printf("    // Écrire le contenu du buffer\n");
        printf("    fprintf(file, \"%%s\\n\", output_buffer);\n");
        printf("    \n");
        printf("    // Fermer le document HTML\n");
        printf("    fprintf(file, \"</body>\\n\");\n");
        printf("    fprintf(file, \"</html>\\n\");\n");
        printf("    \n");
        printf("    fclose(file);\n");
        printf("    printf(\"HTML file generated successfully: %%s\\n\", filename);\n");
        printf("}\n\n");
        
        // Ajouter une fonction main pour tester
        printf("int main(int argc, char *argv[]) {\n");
        printf("    const char *output_file = (argc > 1) ? argv[1] : \"output.html\";\n");
        printf("    generate_html(output_file);\n");
        printf("    return 0;\n");
        printf("}\n");
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
        $$ = $1; // Store the type name
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

import_instructions:
    import_instructions import_instruction {
        // Combine import instructions
        char *buffer = malloc(strlen($1) + strlen($2) + 2);
        sprintf(buffer, "%s%s", $1, $2);
        $$ = buffer;
        free($1); free($2);
    }
    | import_instruction { $$ = $1; }
;

import_instruction:
    IMPORT IDENTIFIER FROM STRING_LITERAL SEMICOLON {
        // Appeler process_import pour analyser le fichier importé
        process_import($2, $4);
        
        // Retourner le nom du composant importé
        $$ = $2;
        free($4); // Libérer la chaîne du chemin du fichier
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
            yyerror("Erreur d'allocation mémoire");
            YYERROR;
        }
        
        // Vérifier si c'est un composant importé
        char *imported_content = find_imported_component($2);
        if (imported_content != NULL && strlen(imported_content) > 0) {
            // C'est un composant importé, utiliser son contenu
            sprintf(buffer, "%s", imported_content);
            append_to_buffer(buffer);  // Ajouter au buffer de sortie global
        } else {
            // Élément HTML normal
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
            
            // Ajouter le contenu de l'élément HTML
            sprintf(buffer + strlen(buffer), ">%s</%s>", $5, $2);
        }
        
        $$ = buffer;
        free($2); free($3); free($5); free($8); 
    }
    | LT IDENTIFIER attributes SLASH GT {   
        char *buffer = malloc(1000);
        
        // Vérifier si c'est un composant importé
        char *imported_content = find_imported_component($2);
        if (imported_content != NULL && strlen(imported_content) > 0) {
            // C'est un composant importé, utiliser son contenu
            strcpy(buffer, imported_content);
            append_to_buffer(buffer);  // Important: ajouter au buffer de sortie global
        } else {
            // Élément HTML auto-fermant normal
            sprintf(buffer, "<%s", $2);
            
            // Traiter les attributs comme ci-dessus
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
        }
        
        $$ = buffer;
        free($2); free($3);
    }
    | html_inner {
        $$ = $1;
    }
    | IDENTIFIER {
        // Vérifier si c'est un composant importé
        char* component_content = find_imported_component($1);
        
        if (component_content != NULL && strlen(component_content) > 0) {
            // C'est un composant importé
            char *buffer = malloc(strlen(component_content) + 1);
            strcpy(buffer, component_content);
            $$ = buffer;
        } else {
            // Ce n'est pas un composant importé, traiter comme un littéral de texte
            $$ = strdup($1);
        }
        
        free($1);
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
            // Vérifier si c'est un composant importé
            char* component_content = find_imported_component($1);
            
            if (component_content != NULL) {
                // C'est un composant importé
                char *buffer = malloc(strlen(component_content) + 100);
                
                // Insérer le contenu du composant importé
                sprintf(buffer, component_content, $1);
                
                $$ = buffer;
            } else {
                // C'est un littéral de texte
                char* tmp = malloc(strlen($1) + 100);
                sprintf(tmp, "%s", $1);
                $$ = tmp;
            }
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
    
    // Initialiser le stockage des composants importés
    for (int i = 0; i < MAX_IMPORTED_COMPONENTS; i++) {
        imported_components[i].name = NULL;
        imported_components[i].html_content = NULL;
    }
    
    int result = yyparse();
    
    // Libérer la mémoire des composants importés
    for (int i = 0; i < imported_count; i++) {
        free(imported_components[i].name);
        free(imported_components[i].html_content);
    }
    
    return result;
}
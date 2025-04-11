%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


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

   
// Structure pour les types personnalisés
typedef struct {
    char name[50];
    char fields[10][2][50]; // [nombre_de_champs][nom_ou_type][chaine]
    int field_count;
} custom_type;

char fields[10][2][50]; // For storing field names and types during type definition
char field_values[20][100]; // For storing values during initialization
int value_count = 0; 
int field_count = 0;  // Nombre de champs dans le type actuel
custom_type custom_types[20]; // Tableau pour stocker les types personnalisés
int type_count = 0;

// Structure pour stocker les variables
typedef struct {
    char name[50];
    char type[50];
    char value[500];
    int is_array;
} variable;

variable variables[100]; // Tableau pour stocker les variables
int var_count = 0;

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

void add_custom_type(char* name) {
    strcpy(custom_types[type_count].name, name);
    custom_types[type_count].field_count = field_count;
    
    // Copier tous les champs
    for (int i = 0; i < field_count; i++) {
        strcpy(custom_types[type_count].fields[i][0], fields[i][0]); // nom
        strcpy(custom_types[type_count].fields[i][1], fields[i][1]); // type
    }
    
    type_count++;
    
    // Réinitialiser le compteur pour le prochain type
    field_count = 0;
}

void add_field_to_type(char* field_name, char* field_type) {
    if (type_count <= 0) {
        printf("ERREUR: Tentative d'ajout d'un champ sans type défini\n");
        fflush(stdout);
        return;
    }
    
    strcpy(custom_types[type_count-1].fields[custom_types[type_count-1].field_count][0], field_name);
    strcpy(custom_types[type_count-1].fields[custom_types[type_count-1].field_count][1], field_type);
    custom_types[type_count-1].field_count++;
}

void generate_structs_and_prototypes() {
    for (int i = 0; i < type_count; i++) {
        printf("typedef struct {\n");
        fflush(stdout);
        
        for (int j = 0; j < custom_types[i].field_count; j++) {
            char* field_type = custom_types[i].fields[j][1];
            if (strcmp(field_type, "string") == 0) {
                printf("    char* ");
            } else {
                printf("    ");
                printf(field_type);
                printf(" ");
            }
            printf(custom_types[i].fields[j][0]);
            printf(";\n");
            fflush(stdout);
        }
        
        printf("} ");
        printf(custom_types[i].name);
        printf(";\n");
        fflush(stdout);
    }
}

// Fonction pour ajouter une variable
int add_variable(char* name, char* type, char* value, int is_array) {
    printf("//Adding variable: %s, type: %s, value: %s\n", name, type, value);
    strcpy(variables[var_count].name, name);
    strcpy(variables[var_count].type, type);
    strcpy(variables[var_count].value, value);
    variables[var_count].is_array = is_array;
    var_count++;
    return 0;
}
// declared variables
void declare_variables() {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].type, "string") == 0) {
            printf("char* %s = %s;\n", variables[i].name, variables[i].value);
        } else if (strcmp(variables[i].type, "number") == 0 || strcmp(variables[i].type, "int") == 0) {
            printf("int %s = %s;\n", variables[i].name, variables[i].value);
        } else if (strcmp(variables[i].type, "boolean") == 0) {
            printf("bool %s = %s;\n", variables[i].name, variables[i].value);
        } else {
            printf("%s %s=%s;\n",variables[i].type ,variables[i].name, variables[i].value);
        }
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
                            sprintf(final_html, "<div id='%s'>\n%s\n</div>", component, html_content);
                            
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
        sprintf(minimal_html, "<div id='%s'><!-- Contenu du composant %s non analysé --></div>", 
                component, component);
        add_imported_component(component, minimal_html);
        // printf("Impossible d'analyser le composant %s. Utilisation d'un composant minimal.\n", component);
    }
    
    // Fermer le fichier
    fclose(imported_file);
    
    // printf("Import du composant %s terminé\n", component);
}

bool verify_type(char* type) {
    // Check built-in types first
    if (strcmp(type, "string") == 0 || 
        strcmp(type, "number") == 0 ||
        strcmp(type, "boolean") == 0 ||
        strcmp(type, "char*") == 0 ||
        strcmp(type, "int") == 0) {
        return true;
    }
    
    // Then check custom types
    for (int i = 0; i < type_count; i++) {
        if (strcmp(custom_types[i].name, type) == 0) {
            return true;  
        }
    }
    
    // Type not found
    return false;
}

// Fonction pour trouver un type personnalisé
custom_type* find_custom_type(char* type_name) {
    for (int i = 0; i < type_count; i++) {
        if (strcmp(custom_types[i].name, type_name) == 0) {
            return &custom_types[i];
        }
    }
    return NULL;
}

// Main type compatibility function
bool check_field_value_compatibility(char* field_type, char* value) {
    // Pour les chaînes
    if (strcmp(field_type, "string") == 0) {
        return value[0] == '"'; // Vérifie si la valeur commence par des guillemets
    }
    // Pour les nombres
    else if (strcmp(field_type, "int") == 0 || strcmp(field_type, "number") == 0) {
        // Vérifier si la valeur est un nombre
        char* endptr;
        strtol(value, &endptr, 10);
        return *endptr == '\0';
    }
    // Pour les booléens
    else if (strcmp(field_type, "boolean") == 0) {
        return (strcmp(value, "true") == 0 || strcmp(value, "false") == 0);
    }
    
    // Vérifier si c'est une variable du bon type
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, value) == 0) {
            return strcmp(variables[i].type, field_type) == 0;
        }
    }
    
    return false;
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
%type <strval> element parameters parameter function html_content html_inner attributes attribute html_element function_body variable_instruction
%type <strval> type_instruction type_properties type_property import_instruction return_instruction instructions instruction import_instructions field_value_list 
%token <strval> NUMBER_LITERAL BOOLEAN_LITERAL
%%

program:
    element {
        liberer_pile();
        // Before calling generate_structs_and_prototypes()
        printf("// Définition des types personnalisés\n");
        fflush(stdout);

        // Call the function
        generate_structs_and_prototypes(); 
        // Déclarer les variables
        printf("// Déclaration des variables\n");
        declare_variables(); 
        // After calling
        fflush(stdout);
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
        
        // Before calling generate_structs_and_prototypes()
        printf("// Définition des types personnalisés\n");
        

        // Call the function
        generate_structs_and_prototypes();

        // Déclarer les variables
        printf("// Déclaration des variables\n");
        declare_variables(); 
        // Déclarer le buffer global
        printf("\nchar output_buffer[10000] = {\n");
        
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
        sprintf(buffer, "<div id='%s'>\n%s\n</div>", $2, $6);
        
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
    |variable_instruction{
        // Générer une instruction de variable
        $$ = $1; // Store the variable name
    }
    | return_instruction {
        // Générer une instruction de retour
        $$ = $1; // Store the return value
    }
    
;

type_instruction:
    TYPE IDENTIFIER EQUALS {add_custom_type($2);}
    LBRACE type_properties RBRACE SEMICOLON {
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
        add_field_to_type($1, $3);
        free($1); free($3);
    }
;

return_instruction:
    RETURN LPAREN html_content RPAREN SEMICOLON {
        $$ = $3; // Store the HTML content
    }
;

field_value_list:
    STRING_LITERAL {
        strcpy(field_values[value_count++], $1);
        free($1);
    }
    | NUMBER_LITERAL {
        strcpy(field_values[value_count++], $1);
        free($1);
    }
    | BOOLEAN_LITERAL {
        strcpy(field_values[value_count++], $1);
        free($1);
    }
    | IDENTIFIER {
        strcpy(field_values[value_count++], $1);
        free($1);
    }
    | field_value_list COMMA STRING_LITERAL {
        strcpy(field_values[value_count++], $3);
        free($3);
    }
    | field_value_list COMMA NUMBER_LITERAL {
        strcpy(field_values[value_count++], $3);
        free($3);
    }
    | field_value_list COMMA BOOLEAN_LITERAL {
        strcpy(field_values[value_count++], $3);
        free($3);
    }
    | field_value_list COMMA IDENTIFIER {
        strcpy(field_values[value_count++], $3);
        free($3);
    }
;

variable_instruction:
    IDENTIFIER COLON IDENTIFIER EQUALS field_value_list SEMICOLON {
        char* var_name = $1;
        char* type_name = $3;
        
        // Vérifier si la variable existe déjà
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, var_name) == 0) {
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' already declared", var_name);
                yyerror(error_msg);
                YYERROR;
                break;
            }
        }
        
        // Trouver le type personnalisé
        custom_type* type = find_custom_type(type_name);
        if (type == NULL) {
            // Vérifier si c'est un type primitif
            if (verify_type(type_name)) {
                // Gérer les types primitifs ici
                // ...
            } else {
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", type_name);
                yyerror(error_msg);
                YYERROR;
            }
        } else {
            // C'est un type personnalisé - vérifier les valeurs
            if (value_count != type->field_count) {
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), 
                       "Error: Expected %d values for type '%s', got %d",
                       type->field_count, type_name, value_count);
                yyerror(error_msg);
                YYERROR;
            } else {
                // Vérifier chaque valeur de champ
                for (int i = 0; i < value_count; i++) {
                    if (!check_field_value_compatibility(type->fields[i][1], field_values[i])) {
                        char error_msg[256];
                        snprintf(error_msg, sizeof(error_msg), 
                               "Type error: Cannot assign '%s' to field '%s' of type '%s'",
                               field_values[i], type->fields[i][0], type->fields[i][1]);
                        yyerror(error_msg);
                        YYERROR;
                        break;
                    }
                }
                
                // Tous les contrôles sont passés, ajouter la variable
                // Combiner les valeurs dans une chaîne pour le stockage
                char value_str[1024] = "";
                for (int i = 0; i < value_count; i++) {
                    strcat(value_str, field_values[i]);
                    if (i < value_count - 1) {
                        strcat(value_str, ",");
                    }
                }
                
                add_variable(var_name, type_name, value_str, 1); // 1 = type personnalisé
            }
        }
        
        // Réinitialiser le compteur de valeurs
        value_count = 0;
        $$ = strdup(var_name); // Return the variable name for further processing if needed
        free(var_name);
        free(type_name);
    }
    |IDENTIFIER COLON IDENTIFIER EQUALS STRING_LITERAL SEMICOLON {
        // Check if variable is already declared
        int var_exists = 0;
        printf("//Checking if variable exists: %s\n", $1);
        
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, $1) == 0) {
                var_exists = 1;
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' is already declared", $1);
                yyerror(error_msg);
                break;
            }
        }
     
        if (!var_exists) {
            // Verify type exists
            if (verify_type($3)) {
                // Check type compatibility between declared type and value
                if (strcmp($3, "string") == 0 && $5[0] == '"') {
                    // String type with string literal value - compatible
                    add_variable($1, $3, $5, 0);
                    $$ = strdup($1);
                } else {
                    // Type mismatch
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                             "Type error: Cannot assign %s to variable '%s' of type %s", 
                             $5, $1, $3);
                    yyerror(error_msg);
                    YYERROR;
                }
            } else {
                // Type doesn't exist
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", $3);
                yyerror(error_msg);
                YYERROR;
            }
        }
        
        // Free memory
        free($1); free($3); free($5);
    }
    | IDENTIFIER COLON IDENTIFIER EQUALS IDENTIFIER SEMICOLON {
        // Check if variable is already declared
        int var_exists = 0;
        printf("Checking if variable exists: %s\n", $1);
        
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, $1) == 0) {
                var_exists = 1;
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' is already declared", $1);
                yyerror(error_msg);
                break;
            }
        }
        
        if (!var_exists) {
            // Verify type exists
            if (verify_type($3)) {
                // Check if the value identifier exists and is compatible
                int value_var_exists = 0;
                char* value_type = NULL;
                
                for (int i = 0; i < var_count; i++) {
                    if (strcmp(variables[i].name, $5) == 0) {
                        value_var_exists = 1;
                        value_type = variables[i].type;
                        break;
                    }
                }
                
                if (!value_var_exists) {
                    // The value is not a known variable
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                             "Error: Undefined identifier '%s' used as value", $5);
                    yyerror(error_msg);
                    YYERROR;
                } else if (strcmp(value_type, $3) != 0) {
                    // Type mismatch between value and variable
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                             "Type error: Cannot assign value of type '%s' to variable '%s' of type '%s'", 
                             value_type, $1, $3);
                    yyerror(error_msg);
                    YYERROR;
                } else {
                    // Types match, add the variable
                    add_variable($1, $3, $5, 0);
                    $$ = strdup($1);
                }
            } else {
                // Type doesn't exist
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", $3);
                yyerror(error_msg);
                YYERROR;
            }
        }
        
        // Free memory
        free($1); free($3); free($5);
    }
    | IDENTIFIER COLON IDENTIFIER EQUALS NUMBER_LITERAL SEMICOLON {
        // Check if variable is already declared
        int var_exists = 0;
        
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, $1) == 0) {
                var_exists = 1;
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' is already declared", $1);
                yyerror(error_msg);
                break;
            }
        }
        
        if (!var_exists) {
            // Verify type exists
            if (verify_type($3)) {
                // Check type compatibility
                if (strcmp($3, "number") == 0 || strcmp($3, "int") == 0) {
                    // Number type with number literal value - compatible
                    add_variable($1, $3, $5, 0);
                    $$ = strdup($1);
                } else {
                    // Type mismatch
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                             "Type error: Cannot assign numeric value to variable '%s' of type '%s'", 
                             $1, $3);
                    yyerror(error_msg);
                    YYERROR;
                }
            } else {
                // Type doesn't exist
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", $3);
                yyerror(error_msg);
                YYERROR;
            }
        }
        
        // Free memory
        free($1); free($3); free($5);
    }
    | IDENTIFIER COLON IDENTIFIER EQUALS BOOLEAN_LITERAL SEMICOLON {
        // Check if variable is already declared
        int var_exists = 0;
        
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, $1) == 0) {
                var_exists = 1;
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' is already declared", $1);
                yyerror(error_msg);
                break;
            }
        }
        
        if (!var_exists) {
            // Verify type exists
            if (verify_type($3)) {
                // Check type compatibility
                if (strcmp($3, "boolean") == 0) {
                    // Boolean type with boolean literal value - compatible
                    add_variable($1, $3, $5, 0);
                    $$ = strdup($1);
                } else {
                    // Type mismatch
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                             "Type error: Cannot assign boolean value to variable '%s' of type '%s'", 
                             $1, $3);
                    yyerror(error_msg);
                    YYERROR;
                }
            } else {
                // Type doesn't exist
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", $3);
                yyerror(error_msg);
                YYERROR;
            }
        }
        
        // Free memory
        free($1); free($3); free($5);
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
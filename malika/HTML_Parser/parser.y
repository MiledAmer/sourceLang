%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "lib/component.h"
#include "lib/customType.h"
#include "lib/variable.h"
#include "lib/identifier.h"

extern int yylex();
void yyerror(const char *s);
extern FILE *yyin;  
extern void yyrestart(FILE* input_file);
#define YYDEBUG 1


char output_buffer[10000];
char interfaces_buffer[1000];  // Buffer pour les interfaces TypeScript
// int id_count = 0;
int found = 0;

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

// // Fonction pour générer le code HTML pur selon le type
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

%}

%union {
    int intval;
    char* strval;
}

%token COMPONENT RETURN EQUALS 
%token LBRACE RBRACE LPAREN RPAREN COLON TYPE SEMICOLON FROM IMPORT COMMA
%token LT GT SLASH DOT LBRACKET RBRACKET
%token <strval> IDENTIFIER STRING_LITERAL
%type <strval> element parameters parameter function html_content html_inner attributes attribute html_element function_body variable_instruction identifiant_interpole
%type <strval> type_instruction type_properties type_property import_instruction return_instruction instructions instruction import_instructions field_value_list 
%type <strval> field_values field_value value custom_type_object custom_type_array_elements array_value array_values
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
        // Déclarer les variables
        printf("// Déclaration des variables\n");
        declare_variables(); 
        printf("\n");
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
        // Déclarer les variables
        printf("// Déclaration des variables\n");
        declare_variables(); 
        printf("\n");
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
        if (strcmp($1," ") == 0) {
            $$ = $2; // Ignore empty instructions
        } else {
            char *buffer = malloc(strlen($1) + strlen($2) + 2);
            sprintf(buffer, "%s\n%s", $1, $2);
            $$ = buffer;
            free($1); free($2);
        }
        
    }
;

instruction:
    type_instruction {
        // Générer une instruction de type
        $$ = strdup(" "); // Store the type name
    }
    |variable_instruction{
        // Générer une instruction de variable
        $$ = strdup(" "); // Store the variable name
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
    LBRACE field_values RBRACE {
        // Traitement terminé, résultat déjà stocké dans field_names et field_values
        $$ = strdup(""); // Simplement pour éviter les erreurs de syntaxe
    }
    | LBRACE RBRACE {
        // Cas d'un objet vide
        $$ = strdup("");
    }
;

field_values:
    field_values COMMA field_value {
        // Ajoute simplement une nouvelle paire field_name:value
    }
    | field_value {
        // Premier champ
    }
;

field_value:
    IDENTIFIER COLON value {
        // Stocker le nom du champ
        if (field_count < MAX_FIELDS) {
            strcpy(field_names[field_count], $1);
            // La valeur a déjà été stockée dans field_values par la règle value
            field_count++;
        } else {
            yyerror("Too many fields");
        }
        free($1);
    }
;

value:
    STRING_LITERAL {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], $1);
            field_types[value_count] = TYPE_STRING;
            value_count++;
        }
        free($1);
    }
    | NUMBER_LITERAL {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], $1);
            field_types[value_count] = TYPE_NUMBER;
            value_count++;
        }
        free($1);
    }
    | BOOLEAN_LITERAL {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], $1);
            field_types[value_count] = TYPE_BOOLEAN;
            value_count++;
        }
        free($1);
    }
    | IDENTIFIER {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], $1);
            field_types[value_count] = TYPE_IDENTIFIER;
            value_count++;
        }
        free($1);
    }
;

variable_instruction:
    IDENTIFIER COLON IDENTIFIER EQUALS field_value_list SEMICOLON {
        char* var_name = $1;
        char* type_name = $3;
        int is_valid = 1;
        
        // Vérifier d'abord si le type existe
        custom_type* type = find_custom_type(type_name);
        int is_primitive_type = verify_type(type_name);
        
        if (type == NULL && !is_primitive_type) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", type_name);
            yyerror(error_msg);
            is_valid = 0;
        }
        
        // Vérifier si la variable existe déjà
        if (is_valid && check_variable_exists(var_name)) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' already declared", var_name);
            yyerror(error_msg);
            is_valid = 0;
        }
        
        if (is_valid) {
            if (type != NULL) {
                // C'est un type personnalisé - vérifier les attributs et leurs valeurs
                // Créer une table de hachage temporaire pour vérifier les champs fournis
                int field_provided[MAX_FIELDS] = {0}; // Pour marquer les champs fournis
                
                // Vérifier si tous les champs fournis existent dans le type
                for (int i = 0; i < field_count && is_valid; i++) {
                    int field_found = 0;
                    
                    for (int j = 0; j < type->field_count; j++) {
                        if (strcmp(field_names[i], type->fields[j][0]) == 0) {
                            field_found = 1;
                            field_provided[j] = 1; // Marquer ce champ comme fourni
                            
                            // Vérifier la compatibilité du type pour ce champ
                            if (!check_field_value_compatibility(type->fields[j][1], field_values[i], field_types[i])) {
                                char error_msg[256];
                                snprintf(error_msg, sizeof(error_msg), 
                                       "Type error: Cannot assign '%s' to field '%s' of type '%s'",
                                       field_values[i], field_names[i], type->fields[j][1]);
                                yyerror(error_msg);
                                is_valid = 0;
                            }
                            break;
                        }
                    }
                    
                    if (!field_found) {
                        char error_msg[256];
                        snprintf(error_msg, sizeof(error_msg), 
                               "Error: Field '%s' does not exist in type '%s'",
                               field_names[i], type_name);
                        yyerror(error_msg);
                        is_valid = 0;
                    }
                }
                
                // Vérifier que tous les champs requis sont fournis
                for (int j = 0; j < type->field_count && is_valid; j++) {
                    if (!field_provided[j] && type->fields[j][2] != NULL && strcmp(type->fields[j][2], "required") == 0) {
                        char error_msg[256];
                        snprintf(error_msg, sizeof(error_msg), 
                               "Error: Required field '%s' of type '%s' is missing",
                               type->fields[j][0], type_name);
                        yyerror(error_msg);
                        is_valid = 0;
                    }
                }
                
                if (is_valid) {
                    char value_buffer[500] = "{";
                    for (int i = 0; i < field_count; i++) {
                        char field_entry[128];
                        
                        // Ajouter des guillemets autour des strings
                        if (field_types[i]== 0) {
                            snprintf(field_entry, sizeof(field_entry), "%s=\"%s\"", field_names[i], field_values[i]);
                        } else {
                            snprintf(field_entry, sizeof(field_entry), "%s=%s", field_names[i], field_values[i]);
                        }

                        strcat(value_buffer, field_entry);
                        if (i < field_count - 1) {
                            strcat(value_buffer, ", ");
                        }
                    }
                    strcat(value_buffer, "}");

                    // Appel existant (inchangé) avec la vraie valeur maintenant
                    add_variable(var_name, type_name, value_buffer, 0);

                }
            } else if (is_primitive_type) {
                // C'est un type primitif
                if (field_count != 1) {
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                           "Error: Primitive type '%s' expects single value, got %d values", 
                           type_name, field_count);
                    yyerror(error_msg);
                    is_valid = 0;
                } else if (!check_primitive_type_compatibility(type_name, field_values[0], field_types[0])) {
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                           "Type error: Cannot assign '%s' to variable of type '%s'", 
                           field_values[0], type_name);
                    yyerror(error_msg);
                    is_valid = 0;
                } else {
                    // Ajouter la variable primitive
                    add_variable(var_name, type_name, field_values[0], 0); // 0 = type primitif
                }
            }
        }
        
        // Réinitialiser les compteurs
        field_count = 0;
        value_count = 0;
        
        if (!is_valid) {
            YYERROR;
        }
        
        $$ = strdup(var_name); // Return the variable name for further processing if needed
        free(var_name);
        free(type_name);
    }
    |IDENTIFIER COLON IDENTIFIER EQUALS value SEMICOLON {
        char* var_name = $1;
        char* type_name = $3;
        int is_valid = 1;
        
        // Vérifier que la valeur a bien été ajoutée
        if (value_count != 1) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Expected 1 value, got %d", value_count);
            yyerror(error_msg);
            is_valid = 0;
        } else {
            char* value_str = field_values[0];
            
            // Vérifier si la variable existe déjà
            if (check_variable_exists(var_name)) {
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' already declared", var_name);
                yyerror(error_msg);
                is_valid = 0;
            }
            
            if (is_valid) {
                // Vérifier si c'est un type personnalisé
                custom_type* type = find_custom_type(type_name);
                if (type != NULL) {
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                           "Error: Custom type '%s' requires %d values", type_name, type->field_count);
                    yyerror(error_msg);
                    is_valid = 0;
                } 
                // Vérifier si c'est un type primitif valide
                else if (!verify_type(type_name)) {
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", type_name);
                    yyerror(error_msg);
                    is_valid = 0;
                }
                // Vérifier la compatibilité des types
                else if (!check_primitive_type_compatibility(type_name, value_str, field_types[0])) {
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                           "Type error: Cannot assign '%s' to variable of type '%s'", 
                           value_str, type_name);
                    yyerror(error_msg);
                    is_valid = 0;
                }
                else {
                    // Ajouter la variable
                    add_variable(var_name, type_name, value_str, 0); // 0 = type primitif
                }
            }
        }
        
        // Réinitialiser le compteur de valeurs
        value_count = 0;
        
        if (!is_valid) {
            YYERROR;
        }
        
        $$ = strdup(var_name);
        free(var_name);
        free(type_name);
    }
     // Add this new rule for array declarations
    | IDENTIFIER COLON IDENTIFIER LBRACKET RBRACKET EQUALS LBRACKET array_values RBRACKET SEMICOLON {
        char* var_name = $1;
        char* type_name = $3;
        
        // Check if variable already exists
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, var_name) == 0) {
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' already declared", var_name);
                yyerror(error_msg);
                YYERROR;
                break;
            }
        }
        
        // Verify the base type exists
        if (verify_type(type_name)) {
            // Add variable with is_array flag set to 1
            char array_value_str[1024] = "{";
            for (int i = 0; i < array_value_count; i++) {
                strcat(array_value_str, array_values[i]);
                if (i < array_value_count - 1) {
                    strcat(array_value_str, ", ");
                }
            }
            strcat(array_value_str, "}");
            printf("Array value string: %s\n", array_value_str); // Debugging line
            add_variable(var_name, type_name, array_value_str, 1);
            $$ = strdup(var_name);
            
            // Reset array_value_count
            array_value_count = 0;
        } else {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", type_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        free(var_name);
        free(type_name);
    }
    | IDENTIFIER COLON IDENTIFIER LBRACKET RBRACKET EQUALS LBRACKET  custom_type_array_elements RBRACKET SEMICOLON {
        char* var_name = $1;
        char* type_name = $3;
        int is_valid = 1;
        
        // Check if the type exists and is a custom type
        custom_type* type = find_custom_type(type_name);
        if (type == NULL) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Custom type '%s' is not defined", type_name);
            yyerror(error_msg);
            is_valid = 0;
        }
        
        // Check if variable already exists
        if (is_valid && check_variable_exists(var_name)) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' already declared", var_name);
            yyerror(error_msg);
            is_valid = 0;
        }
        
        if (is_valid) {
            // Format the array of custom type objects
            char array_value[2048] = "{"; // Larger buffer for complex structures
            
            for (int i = 0; i < custom_array_element_count; i++) {
                strcat(array_value, custom_array_elements[i]);
                
                if (i < custom_array_element_count - 1) {
                    strcat(array_value, ", ");
                }
            }
            strcat(array_value, "}");
            printf("Custom array value string: %s\n", array_value); // Debugging line
            // Add the array variable
            add_variable(var_name, type_name, array_value, 1); // 1 = is_array
        }
        
        // Reset array element count
        custom_array_element_count = 0;
        
        if (!is_valid) {
            YYERROR;
        }
        
        $$ = strdup(var_name);
        free(var_name);
        free(type_name);
    }
;

// Add a rule to collect array values
array_values:
    array_value {
        array_values[0] = $1;
        array_value_count = 1;
    }
    | array_values COMMA array_value {
        if (array_value_count < MAX_ARRAY_VALUES) {
            array_values[array_value_count++] = $3;
        } else {
            yyerror("Too many array values");
            YYERROR;
        }
    }
;

array_value:
    NUMBER_LITERAL {
        $$ = $1;
    }
    | STRING_LITERAL {
        $$ = $1;
    }
    | BOOLEAN_LITERAL {
        $$ = $1;
    }
    | IDENTIFIER {
        // Check if the identifier exists
        int found = 0;
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, $1) == 0) {
                found = 1;
                $$ = strdup(variables[i].name);
                break;
            }
        }
        if (!found) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Undefined identifier '%s' used in array", $1);
            yyerror(error_msg);
            YYERROR;
        }
    }
;

// Rules for custom type array elements
custom_type_array_elements:
    custom_type_object {
        strcpy(custom_array_elements[0], $1);
        custom_array_element_count = 1;
        free($1);
    }
    | custom_type_array_elements COMMA custom_type_object {
        if (custom_array_element_count < MAX_ARRAY_ELEMENTS) {
            strcpy(custom_array_elements[custom_array_element_count], $3);
            custom_array_element_count++;
        } else {
            yyerror("Too many array elements");
            YYERROR;
        }
        free($3);
    }
;

// Rule for a single custom type object
custom_type_object:
    LBRACE field_values RBRACE {
        // Create a temporary buffer for the object
        char object_str[1024] = "{";
        int len = 0;
        
        for (int i = 0; i < field_count; i++) {
            // Format each field correctly
            char field_entry[256];
            
            // Handle string value formatting (adding quotes if necessary)
            if (field_types[i] == TYPE_STRING) {
                // Check if already quoted
                if (field_values[i][0] != '"') {
                    snprintf(field_entry, sizeof(field_entry), "%s = \"%s\"", 
                             field_names[i], field_values[i]);
                } else {
                    snprintf(field_entry, sizeof(field_entry), "%s = %s", 
                             field_names[i], field_values[i]);
                }
            } else {
                // For non-string types
                snprintf(field_entry, sizeof(field_entry), "%s = %s", 
                         field_names[i], field_values[i]);
            }
            
            // Add to the object string
            strcat(object_str, field_entry);
            if (i < field_count - 1) {
                strcat(object_str, ", ");
            }
        }
        
        strcat(object_str, "}");
        
        // Store this object and reset field counters
        $$ = strdup(object_str);
        
        // Reset field_count and value_count for the next object
        field_count = 0;
        value_count = 0;
    }
;

identifiant_interpole:
    IDENTIFIER {
        // Vérifier si l'identifiant existe
        char* type = get_identifier_type($1);
        if (check_variable_exists($1) == 0) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : L'identifiant %s n'est pas défini.", $1);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Obtenir la valeur via get_value (gère simple & structuré)
        char* value = get_value($1, NULL);
        if (value == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : Impossible d'obtenir la valeur de %s", $1);
            yyerror(error_msg);
            YYERROR;
        }
        
        $$ = value;  // déjà dupliqué dans get_value
        
        free($1); // Libérer la chaîne d'origine
        
    }    
    |IDENTIFIER DOT IDENTIFIER {
        // Vérifier si le champ existe dans la structure
        char* var_name = $1;
        char* field_name = $3;
        
        // Vérifier si la variable existe
        if (check_variable_exists(var_name) == 0) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : L'identifiant %s n'est pas une variable définie.", var_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Obtenir la valeur via get_value (gère simple & structuré)
        char* value = get_value(var_name, field_name);
        if (value == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : Impossible d'obtenir la valeur de %s.%s", var_name, field_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        $$ = value;  // déjà dupliqué dans get_value
        
        free($1); free($3); // Libérer les chaînes d'origine
    }
    | IDENTIFIER LBRACKET NUMBER_LITERAL RBRACKET {
        // Array element access
        char* array_name = $1;
        char* index_str = $3;
        
        // Check if the array exists
        if (!check_variable_exists(array_name)) {
            char error_msg[100];
            sprintf(error_msg, "Error: Undefined array '%s'", array_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Check if it's actually an array
        int is_array = 0;
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, array_name) == 0) {
                is_array = variables[i].is_array;
                break;
            }
        }
        
        if (!is_array) {
            char error_msg[100];
            sprintf(error_msg, "Error: '%s' is not an array", array_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        char var_name[100];
        sprintf(var_name, "%s[%s]", array_name, index_str);
        char* value = get_value(var_name, NULL);
        if (value == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : Impossible d'obtenir la valeur de %s", var_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        $$ = value;
        free(array_name);
        free(index_str);
    }
    
    | IDENTIFIER LBRACKET NUMBER_LITERAL RBRACKET DOT IDENTIFIER {
        // Access to field in array element
        char* array_name = $1;
        char* index_str = $3;
        char* field_name = $6;
        
        // Check if the array exists
        if (!check_variable_exists(array_name)) {
            char error_msg[100];
            sprintf(error_msg, "Error: Undefined array '%s'", array_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Check if it's actually an array
        int is_array = 0;
        char* type_name = NULL;
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, array_name) == 0) {
                is_array = variables[i].is_array;
                type_name = variables[i].type;
                break;
            }
        }
        
        if (!is_array) {
            char error_msg[100];
            sprintf(error_msg, "Error: '%s' is not an array", array_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Check if the array type is a custom type
        custom_type* type = find_custom_type(type_name);
        if (type == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Error: '%s' is not a custom type", type_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Check if the field exists in the custom type
        int field_exists = 0;
        for (int i = 0; i < type->field_count; i++) {
            if (strcmp(type->fields[i][0], field_name) == 0) {
                field_exists = 1;
                break;
            }
        }
        
        if (!field_exists) {
            char error_msg[100];
            sprintf(error_msg, "Error: Field '%s' does not exist in type '%s'", field_name, type_name);
            yyerror(error_msg);
            YYERROR;
        }

        char var_name[100];
        sprintf(var_name, "%s[%s]", array_name, index_str);
        char* value = get_value(var_name, field_name);
        if (value == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : Impossible d'obtenir la valeur de %s.%s", var_name, field_name);
            yyerror(error_msg);
            YYERROR;
        }
        $$ = value;
        free(array_name);
        free(index_str);
        free(field_name);
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
            sprintf(buffer, "\n\t<%s", $2);
            
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
            sprintf(buffer + strlen(buffer), ">%s</%s>\n\t", $5, $2);
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
            sprintf(buffer, "\n\t<%s", $2);
            
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
            sprintf(buffer + strlen(buffer), " />\n");
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
    | LBRACE identifiant_interpole RBRACE {
        $$= $2;
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
    
    
    initialize_imported_components(); // Initialiser la liste des composants importés
    int result = yyparse();
    
    free_imported_components(); // Libérer la mémoire des composants importés
    
    return result;
}
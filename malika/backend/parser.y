%{
#include <stdio.h>
#include <string.h>
#include "lib/variable.h"
#include "lib/customType.h"
#include "lib/identifier.h"

extern int yylex();
void yyerror(const char *s);

#define YYDEBUG 1
char current_component[256];
%}

%union {
    int intval;   // For numeric values
    char* strval; // For strings like IDENTIFIER
}


%token COMPONENT LBRACE RBRACE LPAREN RPAREN COLON SEMICOLON COMMA LBRACKET RBRACKET EQUALS FUNCTION IMPORT FROM TYPE RETURN LT GT SLASH DOT 
%token <strval> IDENTIFIER STRING_LITERAL NUMBER_LITERAL BOOLEAN_LITERAL 
%type <strval> element parameters typed_param_list typed_param function_content list_function function program return_instruction instruction instructions 
%type <strval> field_value_list field_values field_value value variable_instruction custom_type_array_elements array_values array_value custom_type_object 
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
    FUNCTION IDENTIFIER LPAREN parameters RPAREN COLON IDENTIFIER function_content
    {
        /* $2 is the function name and $4 is the parameter list */
        printf("%s %s(%s) {\n",$7, $2, $4);
        declare_variables();
        printf("\t%s\n}\n",$8);
        free($2);
        free($7);
        free($8);
        free($4);
    }
;

function_content:
    LBRACE RBRACE {
        $$ = strdup(""); 
    }
    | LBRACE instructions RBRACE {
        $$ = $2; 
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
    variable_instruction{
        // Générer une instruction de variable
        $$ = strdup(" "); // Store the variable name
    }
    | return_instruction {
        // Générer une instruction de retour
        $$ = $1; // Store the return value
    }
    
;

return_instruction:
    RETURN IDENTIFIER SEMICOLON {
        // Vérifier si la variable existe
        int found = 0;
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, $2) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Undefined identifier '%s' used in return", $2);
            yyerror(error_msg);
            YYERROR;
        }
        else {
            $$ = strdup($2); // Store the return value
        }
       
    }
    |RETURN BOOLEAN_LITERAL SEMICOLON {
        $$ = strdup($2); // Store the return value
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
        char* declaration= NULL;
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
    |IDENTIFIER COLON IDENTIFIER SEMICOLON
    {
        char* var_name = $1;
        char* type_name = $3;
        int is_valid = 1;
        
        // Vérifier si la variable existe déjà
        if (check_variable_exists(var_name)) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg),"Variable '%s' already declared" , var_name);
            yyerror(error_msg); 
            is_valid = 0;
        }
        
        // Vérifier si c'est un type valide
        if (is_valid && !verify_type(type_name)) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg),"Type '%s' is not defined", var_name);
            yyerror(error_msg);
            is_valid = 0;
        }
        
        // Ajouter la variable si tout est valide
        if (is_valid) {
            add_variable(var_name, type_name, get_default_value(type_name), 0);
        }
        
        if (!is_valid) {
            YYERROR;
        }
        
        $$ = var_name; // Pas besoin de strdup car var_name sera utilisé
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
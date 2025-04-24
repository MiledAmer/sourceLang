#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "variable.h"
#include "customType.h"


char* array_values[MAX_ARRAY_VALUES];
int array_value_count = 0;

char custom_array_elements[MAX_ARRAY_ELEMENTS][1024]; // Larger size for custom type objects
int custom_array_element_count = 0;

variable variables[100]; // Tableau pour stocker les variables
int var_count ;
char field_names[MAX_FIELDS][256];  // Noms des champs
char field_values[MAX_VALUES][256]; // Valeurs des champs
int field_types[MAX_VALUES];        // Types des valeurs (string, number, boolean, identifier)
int field_count=0;                // Nombre de champs traités
int value_count =0 ;                // Nombre de valeurs traitées

// Fonction pour ajouter une variable
int add_variable(char* name, char* type, char* value, int is_array) {
    // printf("//Adding variable: %s, type: %s, value: %s\n", name, type, value);
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
        if (variables[i].is_array) {
            // Get the type information
            custom_type* type = find_custom_type(variables[i].type);
            
            if (type != NULL) {
                // This is an array of custom types
                printf("\t// Array of custom type %s\n", variables[i].type);
                printf("\t%s %s[%d];\n", variables[i].type, variables[i].name, 
                       count_array_elements(variables[i].value));
                
                // Initialize each element of the array
                char value_copy[2048];
                strcpy(value_copy, variables[i].value);
                
                // Remove outer braces
                char* inner_array = value_copy + 1;  // Skip first '{'
                inner_array[strlen(inner_array)-1] = '\0';  // Remove last '}'
                
                // Parse each element
                int index = 0;
                char* element = extract_array_element(inner_array, &index);
                int element_index = 0;
                
                while (element != NULL) {
                    // Process the individual custom type object
                    initialize_custom_type_element(variables[i].name, element_index, 
                                                 variables[i].type, element);
                    element_index++;
                    
                    // Get next element
                    element = extract_array_element(inner_array, &index);
                }
            } else {
                // Handle primitive type arrays (existing code)
                if (strcmp(variables[i].type, "string") == 0) {
                    printf("\tchar* %s[] = %s;\n", variables[i].name, variables[i].value);
                } else if (strcmp(variables[i].type, "number") == 0) {
                    printf("\tdouble %s[] = %s;\n", variables[i].name, variables[i].value);
                } else if (strcmp(variables[i].type, "int") == 0) {
                    printf("\tint %s[] = %s;\n", variables[i].name, variables[i].value);
                } else if (strcmp(variables[i].type, "boolean") == 0) {
                    printf("\tbool %s[] = %s;\n", variables[i].name, variables[i].value);
                }
            }
        } else {
            if (strcmp(variables[i].type, "string") == 0) {
                printf("\tchar* %s = %s;\n", variables[i].name, variables[i].value);
            } else if (strcmp(variables[i].type, "number") == 0 || strcmp(variables[i].type, "int") == 0) {
                printf("\tint %s = %s;\n", variables[i].name, variables[i].value);
            } else if (strcmp(variables[i].type, "boolean") == 0) {
                printf("\tbool %s = %s;\n", variables[i].name, variables[i].value);
            } else if (find_custom_type(variables[i].type)!= NULL) {
                // Si c'est un type personnalisé, on l'affiche comme une structure
                char* var_name = variables[i].name;
                char* type_name = variables[i].type;
                char* raw_value = variables[i].value;
            
                custom_type* type = find_custom_type(type_name);
                if (type == NULL) continue;
                printf("\n");
                // Déclaration de la variable
                printf("\t%s %s;\n", type_name, var_name);
            
                // Supprimer les accolades { } de raw_value
                char value_copy[500];
                strcpy(value_copy, raw_value);
                char* inner = value_copy;
                if (value_copy[0] == '{') inner++;
                if (inner[strlen(inner)-1] == '}') inner[strlen(inner)-1] = '\0';
            
                // Parser les paires champ=valeur
                char* token = strtok(inner, ",");
                while (token != NULL) {
                    char field_name[50], field_value[200];
                    char* equal_sign = strchr(token, '=');
                    if (equal_sign) {
                        *equal_sign = '\0';
                        strcpy(field_name, token);
                        strcpy(field_value, equal_sign + 1);
            
                        // Nettoyer les espaces
                        while (field_name[0] == ' ') memmove(field_name, field_name + 1, strlen(field_name));
                        while (field_value[0] == ' ') memmove(field_value, field_value + 1, strlen(field_value));
            
                        // Trouver le type du champ
                        char* field_type = NULL;
                        for (int j = 0; j < type->field_count; j++) {
                            if (strcmp(field_name, type->fields[j][0]) == 0) {
                                field_type = type->fields[j][1];
                                break;
                            }
                        }
            
                        // Générer le code d'initialisation
                        if (field_type != NULL) {
                            if (strcmp(field_type, "string") == 0) {
                                printf("\t%s.%s = malloc(strlen(%s)+1);\n", var_name, field_name, field_value);
                                printf("\tstrcpy(%s.%s, %s);\n", var_name, field_name, field_value);
                            } else {
                                printf("\t%s.%s = %s;\n", var_name, field_name, field_value);
                            }
                        }
                    }
                    token = strtok(NULL, ",");
                }
                printf("\n");
            }     
        }
    }    
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

int check_field_value_compatibility(char* field_type, char* value, int value_type_id) {
    // Vérifiez la compatibilité en fonction du type
    if (strcmp(field_type, "string") == 0) {
        return value_type_id == TYPE_STRING;
    } else if (strcmp(field_type, "number") == 0) {
        return value_type_id == TYPE_NUMBER;
    } else if (strcmp(field_type, "boolean") == 0) {
        return value_type_id == TYPE_BOOLEAN;
    } else if (strcmp(field_type, "identifier") == 0 || 
               find_custom_type(field_type) != NULL) {
        // Permettre l'affectation d'identifiants à des types personnalisés
        return value_type_id == TYPE_IDENTIFIER;
    }
    return 0; // Type incompatible
}

int check_primitive_type_compatibility(char* type_name, char* value, int value_type_id) {
    if (strcmp(type_name, "string") == 0) {
        return value_type_id == TYPE_STRING;
    } else if (strcmp(type_name, "number") == 0) {
        return value_type_id == TYPE_NUMBER;
    } else if (strcmp(type_name, "boolean") == 0) {
        return value_type_id == TYPE_BOOLEAN;
    }
    return 0; // Type incompatible
}
int check_variable_exists(const char* var_name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, var_name) == 0) {
            return 1; // La variable existe
        }
    }
    return 0; // La variable n'existe pas
}

bool parse_array_access(const char* input, char* base_name, int* index) {
    const char* open_bracket = strchr(input, '[');
    const char* close_bracket = strchr(input, ']');
    if (open_bracket && close_bracket && close_bracket > open_bracket) {
        strncpy(base_name, input, open_bracket - input);
        base_name[open_bracket - input] = '\0';

        char index_str[10];
        strncpy(index_str, open_bracket + 1, close_bracket - open_bracket - 1);
        index_str[close_bracket - open_bracket - 1] = '\0';
        *index = atoi(index_str);
        return true;
    }

    strcpy(base_name, input);
    *index = -1;
    return false;
}
char* get_value(const char* var_name, const char* field_name) {
    char var_base[100], field_base[100];
    int var_index = -1, field_index = -1;

    parse_array_access(var_name, var_base, &var_index);
    if (field_name)
        parse_array_access(field_name, field_base, &field_index);

    if (!check_variable_exists(var_base)) {
        printf("Variable '%s' non déclarée\n", var_base);
        return NULL;
    }

    variable* var = NULL;
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, var_base) == 0) {
            var = &variables[i];
            break;
        }
    }

    if (!var) return NULL;

    // Cas d'un tableau de type primitif
    if (var->is_array && var_index >= 0 && find_custom_type(var->type) == NULL) {
        // Copier la valeur pour manipulation
        char value_copy[1000];
        strncpy(value_copy, var->value, sizeof(value_copy) - 1);
        value_copy[sizeof(value_copy) - 1] = '\0';
        
        // Nettoyer les accolades ou crochets externes
        char* inner = value_copy;
        if (*inner == '{' || *inner == '[') inner++;
        if (inner[strlen(inner) - 1] == '}' || inner[strlen(inner) - 1] == ']') 
            inner[strlen(inner) - 1] = '\0';
        
        // Extraire les éléments du tableau
        int idx = 0;
        char* token = strtok(inner, ",");
        while (token != NULL) {
            char* trimmed = trim(token);
            if (idx == var_index) {
                // Si c'est une chaîne avec guillemets, nettoyer les guillemets
                if (trimmed[0] == '"' && trimmed[strlen(trimmed) - 1] == '"') {
                    trimmed[strlen(trimmed) - 1] = '\0';
                    return strdup(trimmed + 1);
                }
                return strdup(trimmed);
            }
            token = strtok(NULL, ",");
            idx++;
        }
        
        // Si l'indice est hors limites
        printf("Erreur : Index %d hors limites pour le tableau '%s'\n", var_index, var_base);
        return NULL;
    }

    // Cas d'un tableau d'objets personnalisés
    if (var->is_array && var_index >= 0 && find_custom_type(var->type) != NULL) {
        if (!field_name) {
            printf("Erreur : '%s[%d]' est un type structuré, un champ est requis.\n", var_base, var_index);
            return NULL;
        }

        custom_type* ctype = find_custom_type(var->type);
        if (!ctype) {
            printf("Type personnalisé introuvable : %s\n", var->type);
            return NULL;
        }

        // Copier la valeur pour manipulation
        char value_copy[1000];
        strncpy(value_copy, var->value, sizeof(value_copy) - 1);
        value_copy[sizeof(value_copy) - 1] = '\0';

        // Extraire l'objet à l'index spécifié
        char* p = value_copy;
        if (*p == '{') p++; // Sauter l'accolade externe d'ouverture
        
        int current_obj = 0;
        int brace_level = 0;
        char* obj_start = p;
        
        // Parcourir caractère par caractère pour trouver le bon objet
        while (*p) {
            if (*p == '{') brace_level++;
            else if (*p == '}') {
                brace_level--;
                if (brace_level == 0 && current_obj == var_index) {
                    // On a trouvé la fin de l'objet à l'index var_index
                    break;
                }
                else if (brace_level == 0) {
                    // On a fini un objet mais ce n'est pas celui qu'on cherche
                    current_obj++;
                    // Chercher le début du prochain objet
                    while (*(p+1) && (*(p+1) == ' ' || *(p+1) == ',' || *(p+1) == '\t' || *(p+1) == '\n')) p++;
                    obj_start = p + 1;
                }
            }
            p++;
        }
        
        if (current_obj != var_index) {
            printf("Erreur : Index %d hors limites pour le tableau '%s'\n", var_index, var_base);
            return NULL;
        }
        
        // Extraire l'objet trouvé
        char obj_copy[1000] = {0};
        int len = p - obj_start + 1;
        if (len > 0 && len < 1000) {
            strncpy(obj_copy, obj_start, len);
            obj_copy[len] = '\0';
            
            // Nettoyer les accolades externes de l'objet
            if (obj_copy[0] == '{') {
                memmove(obj_copy, obj_copy + 1, strlen(obj_copy));
            }
            if (obj_copy[strlen(obj_copy) - 1] == '}') {
                obj_copy[strlen(obj_copy) - 1] = '\0';
            }
            
            // Maintenant rechercher le champ dans cet objet
            char* pair = strtok(obj_copy, ",");
            while (pair) {
                char* equal = strchr(pair, '=');
                if (equal) {
                    *equal = '\0';
                    char* key = trim(pair);
                    char* val = trim(equal + 1);
                    
                    if (strcmp(key, field_base) == 0) {
                        // Nettoyer les guillemets si c'est une chaîne
                        if (val[0] == '"' && val[strlen(val) - 1] == '"') {
                            val[strlen(val) - 1] = '\0';
                            return strdup(val + 1);
                        }
                        return strdup(val);
                    }
                }
                pair = strtok(NULL, ",");
            }
            
            printf("Champ '%s' introuvable dans '%s[%d]'\n", field_base, var_base, var_index);
            return NULL;
        }
        
        printf("Erreur lors de l'extraction de l'objet à l'index %d\n", var_index);
        return NULL;
    }

    // Cas d'un objet personnalisé non-tableau
    if (find_custom_type(var->type) != NULL) {
        if (!field_name) {
            printf("Erreur : '%s' est un type structuré, un champ est requis.\n", var_base);
            return NULL;
        }

        custom_type* ctype = find_custom_type(var->type);
        if (!ctype) {
            printf("Type personnalisé introuvable : %s\n", var->type);
            return NULL;
        }

        // Copier la valeur pour manipulation
        char value_copy[1000];
        strncpy(value_copy, var->value, sizeof(value_copy) - 1);
        value_copy[sizeof(value_copy) - 1] = '\0';
        
        // Nettoyer les accolades externes
        char* inner = value_copy;
        if (*inner == '{') inner++;
        if (inner[strlen(inner) - 1] == '}') inner[strlen(inner) - 1] = '\0';
        
        // Rechercher le champ dans l'objet
        char* pair = strtok(inner, ",");
        while (pair) {
            char* equal = strchr(pair, '=');
            if (equal) {
                *equal = '\0';
                char* key = trim(pair);
                char* val = trim(equal + 1);
                
                if (strcmp(key, field_base) == 0) {
                    // Nettoyer les guillemets si c'est une chaîne
                    if (val[0] == '"' && val[strlen(val) - 1] == '"') {
                        val[strlen(val) - 1] = '\0';
                        return strdup(val + 1);
                    }
                    return strdup(val);
                }
            }
            pair = strtok(NULL, ",");
        }
        
        printf("Erreur : Impossible d'obtenir la valeur de %s.%s\n", var_base, field_base);
        return NULL;
    }

    // Cas d'une valeur primitive
    if (!field_name) {
        char* value_copy = strdup(var->value);
        if (value_copy[0] == '"' && value_copy[strlen(value_copy) - 1] == '"') {
            value_copy[strlen(value_copy) - 1] = '\0';
            char* result = strdup(value_copy + 1);
            free(value_copy);
            return result;
        }
        return value_copy;
    }

    printf("Erreur : '%s' est une primitive, champ '%s' invalide.\n", var_base, field_name);
    return NULL;
}
// Fonction utilitaire pour supprimer les espaces avant et après une chaîne
char* trim(char* str) {
    if (!str) return NULL;
    
    // Supprimer les espaces au début
    while (isspace(*str)) str++;
    
    if (*str == '\0') return str;
    
    // Supprimer les espaces à la fin
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
    
    return str;
}

int count_array_elements(const char* array_str) {
    if (strlen(array_str) <= 2) return 0; // Empty array {}
    
    int count = 1;  // Start with 1 for the first element
    int depth = 0;  // Track nested braces
    
    // Skip the first brace
    const char* ptr = array_str + 1;
    
    while (*ptr) {
        if (*ptr == '{') depth++;
        else if (*ptr == '}') depth--;
        else if (*ptr == ',' && depth == 0) count++;
        ptr++;
    }
    
    return count;
}

// Function to extract a single array element from a string
// index is passed by reference and updated for the next call
char* extract_array_element(char* array_str, int* index) {
    static char element_buffer[1024];
    
    // Skip leading whitespace and commas
    while (array_str[*index] == ' ' || array_str[*index] == ',') {
        (*index)++;
    }
    
    // Check if we've reached the end
    if (array_str[*index] == '\0') {
        return NULL;
    }
    
    int start = *index;
    int depth = 0;
    
    // Parse until we hit a comma at depth 0 (or end of string)
    while (array_str[*index]) {
        if (array_str[*index] == '{') depth++;
        else if (array_str[*index] == '}') {
            depth--;
            if (depth < 0) break; // End of the array
        }
        else if (array_str[*index] == ',' && depth == 0) break;
        
        (*index)++;
    }
    
    // Extract the element
    int len = *index - start;
    strncpy(element_buffer, array_str + start, len);
    element_buffer[len] = '\0';
    
    return element_buffer;
}

// Function to initialize a single custom type element in an array
void initialize_custom_type_element(const char* array_name, int index,char* type_name, const char* element_str) {
    char element_copy[1024];
    strcpy(element_copy, element_str);
    
    // Remove outer braces
    element_copy[0] = ' ';
    element_copy[strlen(element_copy)-1] = '\0';
    
    // Parse field assignments
    char* field_str = strtok(element_copy, ",");
    while (field_str != NULL) {
        // Trim whitespace
        while (*field_str == ' ') field_str++;
        
        // Find the equals sign
        char* equals = strchr(field_str, '=');
        if (equals != NULL) {
            *equals = '\0';
            char* field_name = field_str;
            char* field_value = equals + 1;
            
            // Trim whitespace
            while (*field_name == ' ') field_name++;
            while (*field_value == ' ') field_value++;
            
            // Trim trailing whitespace from field name
            char* end = field_name + strlen(field_name) - 1;
            while (end > field_name && *end == ' ') {
                *end = '\0';
                end--;
            }
            
            // Find field type in custom type definition
            custom_type* type = find_custom_type(type_name);
            char* field_type = NULL;
            
            for (int i = 0; i < type->field_count; i++) {
                if (strcmp(type->fields[i][0], field_name) == 0) {
                    field_type = type->fields[i][1];
                    break;
                }
            }
            
            // Generate assignment code based on field type
            if (field_type != NULL) {
                if (strcmp(field_type, "string") == 0) {
                    printf("\t%s[%d].%s = malloc(strlen(%s)+1);\n", array_name, index, field_name, field_value);
                    printf("\tstrcpy(%s[%d].%s, %s);\n", array_name, index, field_name, field_value);
                } else {
                    printf("\t%s[%d].%s = %s;\n", array_name, index, field_name, field_value);
                }
            }
        }
        
        field_str = strtok(NULL, ",");
    }
}


char* get_default_value(char* type_name) {
    if (type_name == NULL) {
        return NULL;
    }
    
    // Default values for primitive types
    if (strcmp(type_name, "int") == 0 || 
        strcmp(type_name, "float") == 0 || 
        strcmp(type_name, "double") == 0) {
        return strdup("0");
    } 
    else if (strcmp(type_name, "char") == 0) {
        return strdup("''");  // Empty character
    }
    else if (strcmp(type_name, "string") == 0) {
        return strdup("\"\"");  // Empty string
    }
    else if (strcmp(type_name, "bool") == 0 || 
             strcmp(type_name, "boolean") == 0) {
        return strdup("false");
    }
    
    // For user-defined types or unrecognized types
    // You might want to look up default values for custom types in a symbol table
    if (find_custom_type(type_name)) {
        return strdup("null");
    }
    
    // Default fallback
    return strdup("null");
}
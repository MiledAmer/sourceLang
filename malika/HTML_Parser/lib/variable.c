#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "variable.h"
#include "customType.h"

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

// Retourne la valeur d'une variable (primitive ou champ d'un type personnalisé)
// Ex: get_value("user", "name") --> retourne user.name ou la valeur si primitive
char* trim(char* str) {
    while (*str == ' ') str++;
    char* end = str + strlen(str) - 1;
    while (end > str && *end == ' ') *end-- = '\0';
    return str;
}

char* get_value(const char* var_name, const char* field_name) {
    if (!check_variable_exists(var_name)) {
        printf("Variable '%s' non déclarée\n", var_name);
        return NULL;
    }

    variable* var = NULL;
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, var_name) == 0) {
            var = &variables[i];
            break;
        }
    }

    if (!var) return NULL;

    // Type structuré
    if (find_custom_type(var->type) != NULL) {
        if (!field_name) {
            printf("Erreur : '%s' est un type structuré, un champ est requis.\n", var_name);
            return NULL;
        }

        custom_type* ctype = find_custom_type(var->type);
        if (!ctype) {
            printf("Type personnalisé introuvable : %s\n", var->type);
            return NULL;
        }

        bool field_found = false;
        for (int j = 0; j < ctype->field_count; j++) {
            if (strcmp(ctype->fields[j][0], field_name) == 0) {
                field_found = true;
                break;
            }
        }
        if (!field_found) {
            printf("Champ '%s' non défini dans le type '%s'\n", field_name, var->type);
            return NULL;
        }

        char value_copy[500];
        strncpy(value_copy, var->value, sizeof(value_copy));
        value_copy[sizeof(value_copy) - 1] = '\0';

        char* inner = value_copy;
        if (*inner == '{') inner++;
        if (inner[strlen(inner) - 1] == '}') inner[strlen(inner) - 1] = '\0';

        char* token = strtok(inner, ",");
        while (token) {
            char* equal = strchr(token, '=');
            if (equal) {
                *equal = '\0';
                char* key = trim(token);
                char* val = trim(equal + 1);

                if (strcmp(key, field_name) == 0) {
                    if (val[0] == '"' && val[strlen(val) - 1] == '"') {
                        val[strlen(val) - 1] = '\0';
                        return strdup(val + 1);
                    }
                    return strdup(val);
                }
            }
            token = strtok(NULL, ",");
        }

        printf("Champ '%s' introuvable dans la variable '%s'\n", field_name, var_name);
        return NULL;
    }

    // Primitive
    if (!field_name) {
        char* value_copy = strdup(var->value);
        if (value_copy[0] == '"' && value_copy[strlen(value_copy) - 1] == '"') {
            value_copy[strlen(value_copy) - 1] = '\0';
            char* result = strdup(value_copy + 1);
            free(value_copy);  // Libérer la copie temporaire
            return result;
        }
        return value_copy;  // Déjà une copie, pas besoin de strdup à nouveau
    }

    printf("Erreur : '%s' est une variable primitive, pas un type structuré.\n", var_name);
    return NULL;
}


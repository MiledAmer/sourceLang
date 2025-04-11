#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "variable.h"
#include "customType.h"

variable variables[100]; // Tableau pour stocker les variables
int var_count ;

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

int check_primitive_type_compatibility(const char* type_name, const char* value) {
    if (strcmp(type_name, "string") == 0) {
        return value[0] == '"'; // Simple check for string literal
    } else if (strcmp(type_name, "number") == 0 || strcmp(type_name, "int") == 0) {
        // Check if value is a number
        char* endptr;
        strtod(value, &endptr);
        return *endptr == '\0';
    } else if (strcmp(type_name, "boolean") == 0) {
        return strcmp(value, "true") == 0 || strcmp(value, "false") == 0;
    }
    return 0;
}
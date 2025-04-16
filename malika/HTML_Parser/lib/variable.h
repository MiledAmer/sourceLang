#ifndef vARIABLE_H
#define vARIABLE_H
#include "customType.h"
#include <stdbool.h>

// Constantes pour les types de valeurs
#define TYPE_STRING    1
#define TYPE_NUMBER    2
#define TYPE_BOOLEAN   3
#define TYPE_IDENTIFIER 4
#define MAX_FIELDS 50
#define MAX_VALUES 50
#define MAX_ARRAY_ELEMENTS 100
#define MAX_ARRAY_VALUES 100

// Structure pour stocker les variables
typedef struct {
    char name[50];
    char type[50];
    char value[500];
    int is_array;
} variable;

extern variable variables[100]; // Tableau pour stocker les variables
extern int var_count ;

extern char field_names[MAX_FIELDS][256];  // Noms des champs
extern char field_values[MAX_VALUES][256]; // Valeurs des champs
extern int field_types[MAX_VALUES];        // Types des valeurs (string, number, boolean, identifier)
extern int field_count;                // Nombre de champs traités
extern int value_count ;                // Nombre de valeurs traitées
extern char custom_array_elements[MAX_ARRAY_ELEMENTS][1024]; // Larger size for custom type objects
extern int custom_array_element_count ;
extern char* array_values[MAX_ARRAY_VALUES];
extern int array_value_count;

int add_variable(char* name, char* type, char* value, int is_array);
void declare_variables();
bool verify_type(char* type);
custom_type* find_custom_type(char* type_name);
int check_field_value_compatibility(char* field_type, char* value, int value_type_id);
int check_primitive_type_compatibility(char* type_name, char* value, int value_type_id);
int check_variable_exists(const char* var_name);
char* trim(char* str);
int check_array_value_compatibility(char* type_name, char* value, int value_type_id);
bool parse_array_access(const char* input, char* base_name, int* index);
char* get_value(const char* var_name, const char* field_name);
int count_array_elements(const char* array_str);
char* extract_array_element(char* array_str, int* index);
void initialize_custom_type_element(const char* array_name, int index, char* type_name, const char* element_str);
char* get_default_value(char* type_name);
#endif 
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


int add_variable(char* name, char* type, char* value, int is_array);
void declare_variables();
bool verify_type(char* type);
custom_type* find_custom_type(char* type_name);
int check_field_value_compatibility(char* field_type, char* value, int value_type_id);
int check_primitive_type_compatibility(char* type_name, char* value, int value_type_id);
int check_variable_exists(const char* var_name);
char* get_value(const char* var_name, const char* field_name);

#endif 
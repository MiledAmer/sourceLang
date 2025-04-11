#ifndef vARIABLE_H
#define vARIABLE_H
#include "customType.h"

// Structure pour stocker les variables
typedef struct {
    char name[50];
    char type[50];
    char value[500];
    int is_array;
} variable;

extern variable variables[100]; // Tableau pour stocker les variables
extern int var_count ;

bool verify_type(char* type);
custom_type* find_custom_type(char* type_name);
bool check_field_value_compatibility(char* field_type, char* value);
int add_variable(char* name, char* type, char* value, int is_array);
void declare_variables();
int check_primitive_type_compatibility(const char* type_name, const char* value);

#endif 
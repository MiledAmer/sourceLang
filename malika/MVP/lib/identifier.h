#ifndef IDENTIFIER_H
#define IDENTIFIER_H


// Structure pour les identifiants typés
typedef struct {
    char* name;
    char* type;
} TypedIdentifier;

extern TypedIdentifier identifiers[100];
extern char current_component_name[100]; 
extern int id_count ;
void add_identifier(const char* name, const char* type);
char* get_identifier_type(const char* name);
#endif
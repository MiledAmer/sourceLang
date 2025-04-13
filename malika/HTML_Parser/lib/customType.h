#ifndef CUSTOMTYPE_H
#define CUSTOMTYPE_H

// Structure pour les types personnalisés
typedef struct {
    char name[50];
    char fields[10][2][50]; // [nombre_de_champs][nom_ou_type][chaine]
    int field_count;
} custom_type;

extern char fields[10][2][50]; // For storing field names and types during type definition
extern custom_type custom_types[20]; // Tableau pour stocker les types personnalisés
extern int value_count ; 
extern int field_count;  
extern int type_count ;

void add_custom_type(char* name) ;
void add_field_to_type(char* field_name, char* field_type) ;
void generate_structs_and_prototypes() ;

#endif 
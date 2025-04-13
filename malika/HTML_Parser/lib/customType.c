#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customType.h"
#include "variable.h"

char fields[10][2][50]; // For storing field names and types during type definition
custom_type custom_types[20]; // Tableau pour stocker les types personnalisés
int type_count = 0;


void add_custom_type(char* name) {
    strcpy(custom_types[type_count].name, name);
    custom_types[type_count].field_count = field_count;
    
    // Copier tous les champs
    for (int i = 0; i < field_count; i++) {
        strcpy(custom_types[type_count].fields[i][0], fields[i][0]); // nom
        strcpy(custom_types[type_count].fields[i][1], fields[i][1]); // type
    }
    
    type_count++;
    
    // Réinitialiser le compteur pour le prochain type
    field_count = 0;
}

void add_field_to_type(char* field_name, char* field_type) {
    if (type_count <= 0) {
        printf("ERREUR: Tentative d'ajout d'un champ sans type défini\n");
        fflush(stdout);
        return;
    }
    
    strcpy(custom_types[type_count-1].fields[custom_types[type_count-1].field_count][0], field_name);
    strcpy(custom_types[type_count-1].fields[custom_types[type_count-1].field_count][1], field_type);
    custom_types[type_count-1].field_count++;
}

void generate_structs_and_prototypes() {
    for (int i = 0; i < type_count; i++) {
        printf("typedef struct {\n");
        fflush(stdout);
        
        for (int j = 0; j < custom_types[i].field_count; j++) {
            char* field_type = custom_types[i].fields[j][1];
            if (strcmp(field_type, "string") == 0) {
                printf("    char* ");
            } else {
                printf("    ");
                printf(field_type);
                printf(" ");
            }
            printf(custom_types[i].fields[j][0]);
            printf(";\n");
            fflush(stdout);
        }
        
        printf("} ");
        printf(custom_types[i].name);
        printf(";\n");
        fflush(stdout);
    }
}

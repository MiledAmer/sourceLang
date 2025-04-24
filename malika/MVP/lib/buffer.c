#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

DynamicBuffer import_buffer = {NULL, 0, 0};
DynamicBuffer interfaces_buffer = {NULL, 0, 0};
DynamicBuffer output_buffer = {NULL, 0, 0};

// Fonction pour initialiser un buffer
void buffer_init(DynamicBuffer* buffer) {
    buffer->capacity = 256; // Taille initiale
    buffer->size = 0;
    buffer->data = (char*)malloc(buffer->capacity);
    if (!buffer->data) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(1);
    }
    buffer->data[0] = '\0';
}

// Fonction pour ajouter du contenu au buffer avec redimensionnement automatique
void buffer_append(DynamicBuffer* buffer, const char* content) {
    size_t content_len = strlen(content);
    size_t new_size = buffer->size + content_len;
    
    // Si besoin de plus d'espace, réallocation
    if (new_size >= buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        while (new_size >= new_capacity) {
            new_capacity *= 2;
        }
        
        char* new_data = (char*)realloc(buffer->data, new_capacity);
        if (!new_data) {
            fprintf(stderr, "Erreur de réallocation mémoire\n");
            exit(1);
        }
        
        buffer->data = new_data;
        buffer->capacity = new_capacity;
    }
    
    // Copier le nouveau contenu
    strcpy(buffer->data + buffer->size, content);
    buffer->size = new_size;
}

// Fonction pour libérer le buffer
void buffer_free(DynamicBuffer* buffer) {
    free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
}

// Pour initialiser les buffers dans main
void initialize_buffers() {
    buffer_init(&import_buffer);
    buffer_init(&interfaces_buffer);
    buffer_init(&output_buffer);
}

// Pour libérer les buffers
void cleanup_buffers() {
    buffer_free(&import_buffer);
    buffer_free(&interfaces_buffer);
    buffer_free(&output_buffer);
}


// Générer le code C pour output_buffer (pour la version statique)
void generate_output_buffer_code() {
    printf("\n// Définition du buffer de sortie\n");
    printf("const char* generate_output_buffer() {\n");
    printf("    static char buffer[] = \n");
    
    // Traiter le contenu ligne par ligne pour une meilleure lisibilité
    const char* content = output_buffer.data;
    printf("        \"");
    
    for (size_t i = 0; i < output_buffer.size; i++) {
        if (content[i] == '\n') {
            printf("\\n\"\n        \"");
        } else if (content[i] == '"') {
            printf("\\\"");
        } else if (content[i] == '\\') {
            printf("\\\\");
        } else {
            printf("%c", content[i]);
        }
    }
    
    printf("\";\n");
    printf("    return buffer;\n");
    printf("}\n\n");
}
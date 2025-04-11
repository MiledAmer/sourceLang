#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"

ImportedComponent imported_components[MAX_IMPORTED_COMPONENTS];
int imported_count = 0;

char* find_imported_component(const char* name) {
    // printf("Recherche du composant importé: %s\n", name);
    for (int i = 0; i < imported_count; i++) {
        if (strcmp(imported_components[i].name, name) == 0) {
            // printf("Composant trouvé: %s\n", name);
            // printf("Contenu: %s\n", imported_components[i].html_content);
            return imported_components[i].html_content;
        }
    }
    return NULL;
}


// Fonction pour ajouter un composant importé
void add_imported_component(const char* name, const char* html_content, const char* path) {
    // Vérifier si le composant existe déjà
    for (int i = 0; i < imported_count; i++) {
        if (strcmp(imported_components[i].name, name) == 0) {
            // Remplacer le contenu existant
            free(imported_components[i].html_content);
            imported_components[i].html_content = strdup(html_content);
            return;
        }
    }
    
    // Ajouter un nouveau composant
    if (imported_count < MAX_IMPORTED_COMPONENTS) {
        imported_components[imported_count].name = strdup(name);
        imported_components[imported_count].html_content = strdup(html_content);
        imported_components[imported_count].Path = strdup(path);
        imported_count++;
    } else {
        fprintf(stderr, "Error: Maximum number of imported components reached\n");
    }
}

void process_import(char* component, char* path) {
    // Enlever les guillemets du chemin
    char real_path[256];
    strncpy(real_path, path + 1, strlen(path) - 2);  // Remove beginning and ending quotes
    real_path[strlen(path) - 2] = '\0';
    
    // Vérifier si le chemin est relatif et ajouter le ./ si nécessaire
    if (real_path[0] != '/' && strncmp(real_path, "./", 2) != 0) {
        char temp[256];
        sprintf(temp, "./%s", real_path);
        strcpy(real_path, temp);
    }
    
    // printf("Traitement de l'import du composant %s depuis %s\n", component, real_path);
    
    // Ouvrir le fichier source
    FILE* imported_file = fopen(real_path, "r");
    if (!imported_file) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", real_path);
        exit(1);
    }
    
    // Lire le contenu du fichier
    char buffer[10000] = {0};
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, imported_file);
    buffer[bytes_read] = '\0';  // Assurer que la chaîne est terminée par null
    
    // printf("Contenu brut du fichier importé:\n%s\n", buffer);
    
    // Analyser manuellement le contenu pour extraire le HTML du composant
    // Ceci est une version simplifiée qui suppose que le composant est correctement formaté
    char* start = strstr(buffer, "component");
    if (start) {
        start = strstr(start, "{");
        if (start) {
            char* return_stmt = strstr(start, "return");
            if (return_stmt) {
                char* open_paren = strstr(return_stmt, "(");
                if (open_paren) {
                    char* close_paren = strrchr(buffer, ')');  // Trouver la dernière parenthèse fermante
                    if (close_paren) {
                        // Extraire le contenu HTML entre les parenthèses de l'instruction return
                        int html_length = close_paren - (open_paren + 1);
                        if (html_length > 0 && html_length < 9000) {
                            char html_content[10000] = {0};
                            strncpy(html_content, open_paren + 1, html_length);
                            html_content[html_length] = '\0';
                            
                            // Créer un div englobant avec le nom du composant comme classe
                            char final_html[10000] = {0};
                            sprintf(final_html, "<div id='%s'>\n%s\n</div>", component, html_content);
                            
                            // Ajouter le composant importé à notre registre
                            add_imported_component(component, final_html, real_path);
                            // printf("Composant importé %s ajouté avec le contenu:\n%s\n", component, final_html);
                        }
                    }
                }
            }
        }
    }
    
    // Si l'analyse manuelle n'a pas fonctionné, créer un composant minimal
    if (find_imported_component(component) == NULL) {
        char minimal_html[1000];
        sprintf(minimal_html, "<div id='%s'><!-- Contenu du composant %s non analysé --></div>", 
                component, component);
        add_imported_component(component, minimal_html, real_path);
        // printf("Impossible d'analyser le composant %s. Utilisation d'un composant minimal.\n", component);
    }
    
    // Fermer le fichier
    fclose(imported_file);
    
    // printf("Import du composant %s terminé\n", component);
}

void initialize_imported_components() {
    // Initialiser le stockage des composants importés
    for (int i = 0; i < MAX_IMPORTED_COMPONENTS; i++) {
        imported_components[i].name = NULL;
        imported_components[i].html_content = NULL;
        imported_components[i].Path = NULL;
    }
}

void free_imported_components() {
    // Libérer la mémoire allouée pour les composants importés
    for (int i = 0; i < imported_count; i++) {
        free(imported_components[i].name);
        free(imported_components[i].html_content);
        free(imported_components[i].Path);
    }
    imported_count = 0;
}
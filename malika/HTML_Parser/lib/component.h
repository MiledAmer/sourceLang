#ifndef COMPONENT_H
#define COMPONENT_H

#define MAX_IMPORTED_COMPONENTS 50

typedef struct {
    char* name;
    char* html_content;
    char* Path;
} ImportedComponent;

extern ImportedComponent imported_components[MAX_IMPORTED_COMPONENTS];
extern int imported_count;

char* find_imported_component(const char* name);
void add_imported_component(const char* name, const char* html_content, const char* path);
void process_import(char* component, char* path);
void free_imported_components();
void initialize_imported_components();

#endif

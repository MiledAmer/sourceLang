#ifndef COMPONENT_H
#define COMPONENT_H

#define MAX_IMPORTED_COMPONENTS 50
#define MAX_PROPS_PER_COMPONENT 20

typedef struct {
    char* name;
    char* html_content;
    char* Path;
    char** prop_names;    // Array of property names
    char** prop_types;    // Array of property types
    int prop_count;       // Number of properties
} ImportedComponent;

// Define a structure for props in your parser header
typedef struct prop_value {
    char* name;
    char* value;
} prop_value;

extern ImportedComponent imported_components[MAX_IMPORTED_COMPONENTS];
extern int imported_count;

char* find_imported_component(const char* name);
void add_imported_component(const char* name, const char* html_content, const char* path);
void extract_component_props(const char* component_name, const char* buffer);
void process_import(char* component, char* path);
void free_imported_components();
void initialize_imported_components();
void free_props(struct prop_value* props, int count);
struct prop_value* parse_props(const char* str, int* count) ;
char* apply_props_to_component(const char* component_name, struct prop_value* props, int prop_count) ;
#endif

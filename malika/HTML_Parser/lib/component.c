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

// Update the add_imported_component function
void add_imported_component(const char* name, const char* html_content, const char* path) {
    // Check if component already exists
    for (int i = 0; i < imported_count; i++) {
        if (strcmp(imported_components[i].name, name) == 0) {
            // Replace existing content
            free(imported_components[i].html_content);
            imported_components[i].html_content = strdup(html_content);
            return;
        }
    }
    
    // Add new component
    if (imported_count < MAX_IMPORTED_COMPONENTS) {
        imported_components[imported_count].name = strdup(name);
        imported_components[imported_count].html_content = strdup(html_content);
        imported_components[imported_count].Path = strdup(path);
        imported_components[imported_count].prop_names = malloc(sizeof(char*) * MAX_PROPS_PER_COMPONENT);
        imported_components[imported_count].prop_types = malloc(sizeof(char*) * MAX_PROPS_PER_COMPONENT);
        imported_components[imported_count].prop_count = 0;
        imported_count++;
    } else {
        fprintf(stderr, "Error: Maximum number of imported components reached\n");
    }
}

// Function to extract props from component declaration
void extract_component_props(const char* component_name, const char* buffer) {
    int component_index = -1;
    
    // Find the component in our array
    for (int i = 0; i < imported_count; i++) {
        if (strcmp(imported_components[i].name, component_name) == 0) {
            component_index = i;
            break;
        }
    }
    
    if (component_index == -1) return;
    
    // Find the component declaration
    char* component_decl = strstr(buffer, "component");
    if (!component_decl) return;
    
    // Find the opening parenthesis for props
    char* props_start = strchr(component_decl, '(');
    if (!props_start) return;
    
    // Find the closing parenthesis
    char* props_end = strchr(props_start, ')');
    if (!props_end) return;
    
    // Extract the props string
    int props_len = props_end - props_start - 1;
    char props_str[1000] = {0};
    strncpy(props_str, props_start + 1, props_len);
    
    // Parse the props
    char* token = strtok(props_str, ",");
    while (token != NULL && imported_components[component_index].prop_count < MAX_PROPS_PER_COMPONENT) {
        // Trim spaces
        while (*token == ' ') token++;
        
        // Extract prop name and type
        char* colon = strchr(token, ':');
        if (colon) {
            int name_len = colon - token;
            char prop_name[256] = {0};
            strncpy(prop_name, token, name_len);
            
            // Trim trailing spaces from name
            int i = name_len - 1;
            while (i >= 0 && prop_name[i] == ' ') {
                prop_name[i] = '\0';
                i--;
            }
            
            // Get prop type
            char* type = colon + 1;
            while (*type == ' ') type++; // Skip leading spaces
            
            // Store prop name and type
            int prop_idx = imported_components[component_index].prop_count;
            imported_components[component_index].prop_names[prop_idx] = strdup(prop_name);
            imported_components[component_index].prop_types[prop_idx] = strdup(type);
            imported_components[component_index].prop_count++;
        }
        
        token = strtok(NULL, ",");
    }
}

// Update process_import to extract props
void process_import(char* component, char* path) {
    // Remove quotes from path
    char real_path[256];
    strncpy(real_path, path + 1, strlen(path) - 2);  // Remove beginning and ending quotes
    real_path[strlen(path) - 2] = '\0';
    
    // Check if path is relative and add ./ if needed
    if (real_path[0] != '/' && strncmp(real_path, "./", 2) != 0) {
        char temp[256];
        sprintf(temp, "./%s", real_path);
        strcpy(real_path, temp);
    }
    
    // Open source file
    FILE* imported_file = fopen(real_path, "r");
    if (!imported_file) {
        fprintf(stderr, "Error opening file %s\n", real_path);
        exit(1);
    }
    
    // Read file content
    char buffer[10000] = {0};
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, imported_file);
    buffer[bytes_read] = '\0';
    
    // Parse manually to extract component HTML
    char* start = strstr(buffer, "component");
    if (start) {
        start = strstr(start, "{");
        if (start) {
            char* return_stmt = strstr(start, "return");
            if (return_stmt) {
                char* open_paren = strstr(return_stmt, "(");
                if (open_paren) {
                    char* close_paren = strrchr(buffer, ')');
                    if (close_paren) {
                        int html_length = close_paren - (open_paren + 1);
                        if (html_length > 0 && html_length < 9000) {
                            char html_content[10000] = {0};
                            strncpy(html_content, open_paren + 1, html_length);
                            html_content[html_length] = '\0';
                            
                            // Create wrapper div with component name as class
                            char final_html[10000] = {0};
                            sprintf(final_html, "<div id='%s'>\n%s\n</div>", component, html_content);
                            
                            // Add imported component to registry
                            add_imported_component(component, final_html, real_path);
                            
                            // Extract props
                            extract_component_props(component, buffer);
                        }
                    }
                }
            }
        }
    }
    
    // If manual parsing didn't work, create minimal component
    if (find_imported_component(component) == NULL) {
        char minimal_html[1000];
        sprintf(minimal_html, "<div id='%s'><!-- Content of component %s not parsed --></div>", 
                component, component);
        add_imported_component(component, minimal_html, real_path);
    }
    
    // Close file
    fclose(imported_file);
}


// Function to initialize component structures
void initialize_imported_components() {
    // Initialize imported components storage
    for (int i = 0; i < MAX_IMPORTED_COMPONENTS; i++) {
        imported_components[i].name = NULL;
        imported_components[i].html_content = NULL;
        imported_components[i].Path = NULL;
        imported_components[i].prop_names = NULL;
        imported_components[i].prop_types = NULL;
        imported_components[i].prop_count = 0;
    }
}

// Update free_imported_components to free prop memory
void free_imported_components() {
    // Free memory allocated for imported components
    for (int i = 0; i < imported_count; i++) {
        free(imported_components[i].name);
        free(imported_components[i].html_content);
        free(imported_components[i].Path);
        
        // Free props
        for (int j = 0; j < imported_components[i].prop_count; j++) {
            free(imported_components[i].prop_names[j]);
            free(imported_components[i].prop_types[j]);
        }
        free(imported_components[i].prop_names);
        free(imported_components[i].prop_types);
    }
    imported_count = 0;
}

// Function to parse props from a string
struct prop_value* parse_props(const char* props_str, int* count) {
    struct prop_value* props = malloc(sizeof(struct prop_value) * MAX_PROPS_PER_COMPONENT);
    *count = 0;
    
    // Make a copy of the string for tokenization
    char* str_copy = strdup(props_str);
    char* token = strtok(str_copy, " ");
    
    while (token != NULL && *count < MAX_PROPS_PER_COMPONENT) {
        // Find prop name and value (format: name=value)
        char* equals = strchr(token, '=');
        if (equals) {
            // Extract name
            int name_len = equals - token;
            props[*count].name = malloc(name_len + 1);
            strncpy(props[*count].name, token, name_len);
            props[*count].name[name_len] = '\0';
            
            // Extract value
            char* value = equals + 1;  // Skip equals sign
            
            // Handle braced values {value}
            if (value[0] == '{' && value[strlen(value)-1] == '}') {
                // Extract content within braces
                int value_len = strlen(value) - 2;  // Exclude braces
                props[*count].value = malloc(value_len + 1);
                strncpy(props[*count].value, value + 1, value_len);
                props[*count].value[value_len] = '\0';
            } else {
                // Regular value (string literal, number, etc.)
                props[*count].value = strdup(value);
            }
            
            (*count)++;
        }
        
        token = strtok(NULL, " ");
    }
    
    free(str_copy);
    return props;
}

// Function to free props memory
void free_props(struct prop_value* props, int count) {
    for (int i = 0; i < count; i++) {
        free(props[i].name);
        free(props[i].value);
    }
    free(props);
}

// Function to apply props to component HTML
char* apply_props_to_component(const char* component_name, struct prop_value* props, int prop_count) {
    // Find the component
    char* html = NULL;
    
    // Get the component's HTML content
    for (int i = 0; i < imported_count; i++) {
        if (strcmp(imported_components[i].name, component_name) == 0) {
            html = strdup(imported_components[i].html_content);
            break;
        }
    }
    
    if (!html) return NULL;
    
    // Replace prop placeholders with values
    for (int i = 0; i < prop_count; i++) {
        char placeholder[256];
        sprintf(placeholder, "{%s}", props[i].name);
        
        // Find and replace all occurrences of the placeholder
        char* pos = strstr(html, placeholder);
        while (pos != NULL) {
            // Calculate lengths
            int prefix_len = pos - html;
            int suffix_len = strlen(pos + strlen(placeholder));
            int new_len = prefix_len + strlen(props[i].value) + suffix_len + 1;
            
            // Create new string with replacement
            char* new_html = malloc(new_len);
            if (!new_html) {
                fprintf(stderr, "Memory allocation error\n");
                free(html);
                return NULL;
            }
            
            // Copy parts with replacement
            strncpy(new_html, html, prefix_len);
            new_html[prefix_len] = '\0';
            strcat(new_html, props[i].value);
            strcat(new_html, pos + strlen(placeholder));
            
            // Replace old html with new
            free(html);
            html = new_html;
            
            // Find next occurrence
            pos = strstr(html, placeholder);
        }
    }
    
    return html;
}
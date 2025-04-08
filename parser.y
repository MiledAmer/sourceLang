%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int yylex();
void yyerror(const char *s);

#define YYDEBUG 1
char identifiers[100][50];      // Stocke les identifiants rencontrés
char types[100][50];            // Stocke les types rencontrés
int id_count = 0;               // Compteur des identifiants
int found = 0;

char output_buffer[10000]; // Buffer pour le code généré
int buffer_index = 0; // Index pour le buffer


char output_buffer[10000]; // Buffer pour le code généré
int buffer_index = 0; // Index pour le buffer

/* Structure de la pile pour les tags HTML */
typedef struct node_tag {
    char* tag_name;
    struct node_tag* next;
} node_tag;

node_tag* pile_tags = NULL;

/* Ajout d'une structure pour stocker les propriétés d'un composant */
typedef struct {
    char name[50];
    char type[50];
    char value[500];
} ComponentParam;

/* Structure pour l'arbre de syntaxe abstraite */
typedef enum {
    NODE_ELEMENT,
    NODE_TEXT,
    NODE_PROP
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char tag[50];               // pour NODE_ELEMENT (ex: "div", "h1")
    char text[200];             // pour NODE_TEXT (texte brut ou placeholder "{{userName}}")
    char prop_name[50];         // pour NODE_PROP : nom de la prop à insérer
    struct ASTNode* children[10];
    int child_count;
} ASTNode;

// Variable globale pour stocker l'AST en cours de traitement
ASTNode* parsed_ast = NULL;
char current_component_name[50] = {0};

// Structure pour stocker les variables
typedef struct {
    char name[50];
    char type[50];
    char value[500];
    int is_array;
    int array_size;  // Ajout de la taille du tableau
} Variable;

Variable variables[100]; // Tableau pour stocker les variables
int var_count = 0;

// Structure pour stocker les AST des composants importés
typedef struct {
    char name[50];  // Nom du composant (ex: "CardComponent")
    ASTNode* ast;   // AST racine du composant importé
} ComponentAST;

ComponentAST component_asts[20];
int component_ast_count = 0;

// Structure pour stocker les types personnalisés
typedef struct {
    char name[50];
    char fields[10][2][50]; // [nombre_de_champs][nom_ou_type][chaine]
    int field_count;
} CustomType;

CustomType custom_types[20]; // Tableau pour stocker les types personnalisés
int type_count = 0;

int needs_assert = 0;
int needs_complex = 0;
int needs_ctype = 0;
int needs_errno = 0;
int needs_fenv = 0;
int needs_float = 0;
int needs_inttypes = 0;
int needs_limits = 0;
int needs_locale = 0;
int needs_math = 0;
int needs_setjmp = 0;
int needs_signal = 0;
int needs_stdio = 0;
int needs_stdlib = 0;
int needs_string = 0;
int needs_time = 0;
int needs_wchar = 0;
int needs_wctype = 0;
int needs_tgmath = 0;
int needs_stddef = 0;
int needs_stdbool = 0;
int needs_stdarg = 0;
int needs_stdalign = 0;
int needs_iso646 = 0;
int needs_unistd = 0;
int needs_fcntl = 0;
int needs_threads = 0;

// Fonction pour ajouter du texte au buffer
void append_to_buffer(const char *text) {
    snprintf(output_buffer + buffer_index, sizeof(output_buffer) - buffer_index, "%s", text);
    buffer_index += strlen(text);
}

// Fonction pour générer les includes
void generate_includes() {
    printf("/* Includes automatiques */\n");

    if (needs_assert) printf("#include <assert.h>\n");
    if (needs_complex) printf("#include <complex.h>\n");
    if (needs_ctype) printf("#include <ctype.h>\n");
    if (needs_errno) printf("#include <errno.h>\n");
    if (needs_fenv) printf("#include <fenv.h>\n");
    if (needs_float) printf("#include <float.h>\n");
    if (needs_inttypes) printf("#include <inttypes.h>\n");
    if (needs_limits) printf("#include <limits.h>\n");
    if (needs_locale) printf("#include <locale.h>\n");
    if (needs_math) printf("#include <math.h>\n");
    if (needs_setjmp) printf("#include <setjmp.h>\n");
    if (needs_signal) printf("#include <signal.h>\n");
    if (needs_stdio) printf("#include <stdio.h>\n");
    if (needs_stdlib) printf("#include <stdlib.h>\n");
    if (needs_string) printf("#include <string.h>\n");
    if (needs_threads) printf("#include <threads.h>\n");
    if (needs_time) printf("#include <time.h>\n");
    if (needs_wchar) printf("#include <wchar.h>\n");
    if (needs_wctype) printf("#include <wctype.h>\n");
    if (needs_tgmath) printf("#include <tgmath.h>\n");
    if (needs_stddef) printf("#include <stddef.h>\n");
    if (needs_stdbool) printf("#include <stdbool.h>\n");
    if (needs_stdarg) printf("#include <stdarg.h>\n");
    if (needs_stdalign) printf("#include <stdalign.h>\n");
    if (needs_iso646) printf("#include <iso646.h>\n");


    printf("\n"); // Space between includes and code
}

// Analyse des dépendances en fonction du contenu des identifiants et types
void analyze_dependencies() {
    // Dépendances essentielles pour le fonctionnement de base
    needs_stdio = 1;  // Pour printf
    needs_stdlib = 1; // Pour malloc/free
    needs_string = 1; // Pour manipulations de chaînes
    
    // Analyse des types pour détecter des dépendances spécifiques
    for (int i = 0; i < id_count; i++) {
        if (strcmp(types[i], "float") == 0 || strcmp(types[i], "double") == 0) {
            needs_math = 1;
        }
        else if (strcmp(types[i], "complex") == 0) {
            needs_complex = 1;
        }
        else if (strcmp(types[i], "bool") == 0) {
            needs_stdbool = 1;
        }
        else if (strcmp(types[i], "wchar_t") == 0) {
            needs_wchar = 1;
        }
        else if (strstr(types[i], "time") != NULL) {
            needs_time = 1;
        }
        else if (strstr(types[i], "int") != NULL) {
            needs_limits = 1; // Pour les limites de int
        }
    }
    
    // Analyse du contenu du buffer pour détecter d'autres dépendances
    if (strstr(output_buffer, "isalpha") != NULL || 
        strstr(output_buffer, "isdigit") != NULL || 
        strstr(output_buffer, "tolower") != NULL) {
        needs_ctype = 1;
    }
    
    if (strstr(output_buffer, "malloc") != NULL || 
        strstr(output_buffer, "free") != NULL || 
        strstr(output_buffer, "exit") != NULL) {
        needs_stdlib = 1;
    }
    
    if (strstr(output_buffer, "sin") != NULL || 
        strstr(output_buffer, "cos") != NULL || 
        strstr(output_buffer, "sqrt") != NULL) {
        needs_math = 1;
    }
    
    if (strstr(output_buffer, "printf") != NULL || 
        strstr(output_buffer, "scanf") != NULL || 
        strstr(output_buffer, "fprintf") != NULL) {
        needs_stdio = 1;
    }
    
    if (strstr(output_buffer, "strcpy") != NULL || 
        strstr(output_buffer, "strlen") != NULL || 
        strstr(output_buffer, "strcat") != NULL) {
        needs_string = 1;
    }
    
    if (strstr(output_buffer, "assert") != NULL) {
        needs_assert = 1;
    }
    
    if (strstr(output_buffer, "errno") != NULL) {
        needs_errno = 1;
    }
    
    if (strstr(output_buffer, "setjmp") != NULL || 
        strstr(output_buffer, "longjmp") != NULL) {
        needs_setjmp = 1;
    }
    
    if (strstr(output_buffer, "signal") != NULL) {
        needs_signal = 1;
    }
    
    if (strstr(output_buffer, "open") != NULL || 
        strstr(output_buffer, "close") != NULL || 
        strstr(output_buffer, "read") != NULL || 
        strstr(output_buffer, "write") != NULL) {
        needs_unistd = 1;
    }
    
    if (strstr(output_buffer, "O_RDONLY") != NULL || 
        strstr(output_buffer, "O_WRONLY") != NULL || 
        strstr(output_buffer, "O_CREAT") != NULL) {
        needs_fcntl = 1;
    }
    
    if (strstr(output_buffer, "thrd_") != NULL || 
        strstr(output_buffer, "mtx_") != NULL || 
        strstr(output_buffer, "cnd_") != NULL) {
        needs_threads = 1;
    }
}

// Fonction pour ajouter du texte au buffer
void append_to_buffer(const char *text) {
    snprintf(output_buffer + buffer_index, sizeof(output_buffer) - buffer_index, "%s", text);
    buffer_index += strlen(text);
}

/* Fonction pour empiler un tag */
void empiler_tag(char* tag_name) {
    node_tag* nouveau = (node_tag*)malloc(sizeof(node_tag));
    if (nouveau == NULL) {
        fprintf(stderr, "Erreur: impossible d'allouer de la mémoire pour un tag\n");
        exit(1);
    }
    nouveau->tag_name = strdup(tag_name);  // Dupliquer la chaîne pour la stocker
    nouveau->next = pile_tags;
    pile_tags = nouveau;
}

/* Fonction pour dépiler et vérifier un tag */
int verifier_tag_fermant(char* tag_name) {
    if (pile_tags == NULL) {
        fprintf(stderr, "Erreur: balise fermante %s sans balise ouvrante correspondante\n", tag_name);
        return 0;
    }
    
    if (strcmp(pile_tags->tag_name, tag_name) == 0) {
        node_tag* tmp = pile_tags;
        pile_tags = pile_tags->next;
        free(tmp->tag_name);
        free(tmp);
        return 1;
    } else {
        fprintf(stderr, "Erreur: balise fermante %s ne correspond pas à la dernière balise ouvrante %s\n", 
                tag_name, pile_tags->tag_name);
        return 0;
    }
}

/* Fonction pour libérer la pile à la fin */
void liberer_pile() {
    while (pile_tags != NULL) {
        node_tag* tmp = pile_tags;
        pile_tags = pile_tags->next;
        free(tmp->tag_name);
        free(tmp);
    }
}

// Fonction pour créer un nœud AST
ASTNode* create_ast_node(ASTNodeType type, const char* tag_or_text) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Erreur: impossible d'allouer de la mémoire pour un nœud AST\n");
        exit(1);
    }
    
    node->type = type;
    node->child_count = 0;
    
    if (type == NODE_ELEMENT) {
        strncpy(node->tag, tag_or_text, sizeof(node->tag) - 1);
        node->tag[sizeof(node->tag) - 1] = '\0';
    } else if (type == NODE_TEXT) {
        strncpy(node->text, tag_or_text, sizeof(node->text) - 1);
        node->text[sizeof(node->text) - 1] = '\0';
    } else if (type == NODE_PROP) {
        strncpy(node->prop_name, tag_or_text, sizeof(node->prop_name) - 1);
        node->prop_name[sizeof(node->prop_name) - 1] = '\0';
    }
    
    return node;
}

// Fonction pour ajouter un enfant à un nœud AST
void add_child_to_ast(ASTNode* parent, ASTNode* child) {
    if (parent->child_count < 10) {
        parent->children[parent->child_count++] = child;
    } else {
        fprintf(stderr, "Erreur: trop d'enfants pour un nœud AST\n");
        free(child);
    }
}

// Fonction pour ajouter un type personnalisé
void add_custom_type(char* name) {
    if (type_count >= 20) {
        fprintf(stderr, "Erreur: trop de types personnalisés\n");
        return;
    }
    strncpy(custom_types[type_count].name, name, sizeof(custom_types[type_count].name) - 1);
    custom_types[type_count].name[sizeof(custom_types[type_count].name) - 1] = '\0';
    custom_types[type_count].field_count = 0;
    type_count++;
}

// Fonction pour ajouter un champ à un type personnalisé
void add_field_to_type(char* field_name, char* field_type) {
    if (type_count <= 0) {
        fprintf(stderr, "Erreur: aucun type personnalisé défini\n");
        return;
    }
    
    int idx = type_count - 1;
    if (custom_types[idx].field_count >= 10) {
        fprintf(stderr, "Erreur: trop de champs pour le type %s\n", custom_types[idx].name);
        return;
    }
    
    strncpy(custom_types[idx].fields[custom_types[idx].field_count][0], field_name, 49);
    custom_types[idx].fields[custom_types[idx].field_count][0][49] = '\0';
    
    strncpy(custom_types[idx].fields[custom_types[idx].field_count][1], field_type, 49);
    custom_types[idx].fields[custom_types[idx].field_count][1][49] = '\0';
    
    custom_types[idx].field_count++;
}

// Fonction pour ajouter une variable
void add_variable(char* name, char* type, char* value, int is_array, int array_size) {
    if (var_count >= 100) {
        fprintf(stderr, "Erreur: trop de variables\n");
        return;
    }
    
    strncpy(variables[var_count].name, name, sizeof(variables[var_count].name) - 1);
    variables[var_count].name[sizeof(variables[var_count].name) - 1] = '\0';
    
    strncpy(variables[var_count].type, type, sizeof(variables[var_count].type) - 1);
    variables[var_count].type[sizeof(variables[var_count].type) - 1] = '\0';
    
    if (value) {
        strncpy(variables[var_count].value, value, sizeof(variables[var_count].value) - 1);
        variables[var_count].value[sizeof(variables[var_count].value) - 1] = '\0';
    } else {
        variables[var_count].value[0] = '\0';
    }
    
    variables[var_count].is_array = is_array;
    variables[var_count].array_size = array_size;
    var_count++;
}

// Fonction pour traiter l'importation d'un composant
void process_import(char* component, char* path) {
    FILE* imported_file = fopen(path, "r");
    if (!imported_file) {
        fprintf(stderr, "Erreur ouverture %s\n", path);
        exit(1);
    }

    // Sauvegarder le fichier d'entrée actuel
    FILE* old_yyin = yyin;
    yyin = imported_file;

    // Sauvegarder le nom du composant actuel
    strncpy(current_component_name, component, sizeof(current_component_name) - 1);
    current_component_name[sizeof(current_component_name) - 1] = '\0';
    
    // Analyser le fichier importé
    yyparse();

    // Stocker l'AST résultant
    if (parsed_ast) {
        strcpy(component_asts[component_ast_count].name, component);
        component_asts[component_ast_count].ast = parsed_ast;
        component_ast_count++;
        parsed_ast = NULL; // Réinitialiser pour la prochaine analyse
    }

    // Restaurer le fichier d'entrée
    fclose(imported_file);
    yyin = old_yyin;
}

// Fonction pour remplacer une expression "{{prop}}" par sa valeur
const char* resolve_placeholder(const char* text, ComponentParam* props, int prop_count) {
    static char resolved[500]; // Buffer statique pour stocker le résultat
    
    // Vérifier si c'est un placeholder {{...}}
    if (strncmp(text, "{{", 2) == 0 && text[strlen(text)-2] == '}' && text[strlen(text)-1] == '}') {
        char key[50];
        size_t key_len = strlen(text) - 4; // Longueur sans les accolades
        if (key_len >= sizeof(key)) key_len = sizeof(key) - 1;
        
        strncpy(key, text + 2, key_len);
        key[key_len] = '\0';

        // Chercher la propriété correspondante
        for (int i = 0; i < prop_count; i++) {
            if (strcmp(props[i].name, key) == 0) {
                return props[i].value;
            }
        }
        
        // Si la propriété n'est pas trouvée, chercher dans les variables globales
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, key) == 0) {
                return variables[i].value;
            }
        }
        
        // Si aucune correspondance n'est trouvée
        snprintf(resolved, sizeof(resolved), "[Prop %s non trouvée]", key);
        return resolved;
    }
    
    // Si ce n'est pas un placeholder, retourner tel quel
    return text;
}

// Fonction pour générer du code C à partir d'un AST
void generate_c_code_from_ast(ASTNode* node, ComponentParam* props, int prop_count) {
    if (!node) return;

    char buffer[500];
    
    switch (node->type) {
        case NODE_ELEMENT:
            snprintf(buffer, sizeof(buffer), "    printf(\"<%s>\\n\");\n", node->tag);
            append_to_buffer(buffer);
            
            for (int i = 0; i < node->child_count; i++) {
                generate_c_code_from_ast(node->children[i], props, prop_count);
            }
            
            snprintf(buffer, sizeof(buffer), "    printf(\"</%s>\\n\");\n", node->tag);
            append_to_buffer(buffer);
            break;

        case NODE_TEXT: {
            const char* resolved = resolve_placeholder(node->text, props, prop_count);
            snprintf(buffer, sizeof(buffer), "    printf(\"%s\\n\");\n", resolved);
            append_to_buffer(buffer);
            break;
        }

        case NODE_PROP:
            for (int i = 0; i < prop_count; i++) {
                if (strcmp(node->prop_name, props[i].name) == 0) {
                    snprintf(buffer, sizeof(buffer), "    printf(\"%s\\n\");\n", props[i].value);
                    append_to_buffer(buffer);
                    break;
                }
            }
            break;
    }
}

// Fonction pour générer l'appel d'un composant
void generate_component_call(char* name, ComponentParam* params, int param_count) {
    for (int i = 0; i < component_ast_count; i++) {
        if (strcmp(component_asts[i].name, name) == 0) {
            ASTNode* ast = component_asts[i].ast;
            generate_c_code_from_ast(ast, params, param_count);
            return;
        }
    }
    fprintf(stderr, "Erreur : Composant %s non trouvé\n", name);
}

// Fonction pour générer des structures et prototypes de fonction
void generate_structs_and_prototypes() {
    append_to_buffer("// Structure définitions\n");
    
    // Générer les définitions de structures pour les types personnalisés
    for (int i = 0; i < type_count; i++) {
        append_to_buffer("typedef struct {\n");
        for (int j = 0; j < custom_types[i].field_count; j++) {
            char field_type[50];
            strncpy(field_type, custom_types[i].fields[j][1], sizeof(field_type) - 1);
            field_type[sizeof(field_type) - 1] = '\0';
            
            if (strcmp(field_type, "string") == 0) {
                append_to_buffer("    char* ");
            } else {
                append_to_buffer("    ");
                append_to_buffer(field_type);
                append_to_buffer(" ");
            }
            
            append_to_buffer(custom_types[i].fields[j][0]);
            append_to_buffer(";\n");
        }
        append_to_buffer("} ");
        append_to_buffer(custom_types[i].name);
        append_to_buffer(";\n\n");
    }
    
    // Générer les prototypes de fonctions pour les composants
    for (int i = 0; i < component_ast_count; i++) {
        char buffer[200];
        snprintf(buffer, sizeof(buffer), "void render%s(void);\n", component_asts[i].name);
        append_to_buffer(buffer);
    }
    
    append_to_buffer("void renderMain(void);\n\n");
}

// Fonction pour générer une fonction renderMain
void generate_main() {
    append_to_buffer("void renderMain(void) {\n");
    
    // Déclarer les variables
    for (int i = 0; i < var_count; i++) {
        char buffer[1000];
        
        if (strcmp(variables[i].type, "string") == 0) {
            snprintf(buffer, sizeof(buffer), "    char* %s = %s;\n", 
                     variables[i].name, variables[i].value);
            append_to_buffer(buffer);
        } 
        else if (strcmp(variables[i].type, "int") == 0) {
            snprintf(buffer, sizeof(buffer), "    int %s = %s;\n", 
                     variables[i].name, variables[i].value);
            append_to_buffer(buffer);
        } 
        else {
            // Type personnalisé
            int type_found = 0;
            for (int j = 0; j < type_count; j++) {
                if (strcmp(variables[i].type, custom_types[j].name) == 0) {
                    type_found = 1;
                    
                    if (variables[i].is_array) {
                        snprintf(buffer, sizeof(buffer), "    %s %s[%d];\n", 
                                 custom_types[j].name, variables[i].name, variables[i].array_size);
                        append_to_buffer(buffer);
                        
                        // Initialisation des champs (à implémenter selon besoin)
                        append_to_buffer("    // Initialisation du tableau à implémenter\n");
                    } else {
                        snprintf(buffer, sizeof(buffer), "    %s %s;\n", 
                                 custom_types[j].name, variables[i].name);
                        append_to_buffer(buffer);
                        
                        // Initialisation des champs (à implémenter selon besoin)
                        append_to_buffer("    // Initialisation de la structure à implémenter\n");
                    }
                    break;
                }
            }
            
            if (!type_found) {
                fprintf(stderr, "Erreur: type %s non défini\n", variables[i].type);
            }
        }
    }
    
    // Générer le contenu HTML du composant principal
    append_to_buffer("\n    printf(\"<div class='container'>\\n\");\n");
    
    // Ici, nous pourrions appeler les composants ou générer du contenu HTML de manière dynamique
    // basé sur l'AST du composant principal, mais pour simplifier:
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].type, "string") == 0) {
            char buffer[500];
            snprintf(buffer, sizeof(buffer), "    printf(\"<h1>%%s</h1>\\n\", %s);\n", variables[i].name);
            append_to_buffer(buffer);
            break; // Juste pour l'exemple, on affiche une variable string
        }
    }
    
    // Si nous avons des tableaux, nous pouvons générer des boucles
    for (int i = 0; i < var_count; i++) {
        if (variables[i].is_array) {
            char buffer[1000];
            snprintf(buffer, sizeof(buffer), 
                     "\n    // Boucle sur le tableau %s\n"
                     "    for(int i = 0; i < %d; i++) {\n"
                     "        printf(\"<div class='item'>Item %%d</div>\\n\", i);\n"
                     "    }\n",
                     variables[i].name, variables[i].array_size);
            append_to_buffer(buffer);
            break; // Juste pour l'exemple, on traite un tableau
        }
    }
    
    append_to_buffer("\n    printf(\"</div>\\n\");\n");
    
    // Libération de la mémoire si nécessaire
    append_to_buffer("\n    // Libération de la mémoire\n");
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].type, "string") == 0) {
            // Les chaînes allouées dynamiquement doivent être libérées
            // Mais attention à ne pas libérer les litéraux de chaînes!
            if (variables[i].value[0] != '"') {
                char buffer[200];
                snprintf(buffer, sizeof(buffer), "    free(%s);\n", variables[i].name);
                append_to_buffer(buffer);
            }
        }
    }
    
    append_to_buffer("}\n\n");
}

// Fonction pour générer la fonction generer_html
void generate_html_function() {
    append_to_buffer("void generer_html() {\n");
    append_to_buffer("    FILE *file = fopen(\"index.html\", \"w\");\n");
    append_to_buffer("    if (file == NULL) {\n");
    append_to_buffer("        perror(\"Erreur lors de la création de index.html\");\n");
    append_to_buffer("        return;\n");
    append_to_buffer("    }\n");
    append_to_buffer("    \n");
    append_to_buffer("    fprintf(file, \n");
    append_to_buffer("        \"<!DOCTYPE html>\\n\"\n");
    append_to_buffer("        \"<html lang=\\\"fr\\\">\\n\"\n");
    append_to_buffer("        \"<head>\\n\"\n");
    append_to_buffer("        \"    <meta charset=\\\"UTF-8\\\">\\n\"\n");
    append_to_buffer("        \"    <meta name=\\\"viewport\\\" content=\\\"width=device-width, initial-scale=1.0\\\">\\n\"\n");
    append_to_buffer("        \"    <title>Application générée</title>\\n\"\n");
    append_to_buffer("        \"    <style>\\n\"\n");
    append_to_buffer("        \"        .container { max-width: 800px; margin: 0 auto; padding: 20px; }\\n\"\n");
    append_to_buffer("        \"        .card { border: 1px solid #ddd; padding: 15px; margin: 10px 0; border-radius: 5px; }\\n\"\n");
    append_to_buffer("        \"    </style>\\n\"\n");
    append_to_buffer("        \"</head>\\n\"\n");
    append_to_buffer("        \"<body>\\n\"\n");
    append_to_buffer("    );\n");
    append_to_buffer("    \n");
    append_to_buffer("    // Capture la sortie de renderMain dans un buffer\n");
    append_to_buffer("    char buffer[4096] = {0};\n");
    append_to_buffer("    FILE *old_stdout = stdout;\n");
    append_to_buffer("    FILE *new_stdout = fmemopen(buffer, sizeof(buffer), \"w\");\n");
    append_to_buffer("    if (!new_stdout) {\n");
    append_to_buffer("        perror(\"Erreur lors de la création du buffer de mémoire\");\n");
    append_to_buffer("        fclose(file);\n");
    append_to_buffer("        return;\n");
    append_to_buffer("    }\n");
    append_to_buffer("    stdout = new_stdout;\n");
    append_to_buffer("    renderMain();\n");
    append_to_buffer("    fflush(new_stdout);\n");
    append_to_buffer("    stdout = old_stdout;\n");
    append_to_buffer("    fclose(new_stdout);\n");
    append_to_buffer("    \n");
    append_to_buffer("    // Écrit le contenu de renderMain dans le fichier\n");
    append_to_buffer("    fprintf(file, \"%s\", buffer);\n");
    append_to_buffer("    \n");
    append_to_buffer("    fprintf(file,\n");
    append_to_buffer("        \"</body>\\n\"\n");
    append_to_buffer("        \"</html>\\n\"\n");
    append_to_buffer("    );\n");
    append_to_buffer("    \n");
    append_to_buffer("    fclose(file);\n");
    append_to_buffer("    printf(\"index.html généré avec succès !\\n\");\n");
    append_to_buffer("}\n\n");
}

// Fonction génération du main
void generate_main_function() {
    append_to_buffer("int main() {\n");
    append_to_buffer("    generer_html();\n");
    append_to_buffer("    return 0;\n");
    append_to_buffer("}\n");
}

// Fonction pour générer le code final
void generate_final_code() {
    generate_structs_and_prototypes();
    generate_main();
    generate_html_function();
    generate_main_function();
}
%}

%union {
    int intval;   // For numeric values
    char* strval; // For strings like IDENTIFIER
}


%token COMPONENT LBRACE RBRACE LPAREN RPAREN LT GT SLASH COMMA COLON
%token IMPORT FROM TYPE FOREACH IN SEMI STRING INTEGER LBRACKET RBRACKET
%token <strval> IDENTIFIER RETURN DOUBLE_QUOTE EQUALS
%type <strval> element parameters typed_param_list typed_param function html_content html_balise_open html_balise_close html_inner html_balise_autoferme attributes attribute
%type <strval> import_statement type_definition custom_field field_type variable_declaration variable_value forEach_statement
%start program

%%

program:
      imports type_definitions components
      {
        liberer_pile(); 
        analyze_dependencies();
        generate_includes();
        generate_final_code();
        printf("%s", output_buffer);
      }
    ;


element:
      COMPONENT IDENTIFIER LPAREN parameters RPAREN LBRACE function RBRACE
      { 
          /* $2 is the component name and $4 is the parameter list */
          char buffer[1000];
          sprintf(buffer, "void render%s(%s) {%s}\n", $2, $4, $7);
          append_to_buffer(buffer);
          free($4);
      }
    ;

parameters:
      /* empty */ { $$ = strdup(""); }
    | typed_param_list { $$ = $1; }
    ;

typed_param_list:
      typed_param 
      { $$ = $1; }
    | typed_param_list COMMA typed_param
      {
          /* Concatenate the previous list with ", " and the new parameter */
          char* tmp = malloc(strlen($1) + strlen($3) + 3); // extra space for comma, space, and '\0'
          sprintf(tmp, "%s, %s", $1, $3);
          free($1);
          $$ = tmp;
      }
    ;

typed_param:
      IDENTIFIER COLON IDENTIFIER
      {
        
        strcpy(identifiers[id_count], $1);
        strcpy(types[id_count], $3); // Sauvegarde du type
        id_count++;
        /* The DSL expects parameters as "var : type".
            In C, parameters are declared as "type var". 
            So $1 is the variable name and $3 is its type.
        */

        char* res = malloc(strlen($1) + strlen($3) + 2);
        sprintf(res, "%s %s", $3, $1);
        $$ = res;
      }
    ;

function:
    /* empty */ { $$ = strdup(""); }
    | LPAREN RETURN html_content RPAREN
    { 
        char* tmp = malloc(strlen($3) + 50); // Allouer mémoire pour printf
        sprintf(tmp, "\n\tprintf(\"%s\"", $3);
        free($3);

        // Ajouter les arguments à printf
        if (id_count > 0) {
            strcat(tmp, ", ");
            for (int i = 0; i < id_count; i++) {
                strcat(tmp, identifiers[i]);
                if (i < id_count - 1) strcat(tmp, ", ");
            }
        }
        strcat(tmp, ");\n");

        $$ = tmp;  // Retourner la chaîne générée
    }
    ;


imports:
      /* empty */
    | imports import_statement
    ;

import_statement:
      IMPORT IDENTIFIER FROM STRING
      {
        process_import($2, $4);
        free($2);
        free($4);
      }
    ;

type_definitions:
      /* empty */
    | type_definitions type_definition
    ;

type_definition:
      TYPE IDENTIFIER EQUALS LBRACE custom_fields RBRACE SEMI
      {
        add_custom_type($2);
        free($2);
      }
    ;

custom_fields:
      custom_field
    | custom_fields custom_field
    ;

custom_field:
      IDENTIFIER COLON field_type SEMI
      {
        add_field_to_type($1, $3);
        free($1);
        free($3);
      }
    ;

field_type:
      IDENTIFIER { $$ = $1; }
    | IDENTIFIER LBRACKET RBRACKET 
      { 
        char* tmp = malloc(strlen($1) + 3);
        sprintf(tmp, "%s[]", $1);
        free($1);
        $$ = tmp;
      }
    ;

variable_declarations:
      /* empty */
    | variable_declarations variable_declaration
    ;

variable_declaration:
      IDENTIFIER COLON field_type EQUALS variable_value SEMI
      {
        add_variable($1, $3, $5, 0);
        free($1);
        free($3);
        free($5);
      }
    | IDENTIFIER COLON field_type LBRACKET RBRACKET EQUALS LBRACKET object_array RBRACKET SEMI
      {
        add_variable($1, $3, "", 1);
        free($1);
        free($3);
      }
    ;

variable_value:
      STRING { $$ = $1; }
    | INTEGER { $$ = $1; }
    ;

object_array:
      object
    | object_array COMMA object
    ;

object:
      LBRACE object_fields RBRACE
    ;

object_fields:
      object_field
    | object_fields COMMA object_field
    ;

object_field:
      IDENTIFIER COLON variable_value
      {
        free($1);
        free($3);
      }
    ;

components:
      component
    | components component
    ;

component:
      COMPONENT IDENTIFIER LPAREN parameters RPAREN LBRACE component_body RBRACE
      {
        // Votre code existant pour traiter les composants
      }
    ;

component_body:
      variable_declarations function
    ;

forEach_statement:
      FOREACH LPAREN IDENTIFIER COLON IDENTIFIER IN IDENTIFIER RPAREN LBRACE html_content RBRACE
      {
        // Votre code pour traiter les boucles forEach
      }
    ;

html_content:
    html_balise_open html_content html_balise_close
    { 
        char* tmp = malloc(strlen($1) + strlen($2) + strlen($3) + 1);
        sprintf(tmp, "<%s> %s </%s>", $1, $2, $3);
        free($1);
        free($2);
        free($3);
        $$ = tmp;
    
    }
    |
    html_inner
    { 
        char* tmp = malloc( strlen($1) + 5);
        sprintf(tmp, "%s", $1);
        free($1);
        $$ = tmp;
        
    }
    |
    html_balise_autoferme html_content
    { 
        char* tmp = malloc(strlen($1) + strlen($2) + 1); // "<tag/>"
        sprintf(tmp, "<%s/> %s ", $1, $2);
        free($1);
        free($2);
        $$ = tmp;
    }
    ;

html_balise_autoferme:
    LT IDENTIFIER attributes SLASH GT
    {
        char* tmp = malloc(strlen($2) +strlen($3)+ 3); // "<tag/>"
        sprintf(tmp, "%s %s", $2, $3);
        free($2);
        free($3);
        $$ = tmp;
    }
    ;

html_balise_open:
    LT GT
    {
        char* tmp = malloc(1); // "<>"
        sprintf(tmp, "");
        empiler_tag("empty");
        $$ = tmp;
        
    }
    |
    LT IDENTIFIER attributes GT
    {
        char* tmp = malloc(strlen($2)+strlen($3)+ 3); // "<tag>"
        sprintf(tmp, "%s %s", $2,$3);
        
        // Empiler l'identifiant pour vérification ultérieure
        empiler_tag($2);
        free($2);
        free($3);
        $$ = tmp;
    }
    ;

   
attributes:
    attribute
    {
        $$ = $1;
    }
    | attributes attribute
    {
        char* tmp = malloc(strlen($1) + strlen($2) + 2);
        sprintf(tmp, "%s %s", $1, $2);
        free($1);
        free($2);
        $$ = tmp;
    }
    | /* empty */
    {
        $$ = strdup("");
    }
    ;

attribute:
    IDENTIFIER EQUALS DOUBLE_QUOTE IDENTIFIER DOUBLE_QUOTE
    {
        char* tmp = malloc(strlen($1) + strlen($4) + 10);
        sprintf(tmp, "%s='%s'", $1, $4);
        free($1);
        free($4);
        $$ = tmp;
    }
    | IDENTIFIER EQUALS DOUBLE_QUOTE DOUBLE_QUOTE
    {
        char* tmp = malloc(strlen($1) + 10);
        sprintf(tmp, "%s=''", $1);
        free($1);
        $$ = tmp;
    }
    | IDENTIFIER
    {
        // For boolean attributes like <input disabled>
        char* tmp = malloc(strlen($1) + 10);
        sprintf(tmp, "%s", $1);
        free($1);
        $$ = tmp;
    }
    ;
html_balise_close:
    LT SLASH GT
    {
        char* tmp = malloc(1); // "</>"
        sprintf(tmp, "");
        verifier_tag_fermant("empty");
        $$ = tmp;
    }
    |
    LT SLASH IDENTIFIER GT
    {
        char* tmp = malloc(1); // "</tag>"
        sprintf(tmp, "%s", $3);
        verifier_tag_fermant($3);
        free($3);
        $$ = tmp;
    }
    ;


html_inner:
      IDENTIFIER { $$ = $1; }
    | LBRACE IDENTIFIER RBRACE { $$ = $2; }
    | LBRACE forEach_statement RBRACE { $$ = $2; }
    | /* empty */ { $$ = strdup(""); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    exit(1);  // Or set a global error flag
} 


int main() {
    yydebug = 1;
    
    int result = yyparse();
    if (result == 0) {
        return 0;
    } else {
        printf("Parsing failed\n");
    }
    return 0;
}  
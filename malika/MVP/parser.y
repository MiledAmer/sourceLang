%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/import.h"
#include "lib/component.h"
#include "lib/customType.h"
#include "lib/variable.h"
#include "lib/identifier.h"
#include "lib/buffer.h"
#include "parser.tab.h"

extern int yylex();
void yyerror(const char *s);

#define YYDEBUG 1
void liberer_pile() {
    for (int i = 0; i < id_count; i++) {
        free(identifiers[i].name);
        free(identifiers[i].type);
    }
    id_count = 0;
}

void generate_html_output_code() {
   
    printf("// Définition des types personnalisés\n");
    

    generate_structs_and_prototypes();

    fflush(stdout);
    analyze_dependencies(&output_buffer);
    generate_includes();

    // Générer le code du buffer de sortie
    generate_output_buffer_code();

    printf("// Fonction pour générer un fichier HTML avec le contenu du buffer\n");
    printf("void generate_html(const char *filename) {\n");
    printf("    FILE *file = fopen(filename, \"w\");\n");
    printf("    if (file == NULL) {\n");
    printf("        fprintf(stderr, \"Error: Failed to open file %%s for writing\\n\", filename);\n");
    printf("        return;\n");
    printf("    }\n");
    printf("    fprintf(file, \"<!DOCTYPE html>\\n\");\n");
    printf("    fprintf(file, \"<html lang=\\\"en\\\">\\n\");\n");
    printf("    fprintf(file, \"<head>\\n\");\n");
    printf("    fprintf(file, \"    <meta charset=\\\"UTF-8\\\">\\n\");\n");
    printf("    fprintf(file, \"    <meta name=\\\"viewport\\\" content=\\\"width=device-width, initial-scale=1.0\\\">\\n\");\n");
    printf("    fprintf(file, \"    <title>Generated Component</title>\\n\");\n");
    printf("    fprintf(file, \"</head>\\n\");\n");
    printf("    fprintf(file, \"<body>\\n\");\n");
    printf("    fprintf(file, \"%%s\\n\", generate_output_buffer());\n");
    printf("    fprintf(file, \"</body>\\n\");\n");
    printf("    fprintf(file, \"</html>\\n\");\n");
    printf("    fclose(file);\n");
    printf("    printf(\"HTML file generated successfully: %%s\\n\", filename);\n");
    printf("}\n\n");

    printf("int main(int argc, char *argv[]) {\n");
    printf("// Déclaration des variables\n");
    declare_variables();
    printf("\n    const char *output_file = (argc > 1) ? argv[1] : \"output.html\";\n");
    printf("    generate_html(output_file);\n");
    printf("    return 0;\n");
    printf("}\n");
}

void generate_ts(const char *filename, const char* import_buffer, const char* interfaces_buffer) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open TS file %s for writing\n", filename);
        return;
    }

    fprintf(file, "%s\n", import_buffer);
    fprintf(file, "%s\n", interfaces_buffer);

    fclose(file);
    printf("✅ TypeScript file generated: %s\n", filename);
}


%}

%union {
    int intval;
    char* strval;
}
%token COMPONENT RETURN EQUALS 
%token LBRACE RBRACE LPAREN RPAREN COLON TYPE SEMICOLON FROM IMPORT COMMA
%token LT GT SLASH DOT LBRACKET RBRACKET
%token <strval> NUMBER_LITERAL BOOLEAN_LITERAL IDENTIFIER STRING_LITERAL
%type <strval> block import_instruction import_instructions
 
%%
program:
    block {
        //pour static
            liberer_pile();
            generate_html_output_code();
        
        //pour dynamique
        char final_output[20000];
        sprintf(final_output, "%s\n%s", interfaces_buffer.data, output_buffer.data);
        generate_ts("output.ts", import_buffer.data, interfaces_buffer.data);

    }
    |import_instructions block{
        //pour static
        liberer_pile();       
        generate_html_output_code();
        //pour dynamique

        imported_components_ts(&import_buffer);

        char final_output[20000];
        snprintf(final_output, sizeof(final_output), "%s\n%s\n%s", import_buffer.data, interfaces_buffer.data, output_buffer.data);
        generate_ts("output.ts", import_buffer.data, interfaces_buffer.data);
    }
;

block: 
    IDENTIFIER{
        $$=$1;
    }
    
;

import_instructions:
    import_instructions import_instruction {
        // Combine import instructions
        char *buffer = malloc(strlen($1) + strlen($2) + 2);
        sprintf(buffer, "%s%s", $1, $2);
        $$ = buffer;
        free($1); free($2);
    }
    | import_instruction { $$ = $1; }
;

import_instruction:
    IMPORT IDENTIFIER FROM STRING_LITERAL SEMICOLON {
        // Appeler process_import pour analyser le fichier importé
        add_imported_component($2,"null", $4);
        // Retourner le nom du composant importé
        $$ = $2;
        free($4); // Libérer la chaîne du chemin du fichier
    }
;



%%

void yyerror(const char *s) {
    extern char *yytext;  // yytext donne le token actuel
    fprintf(stderr, "Erreur de syntaxe : %s\n", s);
    fprintf(stderr, "Problème avec le token: '%s'\n", yytext);
    exit(1); // Vous pouvez choisir de simplement retourner et continuer à parser, ou sortir
}


int main() {
    liberer_pile();
    initialize_buffers();
    yydebug = 1;
    
    return yyparse();
    cleanup_buffers();
    
}
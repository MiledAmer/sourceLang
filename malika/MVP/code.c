// Définition des types personnalisés
/* Includes automatiques */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Définition du buffer de sortie
const char* generate_output_buffer() {
    static char buffer[] = 
        "";
    return buffer;
}

// Fonction pour générer un fichier HTML avec le contenu du buffer
void generate_html(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open file %s for writing\n", filename);
        return;
    }
    fprintf(file, "<!DOCTYPE html>\n");
    fprintf(file, "<html lang=\"en\">\n");
    fprintf(file, "<head>\n");
    fprintf(file, "    <meta charset=\"UTF-8\">\n");
    fprintf(file, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(file, "    <title>Generated Component</title>\n");
    fprintf(file, "</head>\n");
    fprintf(file, "<body>\n");
    fprintf(file, "%s\n", generate_output_buffer());
    fprintf(file, "</body>\n");
    fprintf(file, "</html>\n");
    fclose(file);
    printf("HTML file generated successfully: %s\n", filename);
}

int main(int argc, char *argv[]) {
// Déclaration des variables

    const char *output_file = (argc > 1) ? argv[1] : "output.html";
    generate_html(output_file);
    return 0;
}
✅ TypeScript file generated: output.ts

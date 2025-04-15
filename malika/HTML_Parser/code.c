#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Définition des types personnalisés
typedef struct {
    char* name;
    int age;
} User;

char output_buffer[10000] = {
    "<div id='main'>\n"
    "<div><h1>miled</h1><h1>miled</h1><h1>malika</h1><h1>12</h1><div id='cardComponent'>\n"
    "\n"
    "        <div>\n"
    "            <h2>World</h2>\n"
    "        </div>\n"
    "    \n"
    "</div><div id='userProfile'>\n"
    "\n"
    "        <p>Malika</p>\n"
    "    \n"
    "</div></div>\n"
    "</div>"
};

// Fonction pour générer un fichier HTML avec le contenu du buffer
void generate_html(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open file %s for writing\n", filename);
        return;
    }
    
    // Écrire l'en-tête HTML standard
    fprintf(file, "<!DOCTYPE html>\n");
    fprintf(file, "<html lang=\"en\">\n");
    fprintf(file, "<head>\n");
    fprintf(file, "    <meta charset=\"UTF-8\">\n");
    fprintf(file, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(file, "    <script src=\"https://cdn.tailwindcss.com\"></script>\n");
    fprintf(file, "    <title>Generated Component</title>\n");
    fprintf(file, "</head>\n");
    fprintf(file, "<body>\n");
    
    // Écrire le contenu du buffer
    fprintf(file, "%s\n", output_buffer);
    
    // Exemple de génération d'un bouton en C avec des classes Tailwind
    fprintf(file, "<button class=\"bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded\">\n");
    fprintf(file, "  Cliquez ici\n");
    fprintf(file, "</button>\n");
    // Exemple de génération d'un titre
    fprintf(file, "<h1 class=\"text-3xl font-bold underline\">Bienvenue</h1>\n");
    // Fermer le document HTML
    fprintf(file, "</body>\n");
    fprintf(file, "</html>\n");
    
    fclose(file);
    printf("HTML file generated successfully: %s\n", filename);
}

int main(int argc, char *argv[]) {
// Déclaration des variables

	User mohamed;
	mohamed.name = malloc(strlen("miled")+1);
	strcpy(mohamed.name, "miled");
	mohamed.age = 24;

	char* hello = "malika";
	int age = 12;
    const char *output_file = (argc > 1) ? argv[1] : "output.html";
    generate_html(output_file);
    return 0;
}

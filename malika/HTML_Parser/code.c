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
    "\n"
    "	<div>\n"
    "	<h1>malika</h1>\n"
    "	\n"
    "	<h1>malika</h1>\n"
    "	\n"
    "	<h3>miled</h3>\n"
    "	\n"
    "	<h1>12</h1>\n"
    "	<div id='cardComponent'>\n"
    "\n"
    "        <div>\n"
    "            <h1>miled</h1>\n"
    "            <p>22</p>\n"
    "            <h2>World</h2>\n"
    "        </div>\n"
    "    \n"
    "</div><div id='userProfile'>\n"
    "\n"
    "        <p>Malika</p>\n"
    "    \n"
    "</div></div>\n"
    "	\n"
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
    fprintf(file, "    <title>Generated Component</title>\n");
    fprintf(file, "</head>\n");
    fprintf(file, "<body>\n");
    
    // Écrire le contenu du buffer
    fprintf(file, "%s\n", output_buffer);
    
    // Fermer le document HTML
    fprintf(file, "</body>\n");
    fprintf(file, "</html>\n");
    
    fclose(file);
    printf("HTML file generated successfully: %s\n", filename);
}

int main(int argc, char *argv[]) {
// Déclaration des variables
	// Array of custom type User
	User users[2];
	users[0].name = malloc(strlen("miled")+1);
	strcpy(users[0].name, "miled");
	users[0].age = 24;
	users[1].name = malloc(strlen("malika")+1);
	strcpy(users[1].name, "malika");
	users[1].age = 22;

	User mohamed;
	mohamed.name = malloc(strlen("miled")+1);
	strcpy(mohamed.name, "miled");
	mohamed.age = 24;

	double array[] = {1, 2, 5, 19};
	char* hello = "malika";
	int year = 12;
	char* name = "";
	int age = 0;

    const char *output_file = (argc > 1) ? argv[1] : "output.html";
    generate_html(output_file);
    return 0;
}
//Résultat du parsing principal : 0

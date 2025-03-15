#include <stdio.h>

void generer_html() {
    FILE *file = fopen("index.html", "w");
    if (file == NULL) {
        perror("Erreur lors de la création de index.html");
        return;
    }

    fprintf(file,
        "<!DOCTYPE html>\n"
        "<html lang='fr'>\n"
        "<head>\n"
        "    <meta charset='UTF-8'>\n"
        "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
        "    <title>Compteur WebAssembly</title>\n"
        "    <script src='compteur_interface.js'></script>\n"
        "    <script src='compteur.js'></script>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>Compteur WebAssembly</h1>\n"
        "    <button onclick='incrementer_compteur()'>Cliquer ici</button>\n"
        "    <div id='compteur'>0</div>\n"
        "</body>\n"
        "</html>\n"
    );

    fclose(file);
    printf("index.html généré avec succès !\n");
}

int main() {
    generer_html();
    return 0;
}

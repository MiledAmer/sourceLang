/* Includes automatiques */
#include <ctype.h>
#include <math.h>
#include <stdio.h>

void renderUserProfile(const char* name) {
    printf("<div> <h1> %s </h1> </div>\n", name);
}

int main() {
    char name[50];
    printf("Entrez votre nom: ");
    fgets(name, sizeof(name),stdin);
    name[strcspn(name, "\n")] = 0;  // Supprime le saut de ligne

    renderUserProfile(name);

    printf("La racine carrée de 16 est : %.2f\n", sqrt(16));
    char c = 'A';

    // Vérifier si c est une lettre ou un chiffre
    if (isalnum(c) && c != '0') {
        printf("'%c' est une lettre ou un chiffre différent de 0.\n", c);
    } else {
        printf("'%c' n'est pas une lettre ou un chiffre valide.\n", c);
    }

    
    return 0;

}
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

    return 0;
}
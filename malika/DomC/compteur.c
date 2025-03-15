#include <stdio.h>
#include <emscripten.h>

// Variable globale pour le compteur
int compteur = 0;

// Fonction pour incrémenter le compteur, exposée à JavaScript
EMSCRIPTEN_KEEPALIVE
void incrementer_compteur() {
    compteur++;
    printf("Compteur: %d\n", compteur); // Affiche le compteur dans la console
}

// Fonction pour obtenir la valeur actuelle du compteur, exposée à JavaScript
EMSCRIPTEN_KEEPALIVE
int get_compteur() {
    return compteur;
}

// La fonction main n'est pas nécessaire pour ce cas d'utilisation WebAssembly
// mais peut être utilisée pour l'initialisation
int main() {
    printf("Module WebAssembly initialisé\n");
    return 0;
}
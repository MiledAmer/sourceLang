#include <emscripten.h>

int compteur = 0;  // Définition du compteur

// Fonction pour incrémenter le compteur
EMSCRIPTEN_KEEPALIVE
void incrementer_compteur() {
    compteur++;
}

// Fonction pour récupérer la valeur actuelle du compteur
EMSCRIPTEN_KEEPALIVE
int get_compteur() {
    return compteur;
}

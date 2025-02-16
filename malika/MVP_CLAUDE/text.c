//starting parseStarting parse
// Component UserProfile template
#include <stdio.h>
#include <string.h>

void renderUserProfile(char *buffer, const char *name, const char *email) {
    sprintf(buffer, "<div>\n");
    sprintf(buffer + strlen(buffer), "  <h1>%s</h1>\n", name);
    sprintf(buffer + strlen(buffer), "  <p>%s</p>\n", email);
    sprintf(buffer + strlen(buffer), "</div>");
}

int main() {
    char buffer[512]; // Assurez-vous que la taille est suffisante
    renderUserProfile(buffer, "Malika Laouiti", "malika@example.com");

    FILE *file = fopen("profile.html", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    fprintf(file, "%s", buffer);
    fclose(file);

    printf("Le profil a été enregistré dans 'profile.html'.\n");
    return 0;
}


// void profileHandler(HttpRequest *req, HttpResponse *res) {
//     char buffer[1024];
//     User user = db_find("users", "id", req->params["id"]);
//     renderUserProfile(buffer, user.name, user.email);
//     http_send(res, buffer);
// }

// void apiUsersHandler(HttpRequest *req, HttpResponse *res) {
//     User *users = db_find_all("users");
//     http_send_json(res, users);
// }

// Parsing completed successfully

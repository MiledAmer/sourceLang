#include "server.h"

int main() {
    initialize_winsock();
    struct Server server = server_Constructor(AF_INET, 80, SOCK_STREAM, 0, 10, INADDR_ANY, launch);
    server.launch(&server);
    return 0;
}
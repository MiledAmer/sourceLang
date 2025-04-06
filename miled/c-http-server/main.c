#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    #define close closesocket
    typedef SOCKET socket_t;
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    typedef int socket_t;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

#define PORT 8080
#define BUFFER_SIZE 30000

void init_platform() {
    #ifdef _WIN32
        WSADATA wsa_data;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            fprintf(stderr, "WSAStartup failed\n");
            exit(1);
        }
    #endif
}

void cleanup_platform() {
    #ifdef _WIN32
        WSACleanup();
    #endif
}

int main() {
    socket_t server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    init_platform();

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("socket failed");
        cleanup_platform();
        exit(EXIT_FAILURE);
    }

    // Configure address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        cleanup_platform();
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        cleanup_platform();
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while(1) {
        // Accept connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) == INVALID_SOCKET) {
            perror("accept");
            continue;
        }

        // Read request
        char buffer[BUFFER_SIZE] = {0};
        recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
        printf("Received request:\n%s\n", buffer);

        // Prepare response
        const char *response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n\r\n"
            "Hello, World!";

        // Send response
        send(new_socket, response, strlen(response), 0);
        printf("Response sent\n");

        // Close connection
        close(new_socket);
    }

    close(server_fd);
    cleanup_platform();
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <signal.h>
    #pragma comment(lib, "Ws2_32.lib")
    #define close closesocket
    typedef SOCKET socket_t;
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <signal.h>
    typedef int socket_t;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

#define PORT 8080
#define BUFFER_SIZE 30000
#define MAX_CONN 3

#ifdef _WIN32
    volatile int running = 1;
#else
    volatile sig_atomic_t running = 1;
#endif

void handle_signal(int sig) {
    running = 0;
}

void init_platform() {
    #ifdef _WIN32
        WSADATA wsa_data;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            fprintf(stderr, "WSAStartup failed: %d\n", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        SetConsoleCtrlHandler((PHANDLER_ROUTINE)handle_signal, TRUE);
    #else
        signal(SIGINT, handle_signal);
        signal(SIGTERM, handle_signal);
    #endif
}

void cleanup_platform() {
    #ifdef _WIN32
        WSACleanup();
    #endif
}

void log_error(const char* msg) {
    #ifdef _WIN32
        fprintf(stderr, "%s: %d\n", msg, WSAGetLastError());
    #else
        perror(msg);
    #endif
}

int main() {
    socket_t server_fd = INVALID_SOCKET, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    
    init_platform();

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        log_error("socket failed");
        cleanup_platform();
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt))) {
        log_error("setsockopt failed");
        close(server_fd);
        cleanup_platform();
        exit(EXIT_FAILURE);
    }

    // Configure address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        log_error("bind failed");
        close(server_fd);
        cleanup_platform();
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, MAX_CONN) < 0) {
        log_error("listen failed");
        close(server_fd);
        cleanup_platform();
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d (Ctrl+C to exit)...\n", PORT);

    while(running) {
        // Accept connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) == INVALID_SOCKET) {
            if (!running) break;  // Exit on signal
            log_error("accept");
            continue;
        }

        // Read request
        char buffer[BUFFER_SIZE] = {0};
        int bytes_read = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read < 0) {
            log_error("recv failed");
            close(new_socket);
            continue;
        }

        printf("Received request:\n%.*s\n", bytes_read, buffer);

        // Prepare response
        const char *response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n\r\n"
            "Hello, World!";

        // Send response
        if (send(new_socket, response, strlen(response), 0) < 0) {
            log_error("send failed");
        } else {
            printf("Response sent\n");
        }

        // Close connection
        close(new_socket);
    }

    // Cleanup
    if (server_fd != INVALID_SOCKET) close(server_fd);
    cleanup_platform();
    printf("\nServer shutdown gracefully\n");
    return 0;
}
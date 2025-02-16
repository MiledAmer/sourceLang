#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#define close closesocket


#include "server.h"

#ifdef _WIN32
void initialize_winsock() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

void cleanup_winsock() {
    WSACleanup();
}
#endif

struct Server server_Constructor(int domain, int port, int service, int protocol, int backlog, u_long interfac, void (*launch)(struct Server *server)) {
    struct Server server;

    server.domain = domain;
    server.service = service;
    server.port = port;
    server.protocol = protocol;
    server.backlog = backlog;
    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interfac);

    server.socket = socket(domain, service, protocol);
    if (server.socket == INVALID_SOCKET) {
        printf("Failed to initialize/connect to socket. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    if (bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address)) == SOCKET_ERROR) {
        printf("Failed to bind socket. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, server.backlog) == SOCKET_ERROR) {
        printf("Failed to start listening. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    server.launch = launch;
    return server;
}

void launch(struct Server *server) {
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("=== WAITING FOR CONNECTION === \n");
        int addrlen = sizeof(server->address);
        int new_socket = accept(server->socket, (struct sockaddr*)&server->address, (socklen_t*)&addrlen);
        if (new_socket == INVALID_SOCKET) {
            printf("Failed to accept connection. Error Code: %d\n", WSAGetLastError());
            continue;
        }

        ssize_t bytesRead = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead == SOCKET_ERROR) {
            printf("Error reading buffer. Error Code: %d\n", WSAGetLastError());
        } else {
            buffer[bytesRead] = '\0'; // Null terminate the string
            puts(buffer);
        }

        // char *response = "HTTP/1.1 200 OK\r\n"
        //                  "Content-Type: text/html; charset=UTF-8\r\n\r\n"
        //                  "<!DOCTYPE html>\r\n"
        //                  "<html>\r\n"
        //                  "<head>\r\n"
        //                  "<title>HTTP-SERVER</title>\r\n"
        //                  "</head>\r\n"
        //                  "<body>\r\n"
        //                  "Welcome to my server\r\n"
        //                  "</body>\r\n"
        //                  "</html>\r\n";
        // send(new_socket, response, strlen(response), 0);
        // close(new_socket);

         // Read the HTML file content
        FILE *html_file = fopen("index.html", "r");
        if (html_file == NULL) {    
            perror("Failed to open HTML file...\n");
            close(new_socket);
            continue;
        }

        // Load the HTML file content into a buffer
        char html_content[BUFFER_SIZE];
        size_t read_size = fread(html_content, 1, sizeof(html_content) - 1, html_file);
        html_content[read_size] = '\0'; // Null terminate the string
        fclose(html_file);

        // Construct the HTTP response
        char response[BUFFER_SIZE * 2];
        snprintf(response, sizeof(response),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                 "%s",
                 html_content);

        // Send the response
        send(new_socket, response, strlen(response), 0);
        close(new_socket);
    }
}

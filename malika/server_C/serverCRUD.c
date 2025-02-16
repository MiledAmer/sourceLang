#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#define close closesocket

#include "server.h"

#define MAX_RESOURCES 100

#ifdef _WIN32
void initialize_winsock()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

void cleanup_winsock()
{
    WSACleanup();
}
#endif

typedef struct
{
    int id;
    char name[100];
} Resource;

Resource resources[MAX_RESOURCES];
int resource_count = 0;

struct Server server_Constructor(int domain, int port, int service, int protocol, int backlog, u_long interfac, void (*launch)(struct Server *server))
{
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
    if (server.socket == INVALID_SOCKET)
    {
        printf("Failed to initialize/connect to socket. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    if (bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address)) == SOCKET_ERROR)
    {
        printf("Failed to bind socket. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, server.backlog) == SOCKET_ERROR)
    {
        printf("Failed to start listening. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    server.launch = launch;
    return server;
}

void send_response(int client_socket, const char *status, const char *content_type, const char *body)
{
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n\r\n"
             "%s",
             status, content_type, strlen(body), body);

    send(client_socket, response, strlen(response), 0);
}
void handle_post(int client_socket, const char *path, const char *body)
{
    if (strcmp(path, "/resources") == 0)
    {
        if (resource_count >= MAX_RESOURCES)
        {
            send_response(client_socket, "400 Bad Request", "text/plain", "Maximum resources reached");
            return;
        }

        // Debug print the raw body
        printf("Received POST body: %s\n", body);

        // Use sscanf to extract the id and name from the JSON body
        int id;
        char name[100];

        // The format should match the JSON structure
        // The space before %[^"] is important to skip over the unwanted characters
        if (sscanf(body, "{\"id\": %d, \"name\": \"%[^\"]\"}", &id, name) == 2)
        {
            // Debug print the parsed id and name
            printf("Parsed id: %d, name: %s\n", id, name);

            // Add the new resource to the array
            resources[resource_count].id = id;
            strcpy(resources[resource_count].name, name);
            resource_count++;

            send_response(client_socket, "201 Created", "text/plain", "Resource created successfully");
        }
        else
        {
            send_response(client_socket, "400 Bad Request", "text/plain", "Invalid JSON format");
        }
    }
    else
    {
        send_response(client_socket, "404 Not Found", "text./plain", "Endpoint not found");
    }
}
void handle_get(int client_socket, const char *path)
{
    char body[BUFFER_SIZE];
    if (strcmp(path, "/resources") == 0)
    {
        char resources_list[BUFFER_SIZE] = "[";
        for (int i = 0; i < resource_count; i++)
        {
            char resource[100];
            snprintf(resource, sizeof(resource), "{\"id\":%d,\"name\":\"%s\"}", resources[i].id, resources[i].name);
            strcat(resources_list, resource);
            if (i < resource_count - 1)
                strcat(resources_list, ",");
        }
        strcat(resources_list, "]");
        snprintf(body, sizeof(body), "{\"resources\":%s}", resources_list);
        send_response(client_socket, "200 OK", "application/json", body);
    }
    else
    {
        int id;
        if (sscanf(path, "/resources/%d", &id) == 1)
        {
            for (int i = 0; i < resource_count; i++)
            {
                if (resources[i].id == id)
                {
                    snprintf(body, sizeof(body), "{\"id\":%d,\"name\":\"%s\"}", resources[i].id, resources[i].name);
                    send_response(client_socket, "200 OK", "application/json", body);
                    return;
                }
            }
            send_response(client_socket, "404 Not Found", "text/plain", "Resource not found");
        }
        else
        {
            send_response(client_socket, "400 Bad Request", "text/plain", "Invalid resource ID");
        }
    }
}

void handle_put(int client_socket, const char *path, const char *body)
{
    int idpath, id;
    printf("Received PUT body: %s\n", body);
    
    // Extract the resource id from the path
    if (sscanf(path, "/resources/%d", &idpath) == 1)
    {
        char name[100];

        // Parse the JSON body to extract the name
        if (sscanf(body, "{\"id\": %d, \"name\": \"%[^\"]\"}", &id, name) == 2)
        {
            printf("Received PUT id: %d, name: %s\n", id, name);
            
            // Search for the resource with the given id
            for (int i = 0; i < resource_count; i++)
            {
                if (resources[i].id == idpath)
                {
                    // Update the resource with the new id and name
                    resources[i].id= id;
                    strcpy(resources[i].name, name);
                    send_response(client_socket, "200 OK", "text/plain", "Resource updated successfully");
                    return;
                }
            }

            // If resource with the id is not found
            send_response(client_socket, "404 Not Found", "text/plain", "Resource not found");
        }
        else
        {
            // If JSON body is invalid or missing name
            send_response(client_socket, "400 Bad Request", "text/plain", "Invalid JSON format");
        }
    }
    else
    {
        // If the path doesn't contain a valid resource ID
        send_response(client_socket, "400 Bad Request", "text/plain", "Invalid resource ID");
    }
}


void handle_delete(int client_socket, const char *path)
{
    int id;
    if (sscanf(path, "/resources/%d", &id) == 1)
    {
        for (int i = 0; i < resource_count; i++)
        {
            if (resources[i].id == id)
            {
                for (int j = i; j < resource_count - 1; j++)
                {
                    resources[j] = resources[j + 1];
                }
                resource_count--;
                send_response(client_socket, "200 OK", "text/plain", "Resource deleted successfully");
                return;
            }
        }
        send_response(client_socket, "404 Not Found", "text/plain", "Resource not found");
    }
    else
    {
        send_response(client_socket, "400 Bad Request", "text/plain", "Invalid resource ID");
    }
}

void handle_request(int client_socket, char *buffer)
{
    char method[10], path[100];
    sscanf(buffer, "%s %s", method, path);

    if (strcmp(method, "GET") == 0)
    {
        handle_get(client_socket, path);
    }
    else if (strcmp(method, "POST") == 0)
    {
        char *body = strstr(buffer, "\r\n\r\n");
        if (body)
        {
            body += 4; // Skip "\r\n\r\n"
            handle_post(client_socket, path, body);
        }
        else
        {
            send_response(client_socket, "400 Bad Request", "text/plain", "No body provided");
        }
    }
    else if (strcmp(method, "PUT") == 0)
    {
        char *body = strstr(buffer, "\r\n\r\n");
        if (body)
        {
            body += 4; // Skip "\r\n\r\n"
            handle_put(client_socket, path, body);
        }
        else
        {
            send_response(client_socket, "400 Bad Request", "text/plain", "No body provided");
        }
    }
    else if (strcmp(method, "DELETE") == 0)
    {
        handle_delete(client_socket, path);
    }
    else
    {
        send_response(client_socket, "405 Method Not Allowed", "text/plain", "Invalid HTTP method");
    }
}

void launch(struct Server *server)
{
    char buffer[BUFFER_SIZE];
    while (1)
    {
        printf("=== WAITING FOR CONNECTION ===\n");
        int addrlen = sizeof(server->address);
        int client_socket = accept(server->socket, (struct sockaddr *)&server->address, &addrlen);
        if (client_socket == INVALID_SOCKET)
        {
            printf("Failed to accept connection. Error Code: %d\n", WSAGetLastError());
            continue;
        }

        ssize_t bytesRead = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0'; // Null terminate
            handle_request(client_socket, buffer);
        }
        close(client_socket);
    }
}

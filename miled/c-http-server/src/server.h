// server.h
#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>
#include "platform.h"
#include "router.h"

// Configuration constants
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 30000
#endif


// Platform-agnostic types
typedef struct
{
    int port;
    size_t buffer_size; // Now matches BUFFER_SIZE by default
    int max_connections;
} ServerConfig;

typedef struct
{
    socket_fd_t socket_fd;
    struct sockaddr_in address;
} ServerInstance;

// Core functions
ServerInstance server_init(const ServerConfig *config);
void server_run(ServerInstance *instance);
void server_cleanup(ServerInstance *instance);

// Request/Response handling
void handle_client(socket_fd_t client_socket);
const char *build_response(const char *body, const char *content_type, int status_code);
void parse_request(const char *buffer, HttpRequest *req);

#endif
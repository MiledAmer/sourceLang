// server.h
#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>
#include "platform.h"

// Configuration constants
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 30000
#endif

#define MAX_ROUTE_PARAMS 10
#define MAX_PARAM_KEY 64
#define MAX_PARAM_VALUE 128

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

typedef enum
{
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_PATCH,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_UNSUPPORTED
} HttpMethod;

typedef struct
{
    HttpMethod method;
    char path[256];
    char protocol[16];
    size_t params_count;
    char params_keys[MAX_ROUTE_PARAMS][MAX_PARAM_KEY];
    char params_values[MAX_ROUTE_PARAMS][MAX_PARAM_VALUE];
} HttpRequest;

typedef struct
{
    const char *body;
    const char *content_type;
    int status_code;
} HttpResponse;

// Core functions
ServerInstance server_init(const ServerConfig *config);
void server_run(ServerInstance *instance);
void server_cleanup(ServerInstance *instance);

// Request/Response handling
void handle_client(socket_fd_t client_socket);
const char *build_response(const char *body, const char *content_type, int status_code);
void parse_request(const char *buffer, HttpRequest *req);

#endif
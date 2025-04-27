// server.c
#include "server.h"
#include "platform.h"
#include "router.h"
#include <string.h>

ServerInstance server_init(const ServerConfig *config)
{
    ServerInstance instance = {INVALID_SOCKET_FD};
    int opt = 1;

    // Create socket
    if ((instance.socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET_FD)
    {
        log_error("socket creation failed");
        return instance;
    }

    // Set socket options
    if (setsockopt(instance.socket_fd, SOL_SOCKET, SO_REUSEADDR,
                   (char *)&opt, sizeof(opt)))
    {
        log_error("setsockopt failed");
        close_socket(instance.socket_fd);
        instance.socket_fd = INVALID_SOCKET_FD;
        return instance;
    }

    // Configure address
    instance.address.sin_family = AF_INET;
    instance.address.sin_addr.s_addr = INADDR_ANY;
    instance.address.sin_port = htons(config->port);

    // Bind and listen
    if (bind(instance.socket_fd, (struct sockaddr *)&instance.address,
             sizeof(instance.address)) < 0)
    {
        log_error("bind failed");
        close_socket(instance.socket_fd);
        instance.socket_fd = INVALID_SOCKET_FD;
    }

    if (listen(instance.socket_fd, config->max_connections) < 0)
    {
        log_error("listen failed");
        close_socket(instance.socket_fd);
        instance.socket_fd = INVALID_SOCKET_FD;
    }

    return instance;
}

void server_run(ServerInstance *instance)
{
    printf("Server listening on port %d...\n", ntohs(instance->address.sin_port));

    while (is_running())
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_socket = accept(instance->socket_fd,
                                   (struct sockaddr *)&client_addr,
                                   &client_len);

        if (client_socket == INVALID_SOCKET_FD)
        {
            if (!is_running())
                break;
            log_error("accept failed");
            continue;
        }

        handle_client(client_socket);
        close_socket(client_socket);
    }
}

const char *build_response(const char *body, const char *content_type, int status_code)
{
    static char response[2048];
    const char *status_msg = "OK";

    switch (status_code)
    {
    case 404:
        status_msg = "Not Found";
        break;
    case 400:
        status_msg = "Bad Request";
        break;
    }

    snprintf(response, sizeof(response),
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n\r\n"
             "%s",
             status_code, status_msg,
             content_type, strlen(body), body);

    return response;
}

void handle_client(socket_fd_t client_socket)
{
    char buffer[BUFFER_SIZE] = {0};
    int bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

    if (bytes_read > 0)
    {
        HttpRequest req;
        parse_request(buffer, &req);

        printf("Received %s %s\n",
               req.method == HTTP_GET ? "GET" : req.method == HTTP_POST ? "POST"
                                                                        : "UNKNOWN",
               req.path);

        router_dispatch(client_socket, &req);
    }
}

void server_cleanup(ServerInstance *instance)
{
    if (instance->socket_fd != INVALID_SOCKET_FD)
    {
        close_socket(instance->socket_fd);
    }
}

void parse_request(const char *buffer, HttpRequest *req)
{
    // Initialize defaults
    req->method = HTTP_UNSUPPORTED;
    memset(req->path, 0, sizeof(req->path));
    memset(req->protocol, 0, sizeof(req->protocol));
    req->params_count = 0;
    memset(req->params_keys, 0, sizeof(req->params_keys));
    memset(req->params_values, 0, sizeof(req->params_values));
    req->body = NULL; // No body initially

    // Read first line (method, path, protocol)
    char method[16], path[256], protocol[16];
    if (sscanf(buffer, "%15s %255s %15s", method, path, protocol) != 3)
    {
        return;
    }

    if (strcmp(method, "GET") == 0)
    {
        req->method = HTTP_GET;
    }
    else if (strcmp(method, "POST") == 0)
    {
        req->method = HTTP_POST;
    }
    else if (strcmp(method, "PUT") == 0)
    {
        req->method = HTTP_PUT;
    }
    else if (strcmp(method, "PATCH") == 0)
    {
        req->method = HTTP_PATCH;
    }
    else if (strcmp(method, "DELETE") == 0)
    {
        req->method = HTTP_DELETE;
    }
    else if (strcmp(method, "HEAD") == 0)
    {
        req->method = HTTP_HEAD;
    }
    else if (strcmp(method, "OPTIONS") == 0)
    {
        req->method = HTTP_OPTIONS;
    }
    else
    {
        req->method = HTTP_UNSUPPORTED;
    }

    strncpy(req->path, path, sizeof(req->path) - 1);
    strncpy(req->protocol, protocol, sizeof(req->protocol) - 1);

    // Extract query parameters (if any)
    char *query_string = strchr(req->path, '?');
    if (query_string)
    {
        *query_string = '\0';                  
        query_string++;                        
        parse_query_params(query_string, req); 
        printf("Query string: %s\n", query_string);
    }

    // Now, extract headers and body
    const char *headers_start = strstr(buffer, "\r\n");
    if (headers_start)
    {
        headers_start += 2; // Skip the initial \r\n after the request line

        // Find the body start after the headers (double CRLF)
        const char *body_start = strstr(headers_start, "\r\n\r\n");
        if (body_start)
        {
            body_start += 4; // Skip the \r\n\r\n separator

            // Extract the body (for POST/PUT, etc.)
            req->body = strdup(body_start); // Allocate memory for body
        }
    }
}

void parse_query_params(const char *query_string, HttpRequest *req)
{
    char *query_copy = strdup(query_string);
    char *param = strtok(query_copy, "&");

    while (param && req->params_count < MAX_ROUTE_PARAMS)
    {
        char *key = strtok(param, "=");
        char *value = strtok(NULL, "=");

        if (key && value)
        {
            // Ensure no buffer overflow for keys/values
            strncpy(req->params_keys[req->params_count], key, MAX_PARAM_KEY - 1);
            strncpy(req->params_values[req->params_count], value, MAX_PARAM_VALUE - 1);
            req->params_count++;
        }
        param = strtok(NULL, "&");
    }

    free(query_copy);
}

// server.c
#include "server.h"
#include "platform.h"

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

        const char *response;

        // Route requests
        if (req.method == HTTP_GET)
        {
            if (strcmp(req.path, "/") == 0)
            {
                response = build_response(
                    "Hello, World!",
                    "text/plain",
                    200);
            }
            else if (strcmp(req.path, "/users") == 0)
            {
                response = build_response(
                    "User List: Alice, Bob",
                    "application/json",
                    200);
            }
            else
            {
                response = build_response(
                    "404 Not Found",
                    "text/plain",
                    404);
            }
        }
        else if (req.method == HTTP_POST)
        {
            if (strcmp(req.path, "/users") == 0)
            {
                response = build_response(
                    "{\"status\": \"User created\"}",
                    "application/json",
                    201);
            }
            else
            {
                response = build_response(
                    "404 Not Found",
                    "text/plain",
                    404);
            }
        }
        else
        {
            response = build_response(
                "400 Bad Request",
                "text/plain",
                400);
        }

        send(client_socket, response, strlen(response), 0);
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

    // Read first line
    char method[16];
    if (sscanf(buffer, "%15s %255s %15s", method, req->path, req->protocol) != 3)
    {
        return;
    }

    // Convert method to enum
    if (strcmp(method, "GET") == 0)
    {
        req->method = HTTP_GET;
    }
    else if (strcmp(method, "POST") == 0)
    {
        req->method = HTTP_POST;
    }
}
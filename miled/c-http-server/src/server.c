#include "server.h"
#include "platform.h"

ServerInstance server_init(const ServerConfig* config) {
    ServerInstance instance = { INVALID_SOCKET_FD };
    int opt = 1;

    // Create socket
    if ((instance.socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET_FD) {
        log_error("socket creation failed");
        return instance;
    }

    // Set socket options
    if (setsockopt(instance.socket_fd, SOL_SOCKET, SO_REUSEADDR, 
                  (char*)&opt, sizeof(opt))) {
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
    if (bind(instance.socket_fd, (struct sockaddr*)&instance.address,
            sizeof(instance.address)) < 0) {
        log_error("bind failed");
        close_socket(instance.socket_fd);
        instance.socket_fd = INVALID_SOCKET_FD;
    }

    if (listen(instance.socket_fd, config->max_connections) < 0) {
        log_error("listen failed");
        close_socket(instance.socket_fd);
        instance.socket_fd = INVALID_SOCKET_FD;
    }

    return instance;
}

void server_run(ServerInstance* instance) {
    printf("Server listening on port %d...\n", ntohs(instance->address.sin_port));
    
    while(is_running()) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_socket = accept(instance->socket_fd,
                                  (struct sockaddr*)&client_addr,
                                  &client_len);
        
        if (client_socket == INVALID_SOCKET_FD) {
            if (!is_running()) break;
            log_error("accept failed");
            continue;
        }

        handle_client(client_socket);
        close_socket(client_socket);
    }
}

const char* build_response(const char* body, const char* content_type) {
    static char response[1024];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n\r\n"
        "%s",
        content_type, strlen(body), body);
    return response;
}

void handle_client(socket_fd_t client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_read > 0) {
        printf("Received:\n%.*s\n", bytes_read, buffer);
        const char* response = build_response("Hello, World!", "text/plain");
        send(client_socket, response, strlen(response), 0);
    }
}

void server_cleanup(ServerInstance* instance) {
    if (instance->socket_fd != INVALID_SOCKET_FD) {
        close_socket(instance->socket_fd);
    }
}
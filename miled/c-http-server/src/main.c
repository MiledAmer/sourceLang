#include "server.h"
#include "platform.h"

int main()
{
    platform_init();
    register_signal_handlers();

    ServerConfig config = {
        .port = 8080,
        .buffer_size = BUFFER_SIZE,
        .max_connections = 3};

    ServerInstance server = server_init(&config);
    if (server.socket_fd == INVALID_SOCKET_FD)
    {
        log_error("Server initialization failed");
        return 1;
    }

    server_run(&server);
    server_cleanup(&server);
    platform_cleanup();

    printf("Server shutdown gracefully\n");
    return 0;
}
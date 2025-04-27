#include "server.h"
#include "router.h"
#include "platform.h"

HttpResponse handle_hello_route(const HttpRequest *req)
{
    return (HttpResponse){"Hello, World!", "text/plain", 200};
}

HttpResponse handle_bonjour_route(const HttpRequest *req)
{
    return (HttpResponse){"bonjour", "text/plain", 200};
}

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
    router_init();

    router_add_route(HTTP_POST, "/bonjour", handle_bonjour_route);
    router_add_route(HTTP_GET, "/hello", handle_hello_route);

    server_run(&server);
    server_cleanup(&server);
    platform_cleanup();

    printf("Server shutdown gracefully\n");
    return 0;
}
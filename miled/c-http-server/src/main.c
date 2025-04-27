#include "server.h"
#include "router.h"
#include "platform.h"
#include "./../dependencies/cJSON/cJSON.h"

HttpResponse handle_bonjour_route(const HttpRequest *req)
{
    if (req->body != NULL)
    {
        cJSON *json = cJSON_Parse(req->body);
        char *printed_json = cJSON_Print(json);
        if (printed_json != NULL)
        {
            printf("JSON: \n%s\n", printed_json);
            free(printed_json); 
        }
        
        cJSON_Delete(json);
    }
    else
    {
        printf("No body received.\n");
    }
    return (HttpResponse){req->body, "application/json", 200};
}

HttpResponse handle_hello_route(const HttpRequest *req)
{
    return (HttpResponse){"Hello, World!", "text/plain", 200};
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
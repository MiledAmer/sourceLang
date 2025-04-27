#ifndef ROUTER_H
#define ROUTER_H

#include "platform.h"
#include "../dependencies/rax/rax.h"

#define MAX_ROUTE_PARAMS 10
#define MAX_PARAM_KEY 64
#define MAX_PARAM_VALUE 128

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

typedef HttpResponse (*RouteHandler)(const HttpRequest *req);

typedef struct
{
    const char *pattern;
    RouteHandler handler;
    HttpMethod method;
} Route;

void router_init();
void router_add_route(HttpMethod method, const char *pattern, RouteHandler handler);
bool router_dispatch(socket_fd_t client_socket, HttpRequest *req);
void router_cleanup();
static void build_route_key(char *dst, size_t size, HttpMethod method, const char *path);

#endif

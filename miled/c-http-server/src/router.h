#ifndef ROUTER_H
#define ROUTER_H

#include "platform.h"
#include "server.h"
#include "../dependencies/rax/rax.h"

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

#include "router.h"
#include "server.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static rax *router_tree = NULL;

void router_init()
{
    if (!router_tree)
        router_tree = raxNew();
}

void router_cleanup()
{
    if (router_tree)
    {
        raxFree(router_tree);
        router_tree = NULL;
    }
}

void router_add_route(HttpMethod method, const char *pattern, RouteHandler handler)
{
    if (!router_tree)
        router_init();

    char key[512];
    build_route_key(key, sizeof(key), method, pattern);

    Route *entry = malloc(sizeof(Route));
    entry->method = method;
    entry->pattern = strdup(pattern);
    entry->handler = handler;

    raxInsert(router_tree, (unsigned char *)key, strlen(key), entry, NULL);
}

bool router_dispatch(socket_fd_t client_socket, HttpRequest *req)
{
    if (!router_tree)
        return false;

    char key[512];
    build_route_key(key, sizeof(key), req->method, req->path);
    printf("Dispatching route: %s\n", key);

    Route *entry = raxFind(router_tree, (unsigned char *)key, strlen(key));

    if (entry == raxNotFound)
    {
        const char *response;
        response = build_response(
            "400 Bad Request",
            "text/plain",
            400);

        send(client_socket, response, strlen(response), 0);
        return false;
    }

    if (entry != raxNotFound)
    {

        HttpResponse res = entry->handler(req);
        const char *response = build_response(res.body, res.content_type, res.status_code);
        send(client_socket, response, strlen(response), 0);
        return true;
    }

    raxIterator iter;
    raxStart(&iter, router_tree);
    raxSeek(&iter, "^", NULL, 0);

    while (raxNext(&iter))
    {
        Route *e = iter.data;

        if (e->method != req->method)
            continue;

        const char *pattern = e->pattern;
        char pattern_copy[256], path_copy[256];
        strncpy(pattern_copy, pattern, sizeof(pattern_copy));
        strncpy(path_copy, req->path, sizeof(path_copy));

        char *pat_tok = strtok(pattern_copy, "/");
        char *path_tok = strtok(path_copy, "/");

        req->params_count = 0;
        bool matched = true;

        while (pat_tok && path_tok)
        {
            if (pat_tok[0] == ':')
            {
                if (req->params_count < MAX_ROUTE_PARAMS)
                {
                    strncpy(req->params_keys[req->params_count], pat_tok + 1, MAX_PARAM_KEY - 1);
                    req->params_keys[req->params_count][MAX_PARAM_KEY - 1] = '\0';

                    strncpy(req->params_values[req->params_count], path_tok, MAX_PARAM_VALUE - 1);
                    req->params_values[req->params_count][MAX_PARAM_VALUE - 1] = '\0';
                    req->params_count++;
                }
            }
            else if (strcmp(pat_tok, path_tok) != 0)
            {
                matched = false;
                break;
            }

            pat_tok = strtok(NULL, "/");
            path_tok = strtok(NULL, "/");
        }

        if (matched && !pat_tok && !path_tok)
        {
            HttpResponse res = e->handler(req);
            const char *response = build_response(res.body, res.content_type, res.status_code);
            send(client_socket, response, strlen(response), 0);
            raxStop(&iter);
            return true;
        }
    }

    raxStop(&iter);
    return false;
}

static void build_route_key(char *dst, size_t size, HttpMethod method, const char *path)
{
    snprintf(dst, size, "%d:%s", method, path);
}
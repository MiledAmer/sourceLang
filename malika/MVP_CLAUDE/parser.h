#ifndef PARSER_H
#define PARSER_H

typedef struct {
    char *method;
    char *path;
    char *params[10];
    int param_count;
} HttpRequest;

typedef struct {
    void (*send)(const char *data);
    void (*json)(const void *data);
} HttpResponse;

typedef struct {
    char *name;
    char *email;
    char *id;
} User;

User db_find(const char *collection, const char *field, const char *value);
User* db_find_all(const char *collection);
void http_send(HttpResponse *res, const char *data);
void http_send_json(HttpResponse *res, const void *data);

#endif
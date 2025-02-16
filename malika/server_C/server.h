#ifndef server_h
#define server_h

//des bibliotheques pour windows
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Lier automatiquement la bibliothèque réseau


#define BUFFER_SIZE 16000

struct Server {
    int domain;
    int port;
    int service;
    int protocol;
    int backlog;
    u_long interfac;

    int socket;
    struct sockaddr_in address;

    void (*launch)(struct Server *server);
};

struct Server server_Constructor(int domain, int port, int service, int protocol, int backlog, u_long interfac, void (*launch)(struct Server *server));
void launch(struct Server *server);

//WIN32
void initialize_winsock();
void cleanup_winsock();


#endif

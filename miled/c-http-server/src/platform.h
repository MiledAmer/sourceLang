#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>
#include <stdio.h>
// Platform detection
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    
    // Windows types
    typedef SOCKET socket_fd_t;
    #define INVALID_SOCKET_FD INVALID_SOCKET
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <signal.h>
    #include <unistd.h>
    
    // UNIX types
    typedef int socket_fd_t;
    #define INVALID_SOCKET_FD -1
#endif

// Platform API
void platform_init();
void platform_cleanup();
bool is_running();
void register_signal_handlers();  // Add missing declaration
void log_error(const char* message);
void close_socket(socket_fd_t sockfd);  // Fix parameter type

#endif
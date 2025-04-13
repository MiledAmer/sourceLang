#include "platform.h"
#include <stdio.h>

// Shared state
#ifdef _WIN32
    static volatile bool running = true;
    static BOOL WINAPI ctrl_handler(DWORD dwCtrlType) {
        running = false;
        return TRUE;
    }
#else
    static volatile sig_atomic_t running = true;
    static void handle_signal(int sig) {
        running = false;
    }
#endif

void platform_init() {
    #ifdef _WIN32
        WSADATA wsa_data;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
            fprintf(stderr, "WSAStartup failed: %d\n", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        SetConsoleCtrlHandler(ctrl_handler, TRUE);
    #else
        // Signal handlers registered in register_signal_handlers()
    #endif
}

void platform_cleanup() {
    #ifdef _WIN32
        WSACleanup();
    #endif
}

bool is_running() {
    return running;
}

void register_signal_handlers() {
    #ifndef _WIN32
        signal(SIGINT, handle_signal);
        signal(SIGTERM, handle_signal);
    #endif
}

void log_error(const char* message) {
    #ifdef _WIN32
        fprintf(stderr, "%s: %d\n", message, WSAGetLastError());
    #else
        perror(message);
    #endif
}

// Fixed parameter type to match header
void close_socket(socket_fd_t sockfd) {
    #ifdef _WIN32
        closesocket(sockfd);
    #else
        close(sockfd);
    #endif
}
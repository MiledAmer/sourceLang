#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link with the WinSock library

int main()
{
    WSADATA wsaData;
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[1024];
    char requete[1024];  // Buffer for the HTTP request
    char json_body[256]; // Buffer for JSON payload
    int idPut, idUp;     // Variables to store resource ID
    char id[10], name[50]; // Properly sized character arrays

    // Initialize WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return 1;
    }

    portno = 80; // HTTP port

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        printf("ERROR opening socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Configure server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.142.1"); // Use direct IP

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("ERROR connecting: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
        int choice;

        // Display menu
        printf("Choose an option:\n");
        printf("1. GET All Resources\n");
        printf("2. GET Resource by ID\n");
        printf("3. POST (Add New Resource)\n");
        printf("4. PUT (Update Resource)\n");
        printf("5. DELETE Resource by ID\n");
        printf("Enter your choice (1-5): ");

        // Get user input
        scanf("%d", &choice);
        getchar(); // Consume leftover newline

        // Handle choices
        switch (choice)
        {
        case 1:
            // Send GET request for all resources
            snprintf(requete, sizeof(requete), "GET /resources HTTP/1.1\r\nHost: 192.168.142.1\r\nConnection: close\r\n\r\n");
            break;

        case 2:
            printf("Enter the ID of the resource: ");
            scanf("%9s", id); // Limit input length to avoid buffer overflow
            snprintf(requete, sizeof(requete), "GET /resources/%s HTTP/1.1\r\nHost: 192.168.142.1\r\nConnection: close\r\n\r\n", id);
            break;

        case 3: // POST request to add a new resource
            printf("Enter the resource ID: ");
            scanf("%d", &idPut); // Corrected to use &idPut
            printf("Enter the resource Name: ");
            scanf(" %[^\n]", name); // Read full name with spaces

            // Format JSON body: {"id":2,"name":"First Resource Name"}
            snprintf(json_body, sizeof(json_body), "{\"id\":%d,\"name\":\"%s\"}", idPut, name);

            // Format HTTP POST request
            snprintf(requete, sizeof(requete),
                     "POST /resources HTTP/1.1\r\n"
                     "Host: 192.168.142.1\r\n"
                     "Content-Type: application/json\r\n"
                     "Content-Length: %d\r\n"
                     "Connection: close\r\n\r\n"
                     "%s",
                     (int)strlen(json_body), json_body);
            break;

        case 4: // PUT request to update an existing resource
            printf("Enter the resource ID to update: ");
            scanf("%d", &idUp); // Corrected to use &idPut
            printf("Enter the new resource ID : ");
            scanf("%d", &idPut); // Corrected to use &idPut
            printf("Enter the new resource Name: ");
            scanf(" %[^\n]", name); // Read full name with spaces

            // Format JSON body for PUT request: {"id":2,"name":"Updated Resource"}
            snprintf(json_body, sizeof(json_body), "{\"id\":%d,\"name\":\"%s\"}", idPut, name);

            // Format HTTP PUT request
            snprintf(requete, sizeof(requete),
                     "PUT /resources/%d HTTP/1.1\r\n"
                     "Host: 192.168.142.1\r\n"
                     "Content-Type: application/json\r\n"
                     "Content-Length: %d\r\n"
                     "Connection: close\r\n\r\n"
                     "%s",
                     idUp, (int)strlen(json_body), json_body);
            break;

        case 5:
            printf("Enter the ID of the resource: ");
            scanf("%9s", id); // Limit input length to avoid buffer overflow
            snprintf(requete, sizeof(requete), "DELETE /resources/%s HTTP/1.1\r\nHost: 192.168.142.1\r\nConnection: close\r\n\r\n", id);
            break;

        default:
            printf("Invalid choice! Please enter a number between 1 and 5.\n");
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
    

    // Send the request
    n = send(sockfd, requete, strlen(requete), 0);
    if (n < 0)
    {
        printf("ERROR writing to socket: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // Receive HTTP response
    memset(buffer, 0, sizeof(buffer));
    while ((n = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[n] = '\0'; // Ensure null-terminated string
        printf("%s", buffer);
    }

    if (n < 0)
    {
        printf("ERROR reading from socket: %d\n", WSAGetLastError());
    }

    // Close socket and clean up
    closesocket(sockfd);
    WSACleanup();

    return 0;
}

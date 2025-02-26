#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  // For close()

#define PORT 3388

int main() {
    int sockfd, retval, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t actuallen;
    char client_ip[INET_ADDRSTRLEN];
    int client_port;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Configure server address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;  // FIXED: Assigned directly

    // Bind socket
    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        perror("Bind failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Listen for connections
    retval = listen(sockfd, 1);
    if (retval == -1) {
        perror("Listen failed");
        close(sockfd);
        return EXIT_FAILURE;
    }
    printf("Server listening on port %d...\n", PORT);

    // Accept a client connection
    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen); 
    if (newsockfd == -1) {
        perror("Accept failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Convert client IP and port
    if (inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip, sizeof(client_ip)) == NULL) {
        perror("inet_ntop failed");
        close(newsockfd);
        close(sockfd);
        return EXIT_FAILURE;
    }
    client_port = ntohs(clientaddr.sin_port);

    printf("Client connected: IP = %s, Port = %d\n", client_ip, client_port);

    // Close sockets
    close(newsockfd);
    close(sockfd);

    return EXIT_SUCCESS;
}

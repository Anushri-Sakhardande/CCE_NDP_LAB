#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3388
#define BUFFER_SIZE 1024

int main() {
    int sockfd, retval;
    struct sockaddr_in serveraddr;
    char sendBuffer[BUFFER_SIZE], recvBuffer[BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Configure server address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);

    // Convert and assign IP address (Recommended over inet_addr)
    if (inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr) <= 0) {
        perror("Invalid address / Address conversion failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Connect to the server
    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        perror("Connection failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Connected to the server!\n");

    // Send data to the server
    strcpy(sendBuffer, "Hello, Server!");
    if (send(sockfd, sendBuffer, strlen(sendBuffer), 0) == -1) {
        perror("Send failed");
        close(sockfd);
        return EXIT_FAILURE;
    }
    printf("Message sent: %s\n", sendBuffer);

    // Receive response from server
    int recedbytes = recv(sockfd, recvBuffer, BUFFER_SIZE - 1, 0);
    if (recedbytes > 0) {
        recvBuffer[recedbytes] = '\0';  // Null-terminate the string
        printf("Server response: %s\n", recvBuffer);
    } else {
        perror("Receive failed");
    }

    // Close the socket
    close(sockfd);

    return EXIT_SUCCESS;
}

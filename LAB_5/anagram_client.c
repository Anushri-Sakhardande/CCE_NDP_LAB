#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3388
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char str1[50], str2[50];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        exit(1);
    }

    // Get user input
    printf("Enter two strings: ");
    scanf("%s %s", str1, str2);

    // Send strings to server
    snprintf(buffer, BUFFER_SIZE, "%s %s", str1, str2);
    send(sock, buffer, strlen(buffer) + 1, 0);

    // Receive response from server
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Server response: %s\n", buffer);

    // Close socket
    close(sock);
    return 0;
}

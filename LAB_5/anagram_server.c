#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 3388
#define BUFFER_SIZE 1024

// Function to check if two strings are anagrams
int areAnagrams(char *str1, char *str2) {
    int count[256] = {0};

    if (strlen(str1) != strlen(str2))
        return 0;

    for (int i = 0; str1[i] && str2[i]; i++) {
        count[(int)str1[i]]++;
        count[(int)str2[i]]--;
    }

    for (int i = 0; i < 256; i++) {
        if (count[i] != 0)
            return 0;
    }
    return 1;
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_sock);
        exit(1);
    }

    // Listen for connections
    if (listen(server_sock, 5) == -1) {
        perror("Listen failed");
        close(server_sock);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept client connection
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        if (client_sock == -1) {
            perror("Accept failed");
            continue;
        }

        // Log the connection with date and time
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char time_buffer[50];
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("[%s] Connected to client: %s:%d\n", time_buffer, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Receive two strings from client
        recv(client_sock, buffer, BUFFER_SIZE, 0);
        char str1[50], str2[50];
        sscanf(buffer, "%s %s", str1, str2);

        // Check if they are anagrams
        char result[BUFFER_SIZE];
        if (areAnagrams(str1, str2)) {
            snprintf(result, BUFFER_SIZE, "The strings '%s' and '%s' are anagrams.", str1, str2);
        } else {
            snprintf(result, BUFFER_SIZE, "The strings '%s' and '%s' are NOT anagrams.", str1, str2);
        }

        // Send response to client
        send(client_sock, result, strlen(result) + 1, 0);

        // Close client connection
        close(client_sock);
    }

    close(server_sock);
    return 0;
}

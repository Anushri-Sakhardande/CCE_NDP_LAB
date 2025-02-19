#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 3388
#define MAX_CLIENTS 2
#define BUFFER_SIZE 1024
#define FILENAME "text.txt"  // Corrected string macro

int client_count = 0;
int client_sockets[MAX_CLIENTS];

void handle_client(int client_sock, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    FILE *file;

    file = fopen(FILENAME, "a");  // Open file in append mode
    if (file == NULL) {
        perror("File opening failed");
        exit(1);
    }

    // Receive data from client
    recv(client_sock, buffer, sizeof(buffer), 0);
    printf("Received: %s from %s:%d\n", buffer, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Append message to file
    fprintf(file, "%s\t", buffer);  
    fclose(file);

    // Store message in global buffer
    strcpy(message, buffer);

    if (client_count == MAX_CLIENTS) {
        printf("Final message: %s\n", message);

        // Send the final message to all connected clients
        for (int i = 0; i < MAX_CLIENTS; i++) {
            send(client_sockets[i], message, strlen(message) + 1, 0);
        }
    } else {
        send(client_sock, "Waiting for another client...", 30, 0);
    }

    close(client_sock);
    exit(0);
}

void terminate_clients() {
    char terminate_msg[] = "terminate session";
    for (int i = 0; i < client_count; i++) {
        send(client_sockets[i], terminate_msg, sizeof(terminate_msg), 0);
        close(client_sockets[i]);
    }
    printf("Too many clients. Server terminating.\n");
    exit(0);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_sock);
        exit(1);
    }

    if (listen(server_sock, 5) == -1) {
        perror("Listen failed");
        close(server_sock);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        if (client_sock == -1) {
            perror("Accept failed");
            continue;
        }

        client_sockets[client_count] = client_sock;
        client_count++;

        if (client_count > MAX_CLIENTS) {
            terminate_clients();
        }

        if (fork() == 0) {
            close(server_sock);
            handle_client(client_sock, client_addr);
        }
        close(client_sock);
    }

    close(server_sock);
    return 0;
}

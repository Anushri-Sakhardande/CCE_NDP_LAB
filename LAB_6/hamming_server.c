#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

// Function to correct the received Hamming(7,4) code
void decodeHamming(int code[7]) {
    int p1 = code[0] ^ code[2] ^ code[4] ^ code[6]; // P1 check
    int p2 = code[1] ^ code[2] ^ code[5] ^ code[6]; // P2 check
    int p4 = code[3] ^ code[4] ^ code[5] ^ code[6]; // P4 check

    int error_pos = p1 * 1 + p2 * 2 + p4 * 4; // Error bit position

    if (error_pos > 0) {
        printf("Error detected at position: %d\n", error_pos);
        code[error_pos - 1] ^= 1; // Flip the erroneous bit
    } else {
        printf("No error detected.\n");
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    int received_code[7];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 1) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d...\n", PORT);
    
    // Accept client connection
    new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if (new_socket == -1) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // Receive data
    recv(new_socket, received_code, sizeof(received_code), 0);
    
    printf("Received code: ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", received_code[i]);
    }
    printf("\n");

    // Correct the Hamming code
    decodeHamming(received_code);

    // Send corrected code back to client
    send(new_socket, received_code, sizeof(received_code), 0);
    
    printf("Corrected code sent back to client.\n");

    // Close connections
    close(new_socket);
    close(server_fd);
    
    return 0;
}

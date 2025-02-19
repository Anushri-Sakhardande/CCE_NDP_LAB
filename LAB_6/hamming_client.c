#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

// Function to encode 4-bit data into Hamming(7,4) code
void encodeHamming(int data[4], int code[7]) {
    code[2] = data[0];
    code[4] = data[1];
    code[5] = data[2];
    code[6] = data[3];

    code[0] = code[2] ^ code[4] ^ code[6]; // P1
    code[1] = code[2] ^ code[5] ^ code[6]; // P2
    code[3] = code[4] ^ code[5] ^ code[6]; // P4
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    int data[4], code[7];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    // Input 4-bit data
    printf("Enter 4-bit data (space-separated, e.g., 1 0 1 1): ");
    for (int i = 0; i < 4; i++) {
        scanf("%d", &data[i]);
    }

    // Encode using Hamming(7,4)
    encodeHamming(data, code);

    printf("Encoded Hamming code: ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", code[i]);
    }
    printf("\n");

    // Simulate error (optional)
    int error_index;
    printf("Enter error position (1-7, 0 for no error): ");
    scanf("%d", &error_index);

    if (error_index > 0 && error_index <= 7) {
        code[error_index - 1] ^= 1; // Flip the bit
        printf("Corrupted code: ");
        for (int i = 0; i < 7; i++) {
            printf("%d ", code[i]);
        }
        printf("\n");
    }

    // Send data to server
    send(sock, code, sizeof(code), 0);

    // Receive corrected data
    recv(sock, code, sizeof(code), 0);

    printf("Corrected code received from server: ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", code[i]);
    }
    printf("\n");

    // Close socket
    close(sock);
    return 0;
}

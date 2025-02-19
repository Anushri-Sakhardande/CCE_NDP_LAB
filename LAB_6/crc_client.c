#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define POLY_12  0x80F  
#define POLY_16  0x8005  

unsigned int compute_crc(const char *data, int poly, int crc_bits) {
    unsigned int crc = 0;
    int len = strlen(data);

    for (int i = 0; i < len; i++) {
        crc ^= (data[i] << (crc_bits - 8)); // XOR with left shift

        for (int j = 0; j < 8; j++) {
            if (crc & (1 << (crc_bits - 1))) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc & ((1 << crc_bits) - 1); // Mask to fit CRC bits
}

int main() {
    int sockfd, retval;
    int sentbytes;
    struct sockaddr_in serveraddr;

    char buff[50];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("\nSocket Creation Error\n");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Connection error\n");
        close(sockfd);
        return 1;
    }

    printf("Socket connected...\n");
    printf("Enter the binary message to be sent: ");
    scanf("%s", buff);

    // Compute CRC
    unsigned int crc12 = compute_crc(buff, POLY_12, 12);
    unsigned int crc16 = compute_crc(buff, POLY_16, 16);

    // Append CRC to message
    char message_with_crc[100];
    snprintf(message_with_crc, sizeof(message_with_crc), "%s|%X", buff, crc16);

    printf("Sending correct message: %s\n", message_with_crc);
    sentbytes = send(sockfd, message_with_crc, strlen(message_with_crc), 0);
    if (sentbytes == -1) {
        printf("Sending error\n");
        close(sockfd);
        return 1;
    }

    // Simulate corruption by flipping a bit
    message_with_crc[0] = (message_with_crc[0] == '0') ? '1' : '0';

    printf("Sending corrupted message: %s\n", message_with_crc);
    sentbytes = send(sockfd, message_with_crc, strlen(message_with_crc), 0);
    if (sentbytes == -1) {
        printf("Sending error\n");
        close(sockfd);
        return 1;
    }

    // Send exit signal
    printf("Exiting...\n");
    send(sockfd, "exit", sizeof("exit"), 0);

    close(sockfd);
    return 0;
}

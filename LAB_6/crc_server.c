#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 50
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
    int sockfd, newsockfd, retval;
    socklen_t actuallen;
    int recedbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[100];  // Increased size to accommodate CRC in message

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation error\n");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;  // Fixed issue

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Binding error\n");
        close(sockfd);
        exit(1);
    }

    printf("Socket binded\n");

    retval = listen(sockfd, 1);
    if (retval == -1) {
        printf("Listening error\n");
        close(sockfd);
        exit(1);
    }

    printf("Server listening...\n");

    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1) {
        printf("Accept error\n");
        close(sockfd);
        exit(1);
    }

    printf("Server accepting connections...\n");

    while (1) {
        memset(buff, 0, sizeof(buff));  // Clear buffer before receiving
        recedbytes = recv(newsockfd, buff, sizeof(buff) - 1, 0);
        if (recedbytes == -1) {
            printf("Failed to receive message\n");
            close(sockfd);
            close(newsockfd);
            exit(1);
        }

        buff[recedbytes] = '\0';  // Ensure null termination

        if (strcmp(buff, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        printf("Received message: %s\n", buff);

        // Extract message and received CRC
        char data_part[MAXSIZE], crc_part[10];
        char *token = strtok(buff, "|");
        if (token != NULL) {
            strcpy(data_part, token);
            token = strtok(NULL, "|");
            if (token != NULL) {
                strcpy(crc_part, token);
            } else {
                printf("Error: Invalid message format\n");
                continue;
            }
        } else {
            printf("Error: Invalid message format\n");
            continue;
        }

        unsigned int received_crc = (unsigned int)strtol(crc_part, NULL, 16);
        unsigned int computed_crc = compute_crc(data_part, POLY_16, 16);

        if (computed_crc == received_crc) {
            printf("Message was not corrupted\n");
        } else {
            printf("Message has been corrupted\n");
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}

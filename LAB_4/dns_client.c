#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3388
#define MAXSIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serveraddr;
    char domain[MAXSIZE], response[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    printf("Connected to DNS server...\n");

    while (1) {
        printf("Enter domain name (or 'exit' to quit): ");
        scanf("%s", domain);

        if (strcmp(domain, "exit") == 0) {
            break;
        }

        send(sockfd, domain, strlen(domain) + 1, 0);

        recv(sockfd, response, sizeof(response), 0);
        printf("Resolved IP: %s\n", response);
    }

    close(sockfd);
    return 0;
}

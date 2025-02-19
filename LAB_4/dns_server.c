#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXSIZE 1024
#define DB_FILE "database.txt"

void lookupDomain(const char *domain, char *ipAddress) {
    FILE *file = fopen(DB_FILE, "r");
    if (!file) {
        perror("Error opening database");
        strcpy(ipAddress, "Database not found");
        return;
    }

    char line[256], fileIP[100], fileDomain[100];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%99[^,],%99s", fileIP, fileDomain) == 2) {
            if (strcmp(domain, fileDomain) == 0) {
                strcpy(ipAddress, fileIP);
                found = 1;
                break;
            }
        }
    }
    fclose(file);

    if (!found) {
        strcpy(ipAddress, "Domain not found");
    }
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t actuallen;
    char buff[MAXSIZE], response[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 5) == -1) {
        perror("Listen failed");
        close(sockfd);
        exit(1);
    }

    printf("DNS Server is running on port %d...\n", PORT);

    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1) {
        perror("Accept failed");
        close(sockfd);
        exit(1);
    }

    printf("Client connected...\n");

    while (1) {
        int recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        buff[recedbytes] = '\0';

        printf("Received domain: %s\n", buff);
        lookupDomain(buff, response);

        send(newsockfd, response, strlen(response) + 1, 0);
        printf("Sent response: %s\n", response);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}

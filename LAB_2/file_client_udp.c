#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buff[1000];
    socklen_t serlen;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("Error in socket creation\n");
        exit(1); // Use a non-zero exit code to indicate an error.
    }
    else
    {
        printf("Socket created successfully\n");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(3388);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // loopback

    serlen = sizeof(servaddr);

    while (1)
    {
        char filename[100];
        char buff[100];
        printf("\nEnter the filename: ");
        scanf("%s", filename);
        sendto(sockfd, filename, sizeof(filename), 0, (struct sockaddr *)&servaddr, serlen);
        recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, serlen);
        if (strcmp(buff, "File not found") == 0)
        {
            printf("File not found\n");
            close(sockfd);
            exit(1); // Use a non-zero exit code to indicate an error.
        }
        else
        {
            printf("File found\n");
            printf("\n1.Search\n2.Replace\n3.Reorder\n4.Exit\n");
            int choice;
            printf("Enter your choice: ");
            scanf("%d", &choice);
            sendto(sockfd, &choice, sizeof(choice), 0, (struct sockaddr *)&servaddr, serlen);
            if (choice == 1)
            {
                char search[100];
                printf("Enter the string to be searched: ");
                scanf("%s", search);
                sendto(sockfd, search, sizeof(search), 0, (struct sockaddr *)&servaddr, serlen);
                int occurrences = 0;
                recvfrom(sockfd, &occurrences, sizeof(occurrences), 0, (struct sockaddr *)&servaddr, serlen);
                printf("Occurrences: %d\n", occurrences);
            }
            else if (choice == 2)
            {
                char search[100];
                printf("Enter the string to be searched: ");
                scanf("%s", search);
                sendto(sockfd, search, sizeof(search), 0, (struct sockaddr *)&servaddr, serlen);
                char replace[100];
                printf("Enter the string to be replaced: ");
                scanf("%s", replace);
                sendto(sockfd, replace, sizeof(replace), 0, (struct sockaddr *)&servaddr, serlen);
                char result[100];
                recvfrom(sockfd, result, sizeof(result), 0, (struct sockaddr *)&servaddr, serlen);
                printf("Result: %s\n", result);
            }
            else if (choice == 3)
            {
                char result[100];
                recvfrom(sockfd, result, sizeof(result), 0, (struct sockaddr *)&servaddr, serlen);
                printf("Result: %s\n", result);
            }
            else if (choice == 4)
            {
                close(sockfd);
                exit(0);
            }
        }
    }
    close(sockfd);
    return 0;
}

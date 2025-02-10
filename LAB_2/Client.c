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

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Error in connection");
        exit(1);
    }

    while (1)
    {
        char filename[100];
        char buff[100];
        //memset(filename,0,sizeof(filename));
        printf("\nEnter the filename: ");
        scanf("%s", filename);
        send(sockfd, filename, sizeof(filename), 0);
        recv(sockfd, buff, sizeof(buff), 0);
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
            send(sockfd, &choice, sizeof(choice), 0);
            if (choice == 1)
            {
                char search[100];
                printf("Enter the string to be searched: ");
                scanf("%s", search);
                send(sockfd, search, sizeof(search), 0);
                int occurrences = 0;
                recv(sockfd, &occurrences, sizeof(occurrences), 0);
                printf("Occurrences: %d\n", occurrences);
            }
            else if (choice == 2)
            {
                char search[100];
                printf("Enter the string to be searched: ");
                scanf("%s", search);
                send(sockfd, search, sizeof(search), 0);
                char replace[100];
                printf("Enter the string to be replaced: ");
                scanf("%s", replace);
                send(sockfd, replace, sizeof(replace), 0);
                char result[100];
                recv(sockfd, result, sizeof(result), 0);
                printf("Result: %s\n", result);
            }
            else if (choice == 3)
            {
                char result[100];
                recv(sockfd, result, sizeof(result), 0);
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

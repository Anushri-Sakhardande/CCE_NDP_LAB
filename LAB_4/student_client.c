#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, retval;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr;

    int num;
    int choice = 1;
    char buff[50];
    char name[50];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("\nSocket Creation Error\n");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Connection error\n");
        return 1;
    }
    printf("Socket connected...\n");

    while (choice != 4)
    {
        printf("Enter choice:\n 1.Registration number\n 2.Student name\n 3.Subject code\n 4.exit\n");
        scanf("%d", &choice);
        getchar();

        buff[0] = choice+'0';

        switch (choice)
        {
        case 1:
            printf("Enter Registration Number: ");
            scanf("%d", &num);
            memcpy(buff + 1, &num, sizeof(num)); 
            break;
        case 2:
            printf("Enter Name: ");
            scanf("%s", name);
            strcpy(buff + 1, name);
            break;
        case 3:
            printf("Enter Subject Code: ");
            scanf("%s", name);
            strcpy(buff + 1, name);
            break;
        case 4:
            printf("Exiting...\n");
            send(sockfd, buff, sizeof(buff), 0);
            close(sockfd);
            return 0;
        default:
            printf("Incorrect option!\n");
            continue;
        }

        sentbytes = send(sockfd, buff, sizeof(buff), 0);
        if (sentbytes == -1)
        {
            printf("Failed to send message\n");
            close(sockfd);
            return 1;
        }

        recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        if (recedbytes <= 0)
        {
            printf("Failed to receive message\n");
            close(sockfd);
            return 1;
        }
        printf("Received details:\n %s\n", buff);
    }

    close(sockfd);
    return 0;
}

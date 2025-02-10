#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    int sockfd, retval, i;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr;

    int num;
    int choice = 1;
    char buff[50];
    char name[50]

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("\nSocket Creation Error");
        return;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Connection error");
        return;
    }
    printf("Socket connected...\n");

    while(choice!=4) {
        printf("Enter choice:\n 1.Registration number\n 2.Student name\n 3.Subject code\n 4.exit");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter Registration Number\n");
            scanf("%d", &num);
            buff[0] = choice;
            buff[1] = num;
            sentbytes = send(sockfd, buff, sizeof(buff), 0);
            if (sentbytes == -1)
            {
                close(sockfd);
                printf("Failed to send message");
                exit(0);
            }
            recedbytes= recv(newsockfd, buff, sizeof(buff), 0);
            printf("Receivecd details:\n%s",buff);
            break;
        case 2:
            printf("Enter Name\n");
            scanf("%d", &name);
            buff[0]=choice;
            strcpy(buff+1,name);
            sentbytes = send(sockfd, buff, sizeof(buff), 0);
            if (sentbytes == -1)
            {
                close(sockfd);
                printf("Failed to send message");
                exit(0);
            }
            recedbytes= recv(newsockfd, buff, sizeof(buff), 0);
            printf("Receivecd details:\n%s",buff);
            break;
        case 3:
            printf("Enter Subject Code\n");
            scanf("%d", &name);
            buff[0]=choice;
            strcpy(buff+1,name);
            sentbytes = send(sockfd, buff, sizeof(buff), 0);
            if (sentbytes == -1)
            {
                close(sockfd);
                printf("Failed to send message");
                exit(0);
            }
            recedbytes= recv(newsockfd, buff, sizeof(buff), 0);
            printf("Receivecd details:\n%s",buff);
            break;
        case 4:
            buff[0]=choice;
            entbytes = send(sockfd, buff, sizeof(buff), 0);
            if (sentbytes == -1)
            {
                close(sockfd);
                printf("Failed to send message");
                exit(0);
            }
            break;
        default:
            printf("Incorrect option!\n");
        }
    }


	close(sockfd);
	return 0;
}
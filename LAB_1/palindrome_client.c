#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    int sockfd, i;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr;
    socklen_t len;

    char buff[50];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("Socket creation error");
    }
    printf("Socket created\n");
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(serveraddr);

    while (1)
    {
        printf("Enter new string: ");
        scanf("%s", buff);
        if (!strcmp(buff, "stop"))
            break;
        sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        if (sentbytes == -1)
        {
            close(sockfd);
            printf("\nMessage sending Failed");
            exit(0);
        }

        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        if (recedbytes == -1)
        {
            printf("\nMessage Receiving Failed");
            close(sockfd);
            exit(0);
        }
        if (buff[0] == 1)
            printf("\nString is palindrome! Length of string is %d and it contains %d number of vowels. ", buff[1], buff[2]);
        else
            printf("\nString is not a palindrome! Length of string is %d and it contains %d number of vowels. ", buff[1], buff[2]);
    }
    close(sockfd);
}

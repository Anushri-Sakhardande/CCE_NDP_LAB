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
    char buff[50];

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

    printf("Enter the IP Address to be found:\n");
    scanf("%s",buff);

    sentbytes = send(sockfd,buff,sizeof(buff),0);
    if(sentbytes==-1) {
        close(sockfd);
        printf("Failed to send message");
        exit(0);
    }

    

}
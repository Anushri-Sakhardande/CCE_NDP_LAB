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
    int sockfd, retval;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr;

    int arr[50];
    int parity=0;
    int i=0;
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

    printf("Enter the binary message to be sent\n");
    scanf("%s",buff);

    while(buff[i]!='\0') {
        if(buff[i]=='1') {
            parity^=1;
        }
        i++;
    }
    buff[i]=parity+'0';

    //printf("%s",buff);

    printf("Sending correct message\n");
    sentbytes = send(sockfd,buff,sizeof(buff),0);
    if(sentbytes==-1) {
        printf("Sending error");
        close(sockfd);
    }

    buff[0]=(buff[0]=='1')?'0':'1';
    printf("Sending corrupted message\n");
    send(sockfd,buff,sizeof(buff),0);
    if(sentbytes==-1) {
        printf("Sending error");
        close(sockfd);
    }

    printf("Exiting..\n");
    send(sockfd,"exit",sizeof("exit"),0);
    if(sentbytes==-1) {
        printf("Sending error");
        close(sockfd);
    }

    close(sockfd);

    return 0;
}
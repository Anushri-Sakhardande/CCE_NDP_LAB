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

void crc(int n, int* arr) {

}

int main()
{
    int sockfd, retval;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr;

    int arr[50];
    int parity=0;
    int i=0;
    char buff[50];
    int arr[50];

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

    for(i=0;i<50;i++) {
        if(buff[i]!='\0') {
            arr[i] = buff[i]-'\0';
        }
        else {
            arr[i]=0;
        }
    }

    printf("Sending correct message\n");
    sentbytes = send(sockfd,arr,sizeof(arr),0);
    if(sentbytes==-1) {
        printf("Sending error");
        close(sockfd);
    }

    arr[0]=!arr[0];
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
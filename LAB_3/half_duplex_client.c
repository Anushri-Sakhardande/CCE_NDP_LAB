#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
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

    while(1) {
        printf("Enter new string: ");
        scanf("%s", buff);
        sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        if (sentbytes == -1)
        {
            printf("\nMessage Sending Failed");
            close(sockfd);
            exit(0);
        }
        if(!strcmp(buff, "stop"))
        	break;
	  
    }
}
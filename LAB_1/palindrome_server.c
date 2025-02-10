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
    int sockfd, retval, i;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
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
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    len = sizeof(clientaddr);

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Binding error");
        close(sockfd);
    }
    printf("Socket binded\n");

    while (1)
    {

        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&clientaddr, &len);
        if (recedbytes == -1)
        {
            printf("\nMessage Recieving Failed");
            close(sockfd);
            exit(0);
        }
        printf("\nMessage Recieved: ");
        printf("%s\n", buff);
        if (!strcmp(buff, "stop"))
            break;

        char buff2[50];
        strcpy(buff2, buff);
        buff[1] = strlen(buff2);
        int n = 0;
        for (int i = 0; i < buff[1]; i++)
            if (buff2[i] == 'a' || buff2[i] == 'e' || buff2[i] == 'o' || buff2[i] == 'i' || buff2[i] == 'u')
                n++;
        buff[2] = n;
        buff[0] = 1;
        for (int i = 0; i < buff[1] / 2; i++)
        {
            if (buff2[i] != buff2[buff[1] - i - 1])
            {
                buff[0] = 0;
                break;
            }
        }
        sentbytes = sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&clientaddr, len);
        if (sentbytes == -1)
        {
            printf("\nMessage Sending Failed");
            close(sockfd);
            exit(0);
        }

        if (!strcmp(buff, "stop"))
            break;
    }
    close(sockfd);
}

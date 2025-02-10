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

    int arr[50];
    int n, num;
    int choice = 1;
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

    printf("Enter the number of elements\n");
    scanf("%d", &n);

    printf("Enter the elements\n");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }
    
    sentbytes = send(sockfd,arr,sizeof(arr),0);
    if(sentbytes==-1) {
        close(sockfd);
        printf("Failed to send message");
        exit(0);
    }

    while(choice!=5) {
        printf("\n 1.Search\n2.Sort ascending\n3.Sort descending\n4.Split into odd and even\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter number to searched\n");
            scanf("%d", &num);
            buff[0] = n;
            buff[1] = choice;
            buff[2] = num;
            sentbytes = send(sockfd, buff, sizeof(buff), 0);
            if (sentbytes == -1)
            {
                close(sockfd);
                printf("Failed to send message");
                exit(0);
            }
            break;
        case 2:
            buff[0]=n;
            buff[1]=choice;
            sentbytes = send(sockfd, buff, sizeof(buff), 0);
            if (sentbytes == -1)
            {
                close(sockfd);
                printf("Failed to send message");
                exit(0);
            }
            break;
        case 3:
            buff[0]=n;
            buff[1]=choice;
            sentbytes = send(sockfd, buff, sizeof(buff), 0);
            if (sentbytes == -1)
            {
                close(sockfd);
                printf("Failed to send message");
                exit(0);
            }
            break;
        case 4:
            buff[0]=n;
            buff[1]=choice;
            sentbytes = send(sockfd, buff, sizeof(buff), 0);
            if (sentbytes == -1)
            {
                close(sockfd);
                printf("Failed to send message");
                exit(0);
            }
            break;
        case 5:
            buff[0]=n;
            buff[1]=choice;
            sentbytes = send(sockfd, buff, sizeof(buff), 0);
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
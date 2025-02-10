
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define MAXSIZE 50

int search(int *arr, int n, int num)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (arr[i] == num)
        {
            break;
        }
    }
    if (i == n)
    {
        printf("Element not found!\n");
    }
    else
    {
        printf("Element found\n");
    }
}

void sortAsc(int *arr, int n)
{
    int i = 0, j = 0;
    int swap = 0;
    for (i = 0; i < n - 1; i++)
    {
        for (j = i+1; j < n; j++)
        {
            if (arr[i] > arr[j])
            {
                swap = arr[i];
                arr[i] = arr[j];
                arr[j] = swap;
            }
        }
    }

    for(i=0;i<n;i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
}

void sortDesc(int *arr, int n)
{
    int i = 0, j = 0;
    int swap = 0;
    for (i = 0; i < n - 1; i++)
    {
        for (j = i+1; j < n; j++)
        {
            if (arr[i] < arr[j])
            {
                swap = arr[i];
                arr[i] = arr[j];
                arr[j] = swap;
            }
        }
    }

    for(i=0;i<n;i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
}

int split(int *arr, int n)
{
    int i;
    printf("Even numbers:\n");
    for (i = 0; i < n; i++)
    {
        if (arr[i] % 2 == 0)
        {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");
    printf("Odd numbers:\n");
    for (i = 0; i < n; i++)
    {
        if (arr[i] % 2 != 0)
        {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");
}

int main()
{
    int sockfd, newsockfd, retval, i;
    socklen_t actuallen;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;

    int arr[50];
    char buff[50];
    int choice = 1;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        printf("\nSocket creation error");
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == 1)
    {
        printf("Binding error");
        close(sockfd);
    }
    printf("Socket binded\n");
    retval = listen(sockfd, 1);
    if (retval == -1)
    {
        close(sockfd);
    }
    printf("Server listening...\n");
    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1)
    {
        close(sockfd);
    }
    printf("Sever accepting...\n");

    recedbytes = recv(newsockfd, arr, sizeof(arr), 0);
    if (recedbytes == -1)
    {
        close(sockfd);
        printf("Failed to receive message");
        exit(0);
    }
    printf("Array received!\n");

    while (choice != 5)
    {
        recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1)
        {
            close(sockfd);
            printf("Failed to receive message");
            exit(0);
        }
        n = buff[0];
        choice = buff[1];
        switch (choice)
        {
        case 1:
            int num = buff[2];
            search(arr, n, num);
            break;
        case 2:
            sortAsc(arr, n);
            break;
        case 3:
            sortDesc(arr, n);
            break;
        case 4:
            split(arr, n);
            break;
        case 5:
            printf("Goodbye!\n");
            break;
        default:
            printf("Error!");
        }
    }

    close(sockfd);
    close(newsockfd);
}
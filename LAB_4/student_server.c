
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define MAXSIZE 50

struct course {
    char courseName[50];
}

typedef struct{
    int regNo;
    char name[50];
    char address[50];
    char dept[50];
    int semester;
    char section;
    struct course[6];
}Student;


Student fillStudent(){
    Student s;
    s->regNo = 1;
    s->
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
    int pid;

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

    while (choice != 4)
    {
        recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes == -1)
        {
            close(sockfd);
            printf("Failed to receive message");
            exit(0);
        }
        choice = buff[0];
        switch (choice)
        {
        case 1:
            pid = fork();
            if(pid==0) {
                
            }
            break;
        case 2:
            pid = fork();
            if(pid==0) {
                
            }
            break;
        case 3:
            pid = fork();
            if(pid==0) {
                
            }
            break;
        case 4:
            printf("Goodbye!\n");
            break;
        default:
            printf("Error!");
        }
    }

    close(sockfd);
    close(newsockfd);
}
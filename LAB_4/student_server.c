#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 50

typedef struct
{
    char courseName[50];
} Course;

typedef struct
{
    int regNo;
    char name[50];
    char address[50];
    char dept[50];
    int semester;
    char section;
    Course courses[6];
} Student;

Student fillStudent()
{
    Student s;
    s.regNo = 1;
    strcpy(s.name, "John Doe");
    strcpy(s.address, "123 Street, City");
    strcpy(s.dept, "CSE");
    s.semester = 4;
    s.section = 'A';
    strcpy(s.courses[0].courseName, "Data Structures");
    strcpy(s.courses[1].courseName, "Operating Systems");
    return s;
}

int main()
{
    int sockfd, newsockfd, retval;
    socklen_t actuallen;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[50];
    int choice = 1;
    Student student = fillStudent();

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("\nSocket creation error\n");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Binding error\n");
        close(sockfd);
        return 1;
    }
    printf("Socket binded\n");

    retval = listen(sockfd, 5);
    if (retval == -1)
    {
        printf("Listen error\n");
        close(sockfd);
        return 1;
    }
    printf("Server listening...\n");

    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1)
    {
        printf("Accept error\n");
        close(sockfd);
        return 1;
    }
    printf("Server accepted connection\n");

    while (1)
    {
        recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes <= 0)
        {
            printf("Client disconnected\n");
            break;
        }

        choice = buff[0];

        switch (choice)
        {
        case 1:
            snprintf(buff, sizeof(buff), "Reg No: %d", student.regNo);
            break;
        case 2:
            snprintf(buff, sizeof(buff), "Name: %s", student.name);
            break;
        case 3:
            snprintf(buff, sizeof(buff), "Subject: %s", student.courses[0].courseName);
            break;
        case 4:
            printf("Goodbye!\n");
            close(newsockfd);
            close(sockfd);
            return 0;
        default:
            snprintf(buff, sizeof(buff), "Invalid option!");
        }

        sentbytes = send(newsockfd, buff, sizeof(buff), 0);
        if (sentbytes == -1)
        {
            printf("Failed to send message\n");
            break;
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}

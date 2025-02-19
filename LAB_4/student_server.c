#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 50
#define PORT 3388

typedef struct
{
    char courseCode[10];
    char courseName[50];
    int marks;
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

// Function to initialize a student's data
Student fillStudent()
{
    Student s;
    s.regNo = 1;
    strcpy(s.name, "John Doe");
    strcpy(s.address, "123 Street, City");
    strcpy(s.dept, "CSE");
    s.semester = 4;
    s.section = 'A';
    
    strcpy(s.courses[0].courseCode, "CS101");
    strcpy(s.courses[0].courseName, "Data Structures");
    s.courses[0].marks = 85;
    
    strcpy(s.courses[1].courseCode, "CS102");
    strcpy(s.courses[1].courseName, "Operating Systems");
    s.courses[1].marks = 78;
    
    return s;
}

int main()
{
    int sockfd, newsockfd;
    socklen_t client_len;
    struct sockaddr_in server_addr, client_addr;
    char buff[MAXSIZE];
    Student student = fillStudent();

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server settings
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Binding failed");
        close(sockfd);
        exit(1);
    }
    printf("Socket binded successfully.\n");

    // Start listening
    if (listen(sockfd, 5) == -1)
    {
        perror("Listen failed");
        close(sockfd);
        exit(1);
    }
    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        client_len = sizeof(client_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (newsockfd == -1)
        {
            perror("Accept failed");
            continue;
        }
        printf("Client connected.\n");

        // Fork a new child process for each request
        pid_t pid = fork();

        if (pid == 0) // Child process
        {
            close(sockfd);
            int received_bytes = recv(newsockfd, buff, sizeof(buff), 0);
            if (received_bytes <= 0)
            {
                perror("Receive failed");
                close(newsockfd);
                exit(0);
            }

            int choice = buff[0] - '0'; // Convert char to int
            memset(buff, 0, sizeof(buff));
            printf("%d\n",choice);
            switch (choice)
            {
            case 1:
                snprintf(buff, sizeof(buff), "PID: %d | Name: %s | Address: %s", getpid(), student.name, student.address);
                break;
            case 2:
                snprintf(buff, sizeof(buff), "PID: %d | Dept: %s | Semester: %d | Section: %c", getpid(), student.dept, student.semester, student.section);
                break;
            case 3:
                snprintf(buff, sizeof(buff), "PID: %d | Course: %s | Marks: %d", getpid(), student.courses[0].courseName, student.courses[0].marks);
                break;
            default:
                snprintf(buff, sizeof(buff), "Invalid option! Use 1, 2, or 3.");
                break;
            }

            // Send response back to client
            send(newsockfd, buff, sizeof(buff), 0);

            close(newsockfd);
            exit(0); // Terminate child process
        }
        else if (pid > 0) // Parent process
        {
            close(newsockfd); // Parent closes the client socket
        }
        else
        {
            perror("Fork failed");
            close(newsockfd);
        }
    }

    close(sockfd);
    return 0;
}

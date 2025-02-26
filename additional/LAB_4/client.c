#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, retval;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr;
    char buff[50];
    char name[50];
    int emp_id;
    float salary;
    char designation[50];
    int choice = 0;
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("\nSocket Creation Error\n");
        return 1;
    }
    
    // Set up server address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Connect to server
    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Connection error\n");
        return 1;
    }
    printf("Socket connected...\n");
    
    // Main menu loop
    while (choice != 4)
    {
        printf("\nEnter choice:\n");
        printf("1. Add employee entry\n");
        printf("2. Delete employee entry\n");
        printf("3. Update employee salary\n");
        printf("4. Get max average salary by designation\n");
        printf("5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar(); // Clear input buffer
        
        // Convert choice to character and place in first byte of buffer
        buff[0] = choice + '0';
        
        switch (choice)
        {
        case 1: // Add employee entry
            printf("Enter employee name: ");
            scanf("%s", name);
            printf("Enter employee ID: ");
            scanf("%d", &emp_id);
            printf("Enter salary: ");
            scanf("%f", &salary);
            printf("Enter designation: ");
            scanf("%s", designation);
            
            // Format data for sending
            sprintf(buff + 1, "%s,%d,%.2f,%s", name, emp_id, salary, designation);
            break;
            
        case 2: // Delete employee entry
            printf("Enter employee name to delete: ");
            scanf("%s", name);
            strcpy(buff + 1, name);
            break;
            
        case 3: // Update salary
            printf("Enter employee name: ");
            scanf("%s", name);
            printf("Enter new salary: ");
            scanf("%f", &salary);
            
            // Format data for sending
            sprintf(buff + 1, "%s,%.2f", name, salary);
            break;
            
        case 4: // Get max average salary by designation
            printf("Enter designation: ");
            scanf("%s", designation);
            strcpy(buff + 1, designation);
            break;
            
        case 5: // Exit
            printf("Exiting...\n");
            close(sockfd);
            return 0;
            
        default:
            printf("Incorrect option!\n");
            continue;
        }
        
        // Send data to server
        sentbytes = send(sockfd, buff, strlen(buff) + 1, 0); // +1 for null terminator
        if (sentbytes == -1)
        {
            printf("Failed to send message\n");
            close(sockfd);
            return 1;
        }
        
        // Receive response from server
        recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        if (recedbytes <= 0)
        {
            printf("Failed to receive message\n");
            close(sockfd);
            return 1;
        }
        
        // Ensure the received data is null-terminated
        buff[recedbytes] = '\0';
        printf("Server response:\n%s\n", buff);
    }
    
    close(sockfd);
    return 0;
}
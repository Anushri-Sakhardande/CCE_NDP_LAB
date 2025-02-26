#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#define MAXSIZE 50
#define PORT 3388
#define MAX_EMPLOYEES 100

typedef struct
{
    int id;
    char name[50];
    float salary;
    char designation[30];
} Employee;

// Global employee database
Employee employees[MAX_EMPLOYEES];
int employee_count = 0;

// Function to add an employee
void add_employee(char *data, char *response, int response_size) {
    char name[50];
    int id;
    float salary;
    char designation[30];
    
    // Parse the data: "name,id,salary,designation"
    sscanf(data, "%[^,],%d,%f,%s", name, &id, &salary, designation);
    
    if (employee_count < MAX_EMPLOYEES) {
        employees[employee_count].id = id;
        strcpy(employees[employee_count].name, name);
        employees[employee_count].salary = salary;
        strcpy(employees[employee_count].designation, designation);
        employee_count++;
        
        snprintf(response, response_size, "Employee added successfully. ID: %d, Name: %s", id, name);
    } else {
        snprintf(response, response_size, "Error: Employee database full");
    }
}

// Function to delete an employee by name
void delete_employee(char *name, char *response, int response_size) {
    int found = 0;
    int i, j;
    
    for (i = 0; i < employee_count; i++) {
        if (strcmp(employees[i].name, name) == 0) {
            found = 1;
            // Shift remaining employees to fill the gap
            for (j = i; j < employee_count - 1; j++) {
                employees[j] = employees[j + 1];
            }
            employee_count--;
            snprintf(response, response_size, "Employee '%s' deleted successfully", name);
            break;
        }
    }
    
    if (!found) {
        snprintf(response, response_size, "Error: Employee '%s' not found", name);
    }
}

// Function to update employee salary
void update_salary(char *data, char *response, int response_size) {
    char name[50];
    float salary;
    int found = 0;
    int i;
    
    // Parse the data: "name,salary"
    sscanf(data, "%[^,],%f", name, &salary);
    
    for (i = 0; i < employee_count; i++) {
        if (strcmp(employees[i].name, name) == 0) {
            found = 1;
            employees[i].salary = salary;
            snprintf(response, response_size, "Salary updated for '%s' to %.2f", name, salary);
            break;
        }
    }
    
    if (!found) {
        snprintf(response, response_size, "Error: Employee '%s' not found", name);
    }
}

// Function to get max average salary by designation
void get_max_avg_salary(char *designation, char *response, int response_size) {
    float total_salary = 0;
    int count = 0;
    int i;
    
    for (i = 0; i < employee_count; i++) {
        if (strcmp(employees[i].designation, designation) == 0) {
            total_salary += employees[i].salary;
            count++;
        }
    }
    
    if (count > 0) {
        float avg_salary = total_salary / count;
        snprintf(response, response_size, "Average salary for '%s': %.2f", designation, avg_salary);
    } else {
        snprintf(response, response_size, "No employees found with designation '%s'", designation);
    }
}

int main()
{
    int sockfd, newsockfd;
    socklen_t client_len;
    struct sockaddr_in server_addr, client_addr;
    char buff[MAXSIZE];
    int status;
    
    // Initialize with some sample data
    Employee sample_employees[5] = {
        {1, "John", 50000.0, "Manager"},
        {2, "Alice", 40000.0, "Developer"},
        {3, "Bob", 45000.0, "Developer"},
        {4, "Carol", 35000.0, "Tester"},
        {5, "David", 55000.0, "Manager"}
    };
    
    // Copy sample data to our employee database
    for (int i = 0; i < 5; i++) {
        employees[i] = sample_employees[i];
    }
    employee_count = 5;
    
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
        // Cleanup zombie processes
        while (waitpid(-1, &status, WNOHANG) > 0);
        
        client_len = sizeof(client_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (newsockfd == -1)
        {
            perror("Accept failed");
            continue;
        }
        printf("Client connected.\n");
        
        // Fork a new child process for each client
        pid_t pid = fork();
        if (pid == 0) // Child process
        {
            close(sockfd);
            
            while (1) {
                // Receive data from client
                memset(buff, 0, sizeof(buff));
                int received_bytes = recv(newsockfd, buff, sizeof(buff), 0);
                if (received_bytes <= 0)
                {
                    printf("Client disconnected or error in receive\n");
                    break;
                }
                
                int choice = buff[0] - '0'; // Convert char to int
                char *data = buff + 1; // Skip the first character (choice)
                
                printf("Received request: choice = %d, data = %s\n", choice, data);
                
                // Process the request
                memset(buff, 0, sizeof(buff));
                switch (choice)
                {
                case 1: // Add employee
                    add_employee(data, buff, sizeof(buff));
                    break;
                    
                case 2: // Delete employee
                    delete_employee(data, buff, sizeof(buff));
                    break;
                    
                case 3: // Update salary
                    update_salary(data, buff, sizeof(buff));
                    break;
                    
                case 4: // Get max average salary by designation
                    get_max_avg_salary(data, buff, sizeof(buff));
                    break;
                    
                default:
                    snprintf(buff, sizeof(buff), "Invalid option! Use 1, 2, 3, or 4.");
                    break;
                }
                
                // Send response back to client
                send(newsockfd, buff, strlen(buff) + 1, 0);
            }
            
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
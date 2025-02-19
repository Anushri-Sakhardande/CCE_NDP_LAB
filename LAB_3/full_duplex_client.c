#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
	int sockfd, new_socket;
	struct sockaddr_in servaddr;
	int recedbytes, sentbytes;
	char buff1[100],buff2[100];
	int pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error in socket creation");
        exit(1); 
    }
    else
    {
        printf("Socket created successfully\n");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(3388);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Error in connecting");
        exit(1); 
    }
    else
    {
        printf("Connected successfully\n");
    }
    
    pid = fork();

    while (1)
    {
        
        if(pid==-1) {
            printf("Unsucessful fork");
            exit(1);
        }
        if(pid==0) {
            printf("PID:%d PPID:%d \n",getpid(),getppid());
        	recedbytes = recv(sockfd, buff1, sizeof(buff1), 0);
        	if (recedbytes == -1)
		    {
		        close(sockfd);
		        printf("Failed to receive message");
		        exit(0);
		    }
		  	printf("Received message:%s\n",buff1);
        }
        else {
            printf("PID:%d PPID:%d \n",getpid(),getppid());
        	printf("Enter message");
        	scanf("%s",buff2);
        	sentbytes = send(sockfd, buff2, sizeof(buff2), 0);
        	if(sentbytes == -1) {
        		close(sockfd);
		        printf("Failed to send message");
		        exit(0);
        	}
        }
    }
}
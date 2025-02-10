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
	char buff[100];

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

    while (1)
    {
    	printf("Enter message");
    	scanf("%s",buff);
    	sentbytes = send(sockfd, buff, sizeof(buff), 0);
    	if(sentbytes == -1) {
    		close(sockfd);
	        printf("Failed to send message");
	        exit(0);
    	}
    }
}
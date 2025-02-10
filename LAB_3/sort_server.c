#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

int main() {
	int sockfd, new_socket;
	struct sockaddr_in servaddr, cliaddr;
	int recedbytes, sentbytes;
    int clilen;
    int i,j,k,n;
    int n1,n2;
	char buff[100];
    int nums[100];
    char alpha[100];
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
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Error in binding");
        exit(1); 
    }
    else
    {
        printf("Binded successfully\n");
    }

    if (listen(sockfd, 5) < 0)
    {
        perror("Error in listen");
        exit(1);
    }

    clilen = sizeof(cliaddr);
    new_socket = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);

    while(1) {
        i=0;
        j=0;
        k=0;
        //empty the strings
        memset();
        recedbytes = recv(new_socket, buff, sizeof(buff), 0);
        if (recedbytes == -1)
        {
            close(sockfd);
            printf("Failed to receive message");
            exit(0);
        }
        printf("Received message:%s \n",buff);
        n = strlen(buff);
        for(i=0;i<n;i++) {
            if(isalpha(buff[i])){
                alpha[j++]=buff[i];
            }
            else if(isdigit(buff[i])){
                nums[k++]=buff[i]-'0';
            }
        }
        n1=j;
        n2=k;

        pid=fork();
        if(pid==0) {
            int swap;
            for(i=0;i<n2-1;i++) {
                for(j=i+1;j<n2;j++) {
                    if (nums[i] > nums[j])
                    {
                        swap = nums[i];
                        nums[i] = nums[j];
                        nums[j] = swap;
                    }
                }
            }
            for(i=0;i<n2;i++) {
                printf("%d ",nums[i]);
            }
            printf("\n");
        }
        else {
            char swap;
            for(i=0;i<n1-1;i++) {
                for(j=i+1;j<n1;j++) {
                    if (alpha[i] > alpha[j])
                    {
                        swap = alpha[i];
                        alpha[i] = alpha[j];
                        alpha[j] = swap;
                    }
                }
            }
            for(i=0;i<n1;i++) {
                printf("%c ",alpha[i]);
            }
            printf("\n");
        }
    }
	
    close(sockfd);

}


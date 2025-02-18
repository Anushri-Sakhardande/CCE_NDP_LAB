#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define MAXSIZE 50

int main()
{
    int sockfd, newsockfd, retval;
    socklen_t actuallen;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
	int parity = 0;
    char buff[50];
    int i= 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        printf("\nSocket creation error");
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
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

    while(1) {
    	recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
    	
	    if (recedbytes == -1)
	    {
	        close(sockfd);
	        printf("Failed to receive message");
	        exit(0);
	    }
	    if(!strcmp(buff,"exit")) {
	    	printf("Goodbye!");
	    	break;
	    }
	    printf("Message sent:%s\n",buff);
	    parity=0;
	    i=0;
	    while(buff[i]!='\0') {
	    	if(buff[i]=='1') {
	    		parity^=1;
	    		//printf("%d\n",parity);
	    	}
	    	i++;
	    }
	    if(parity==0) {
	    	printf("Message was not corrupted\n");
	    }
	    else {
	    	printf("Message has been corrupted\n");
	    }
    }
    close(sockfd);
    close(newsockfd);
}
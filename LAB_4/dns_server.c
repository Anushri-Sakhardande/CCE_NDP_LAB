#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    int sockfd, newsockfd, retval, i;
    socklen_t actuallen;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[50];

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
    while(1) {
    	recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
	    FILE *fp = fopen(filename, "r");
	    char line[100];
	    while (fgets(line, sizeof(line), fp))
	    {
	        char* domain = strok(line,',');
	        
	    }
    }
}
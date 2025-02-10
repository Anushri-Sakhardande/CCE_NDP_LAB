#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void swap(char *x,char *y) {
    char temp;
    temp = *x;
    *x=*y;
    *y=temp;
}

void permute(char* str,int l,int r) {
    int i;
    if(l==(r-1)) {
        printf("%s\n",str);
    }
    else{
        for(i=0;i<r;i++) {
            swap((str+l),(str+i));
            permute(str,l+1,r);
            swap((str+l),(str+i));
        }
    }
}

int main() {
	int sockfd, retval, i;
    int n;
    int recedbytes, sentbytes;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;

    char buff[50];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("Socket creation error");
    }
    printf("Socket created\n");
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    len = sizeof(clientaddr);

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Binding error");
        close(sockfd);
    }
    printf("Socket binded\n");

    while (1)
    { 
        recedbytes = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&clientaddr, &len);
        if (recedbytes == -1)
        {
            printf("\nMessage Recieving Failed");
            close(sockfd);
            exit(0);
        }
        printf("\nMessage Recieved: ");
        printf("%s\n", buff);

        int n = strlen(buff);
        printf("String permutations:");
        permute(buff,0,n);

        if (!strcmp(buff, "stop"))
            break;
    }
    
}


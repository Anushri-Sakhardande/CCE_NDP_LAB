#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>

int main() {
    int sockfd,retval,sentbytes,recedbytes;
    struct sockaddr_in serveraddr;
    char buff[200];

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port=htons(3388);
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    retval = connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

    for(int i=0;i<5;i++){
        recv(sockfd,buff,sizeof(buff),0);
        printf("%s",buff);
    }

    recv(sockfd,buff,sizeof(buff),0);
    printf("%s",buff);

    char source[50];
    char dest[50];
    int seats;

    scanf("%s",source);
    scanf("%s",dest);
    scanf("%d",&seats);

    buff[0]= seats+'0';
    snprintf(buff+1,sizeof(buff),"%s,%s,",source,dest);
    send(sockfd,buff,sizeof(buff)+1,0);
    //send(sockfd,buff,sizeof(buff),0);

    recv(sockfd,buff,sizeof(buff),0);
    printf("%s",buff);

    close(sockfd);
    return 0;
}

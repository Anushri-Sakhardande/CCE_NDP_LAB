#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

typedef struct
{
    char source[50];
    char dest[50];
    int seats;
    int price;
} trains;

int main()
{
    int sockfd, retval, sentbytes, recedbytes;
    struct sockaddr_in serveraddr, cliaddr;
    socklen_t clilen;
    char buff[200];

    trains avail[5] = {
        {"mumbai", "delhi", 20, 1000},
        {"mumbai", "bangalore", 30, 500},
        {"chennai", "bangalore", 15, 500},
        {"delhi", "kolkata", 40, 500},
        {"pune", "mumbai", 60, 200}};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    retval = listen(sockfd, 5);

    while (1)
    {
        clilen = sizeof(cliaddr);
        int newsock = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        pid_t pid = fork();
        if (pid == 0)
        {
            close(sockfd);
            for (int i = 0; i < 5; i++)
            {
                snprintf(buff, sizeof(buff), "from:%s to:%s available:%d price:%d\n", avail[i].source, avail[i].dest, avail[i].seats, avail[i].price);
                send(newsock, buff, sizeof(buff), 0);
                memset(buff, 0, sizeof(buff));
            }
            strcpy(buff, "Enter source, destination and number of seats required\n");
            send(newsock, buff, sizeof(buff), 0);
            recv(newsock, buff, sizeof(buff), 0);
            char source[50];
            char dest[50];
            char garbage[20];
            int seats;
            int found = 0;
            
            

            recv(newsock,buff,sizeof(buff),0);
            seats = buff[0]-'0';
            //recv(newsock,buff,sizeof(buff),0);
            sscanf(buff+1,"%s,%s,%s",source,dest,garbage);

            printf("%s %s %d", source, dest, seats);
            for (int i = 0; i < 5; i++)
            {
                if (strcmp(avail[i].source, source) == 0 && strcmp(avail[i].dest, dest) == 0)
                {
                    found = 1;
                    if (avail[i].seats > seats)
                    {
                        int tot_price = avail[i].price * seats;
                        avail[i].seats -= seats;
                        snprintf(buff, sizeof(buff), "booked:%d seats to pay:%d\n", seats, tot_price);
                        printf("%s",buff);
                    }
                    else
                    {
                        snprintf(buff, sizeof(buff), "Not enough seats available\n");
                    }
                    break;
                }
            }
            if (found == 0)
                {
                    snprintf(buff, sizeof(buff), "train unavailable");
                }
                send(newsock, buff, sizeof(buff), 0);
            close(newsock);
            exit(0);
        }
        if (pid > 0)
        {
            close(newsock);
        }
        else
        {
            perror("Fork failed");
            close(newsock);
            exit(0);
        }
    }
}

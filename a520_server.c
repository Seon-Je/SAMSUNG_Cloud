/****************************************************************************
*
* Copyright 2018 baruncorechips All Rights Reserved.
*
* Filename: a520_server.c
* Author: enj.park, yr.kim, sj.yang
* Release date: 2018/05/02
* Version: 2.2
* Modified date : 2018/08/22 by sj.yang
*
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100
#define BUFF_SIZE 100

#define _ADC_MODE_
//#define _CHATTING_MODE_

# define BACKLOG 5


int main(int argc, char* argv[])
{

        if (argc != 2)
    {
        printf("usage : ./program PORT");
        return -1;
    }

    int PORT = atoi(argv[1]);

    int sockfd, new_fd; // socket(file) descriptor

    struct sockaddr_in my_addr; // server IP address

    memset(&my_addr, 0, sizeof(my_addr)); // initialization

    ssize_t recv_num; // for check return value of recv function
    char recv_data[BUFFER_SIZE];
    char send_data[BUFFER_SIZE];
    pid_t pid; // process id

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    my_addr.sin_family = AF_INET; // IPv4
    my_addr.sin_port = htons(PORT); // Server Port
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // server IP address


    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) == -1)
    {
        printf("bind error\n");
        return -1;
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        printf("listen error\n");
                return -1;
    }

    printf("Waiting clients\n");

    while (1)
    {
        struct sockaddr_in their_addr, peer_addr; // client IP

        socklen_t their_addrLength = sizeof(their_addr);

        while((new_fd = accept(sockfd, (struct sockaddr*)&their_addr, (socklen_t *)&their_addrLength)) >= 0)
        {
            getpeername(new_fd, (struct sockaddr*)&peer_addr, &their_addrLength);

            char peer_ip[sizeof(peer_addr.sin_addr) + 1] = { 0 };
            sprintf(peer_ip, "%s", inet_ntoa(peer_addr.sin_addr));

            if(strcmp(peer_ip,"0.0.0.0") != 0)
            {
                printf("Client IP : %s\n", peer_ip);
            }

            if (new_fd < 0)
            {
                printf("accept error\n");
                                exit(0);
            }

            pid = fork();

            if(pid == 0)
            {
                close(sockfd);
                ssize_t recv_num;

                                while((recv_num = recv(new_fd, recv_data, BUFF_SIZE, 0)) > 0)
                                {
                                        recv_data[recv_num] = '\0';
#ifdef _ADC_MODE_
                    FILE *fp = fopen("send_data.txt","w");
                    printf("From Client(%s) : %d\n", peer_ip, recv_data[0]);
                    fprintf(fp, "%d",recv_data[0]);
                    fclose(fp);
#endif

#ifdef _CHATTING_MODE_
                    printf("From Client(%s) : %s", peer_ip, recv_data);
#endif

                    sprintf(send_data,"%s",recv_data);
                    send(new_fd, send_data, strlen(send_data), 0);
                }

                close(new_fd);
                return 0;
            }

            else
            {
                close(new_fd);
            }
        }

    }
    close(sockfd);

    return 0;
}
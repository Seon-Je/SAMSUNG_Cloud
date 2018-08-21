/****************************************************************************
*
* Copyright 2018 Primesoft All Rights Reserved.
*
* Filename: 522_LWIPClient.c
* Author: enj.park, yr.kim
* Release date: 2018/05/02
* Version: 2.1
* Modified date : 2018/05/23 by sj.yang
*
****************************************************************************/

#include <net/lwip/sockets.h>
#include <string.h>
#include <stdlib.h>
#include <readline.h>
#include "adc.h"

#define _ADC_MODE_
//#define _CHATTING_MODE_

#define BACKLOG 10
#define MAX_DATA_SIZE 100

#ifdef _ADC_MODE_
	#define A0			0 //
	#define A1			1 //
	#define A2			2 //
	#define A3			3 //

	int TEMP = A0;
#endif

int client_main(int argc, char *argv[])
{
	int s;
	int ret;
	struct sockaddr_in server_addr;
	char str[MAX_DATA_SIZE];

	int PORT = atoi(argv[3]);

	//int sockfd;
	//int numbytes;
	//char * string = "Hello, ARTIK053! This is ARTIK520!";
	//struct hostent * he;

	// set up address to connect to
	printf("Set up address...\n");
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_len = sizeof(server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT); //port
	server_addr.sin_addr.s_addr = inet_addr(argv[2]); //host

	// create the socket
	printf("Create the socket...\n");
	s = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
    	printf("[Error] lwip_socket()\n");
    	return -1;
    }

    // connect
    printf("Connect to server...\n");
    ret = lwip_connect(s, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret == -1)
    {
    	printf("[Error] lwip_connect()\n");
    	return -1;
    }

    while(1)
    {
    	// write

#ifdef _CHATTING_MODE_
    	printf("Please enter the data [q:exit] \n");
    	std_readline(str, MAX_DATA_SIZE);
    	int len = strlen(str);
    	printf("Send data ...\n", str);
#endif

#ifdef _ADC_MODE_
    	int temp = read_adc(TEMP);
    	temp = temp * 330 / 4096 - 50;
    	str[0] = temp;
    	str[1] = '\n';
		up_mdelay(1000);
    	int len = strlen(str);
    	printf("ADC%d value is %d \n", TEMP, str[0]);
#endif
    	ret = lwip_write(s, str, len);
    	if (ret != (int)len)
    	{
    		printf("[Error] lwip_write()\n");
    	    return -1;
    	}

    	if ((len == 2) && (str[0] == 'q'))
    	{
        	// close
        	printf("Close socket...\n");
        	ret = lwip_close(s);
        	if (ret == -1)
        	{
        	    printf("[Error] lwip_close()\n");
        	    return -1;
        	}
        	break;
    	}
    }
    return 0;
}

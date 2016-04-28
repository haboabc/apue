#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

#include "proto.h"

int main(int argc,char **argv)
{
	int sd;
	struct msg_st *sbuf;
	int size;
	struct sockaddr_in raddr;

	if(argc < 3)
	{
		fprintf(stderr,"Usage....\n");
		exit(1);
	}
	
	if(strlen(argv[2]) > NAMEMAX-1)
	{
		fprintf(stderr,"name is too long.\n");
		exit(1);
	}

	size = sizeof(struct msg_st) + strlen(argv[2]);
	sbuf = malloc(size);//发多大的包，申请多大内存
	if(sbuf == NULL)
	{
		perror("malloc()");
		exit(1);
	}

	sd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    if(sd < 0)
    {
        perror("socket()");
        exit(1);
    }

	srand(time(NULL));
	strcpy(sbuf->name,argv[2]);
	sbuf->math = htonl(rand()%100);
	sbuf->chinese = htonl(rand()%100);
#ifdef PRINT
	printf("math = %d\n", ntohl(sbuf->math));
	printf("chinese = %d\n", ntohl(sbuf->chinese));
#endif

	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(atoi(RCVPORT));
	inet_pton(AF_INET, "172.17.0.89", &raddr.sin_addr);

	if(sendto(sd,sbuf,size,0,(void *)&raddr,sizeof(raddr)) < 0)
	{
		perror("sendto()");
		exit(1);
	}

	puts("ok!");

	free(sbuf);
	close(sd);

	exit(0);
}



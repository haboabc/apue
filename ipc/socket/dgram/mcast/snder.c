#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "proto.h"

int main(int argc,char **argv)
{
	int sd;
	struct msg_st sbuf;
	struct sockaddr_in raddr;
	struct ip_mreqn mreq;

	sd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
    if(sd < 0)
    {
        perror("socket()");
        exit(1);
    }

	
	inet_pton(AF_INET,MGROUP,&mreq.imr_multiaddr);//组ip
	inet_pton(AF_INET,"0.0.0.0",&mreq.imr_address); //本机ip
	mreq.imr_ifindex = if_nametoindex("eth0");//指定网卡
	if(setsockopt(sd,IPPROTO_IP,IP_MULTICAST_IF,&mreq,sizeof(mreq)) < 0)//创建组
	{
		perror("setsockopt()");
		exit(1);
	}
	
	memset(&sbuf,'\0',sizeof(sbuf));
	strcpy(sbuf.name,"Alan");
	sbuf.math = htonl(rand()%100);
	sbuf.chinese = htonl(rand()%100);

	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(atoi(RCVPORT));
	inet_pton(AF_INET,MGROUP,&raddr.sin_addr);//组播就是往组ip里发包

	if(sendto(sd,&sbuf,sizeof(sbuf),0,(void *)&raddr,sizeof(raddr)) < 0)
	{
		perror("sendto()");
		exit(1);
	}

	puts("ok!");

	close(sd);

	exit(0);
}






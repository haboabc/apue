#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include "proto.h"

#define BUFSIZE		1024
#define STRSIZE		40
#define PROCNUM		4 //进程(服务器数量？）数量

static void server_loop(int sd);

static void server_job(int sd)
{
	int len;
	char buf[BUFSIZE];

	len = sprintf(buf,FMT_STAMP,(long long)time(NULL));

 	sleep(5);

	if(send(sd,buf,len,0) < 0)
	{
		perror("send()");
		exit(1);
	}
}

int main()
{
	int sd;
	int i;
	pid_t pid;
	struct sockaddr_in laddr;

	sd = socket(AF_INET,SOCK_STREAM,0/*IPPROTO_TCP,IPPROTO_SCTP*/);
	if(sd < 0)
	{
		perror("socket()");
		exit(1);
	}

	int val = 1;
	if(setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) < 0)
	{
		perror("setsockopt()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(SERVERPORT));
	inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
	if(bind(sd,(void *)&laddr,sizeof(laddr)) < 0)
	{
		perror("bind()");
		exit(1);
	}

	if(listen(sd,200) < 0)
	{
		perror("listen()");
		exit(1);
	}
	
	for(i = 0 ; i < PROCNUM; i++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork()");
			exit(1);
		}
		if(pid == 0)		// child
		{
			server_loop(sd);// 这个监听的fd何以被多个进程共享?从而在不同的机器上布置多个服务器?
			exit(0);
		}
	}

	for(i = 0 ; i < PROCNUM; i++)
		wait(NULL);

	exit(0);
}

static void server_loop(int sd)
{
	/**
 	 * 各子进程拿到的是与父进程一样的sd，因为是传参传过来的
	 * 并不是各自的进程表项里的
	 * 哪个子进程能抢到任务并不知道?还是将一个服务器变成了多个单独的服务器？
	 * */

	int newsd;
    struct sockaddr_in raddr;
    socklen_t raddr_len;
    char ipstr[STRSIZE];

	raddr_len = sizeof(raddr);

	while(1)
	{
		newsd = accept(sd,(void *)&raddr,&raddr_len);
		if(newsd < 0)
		{
			if(errno == EINTR || errno == EAGAIN)
				continue;
			perror("accept()");
			exit(1);
		}
	
		inet_ntop(AF_INET,&raddr.sin_addr,ipstr,STRSIZE);
		printf("Client:%s:%d\n",ipstr,ntohs(raddr.sin_port));
		server_job(newsd);
		close(newsd);
	}

	close(sd);
}

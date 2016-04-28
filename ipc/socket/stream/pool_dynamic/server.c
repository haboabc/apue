/******************************
 * 模型像超市的收款通道
 * ***************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>

#include "proto.h"

#define BUFSIZE				1024
#define STRSIZE				40

#define MAXCLIENT			20 //同一时刻允许的client数(20)个通道
#define MINSPARESERVER		5  //
#define MAXSPARESERVER		10 //允许空闲的最大个数

#define SIG_NOTIFY			SIGUSR1

enum //相当于＃define 了一堆常量
{
	STATE_IDLE=0,
	STATE_BUSY,
	STATE_CLOSE
};

struct server_st //服务器结构体,包含服务器当前的所有状态
{
	pid_t pid;
	int state;
};


struct server_st *serverpool;// 所有通道的状态
static int idle_count = 0; // 全局变量，空闲个数
static int busy_count = 0; // 全局变量，忙的个数
static int sd;

static void server_loop(int sd); //开着的通道一直loop工作或等待工作


static void usr1_handler(int s)// 通知状态改变
{
	return ;
}


static int add_1_server(void)
{
	int i;
	pid_t pid;

	if(idle_count + busy_count >= MAXCLIENT)//最多20个通道
		return -1;	

	for(i = 0 ; i < MAXCLIENT; i++)
		if(serverpool[i].pid == -1)
			break;//找到拥有最小下标的空闲的通道	

	serverpool[i].state = STATE_IDLE;//真正有client来时为busy，活着但没有client时为idle，其他时候为close
	pid = fork();	
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}
	if(pid == 0)
	{
		server_loop(i);
		exit(0);
	}
	else
	{
		serverpool[i].pid = pid;
	}
	return 0;
}

static int del_1_server(void)
{
	int i;

	if(idle_count <= MAXSPARESERVER)//不用关一个通道
		return -1;
	
	for(i = 0 ; i < MAXCLIENT; i++)
	{
		if(serverpool[i].pid != -1&&serverpool[i].state == STATE_IDLE)//找到空闲的开着的通道
		{
			kill(serverpool[i].pid,SIGTERM);//发信号杀死进程
			idle_count --;	
			serverpool[i].pid = -1;
			break;
		}
	}
	return 0;
}

static int scan_pool(void)
{
	int i;
	int idel = 0,busy = 0;

	for(i = 0 ; i < MAXCLIENT; i++)
	{
		if(serverpool[i].pid == -1)
			continue;
		if(kill(serverpool[i].pid,0)) //检查是否真的活着
		{
			serverpool[i].pid = -1;
			continue;
		}

		if(serverpool[i].state == STATE_IDLE)
			idel ++;
		else if(serverpool[i].state == STATE_BUSY)
				busy ++;
			else
			{
				fprintf(stderr,"Unknown state.\n");
				abort();
			}
	}

	idle_count = idel;
	busy_count = busy;
	return 0;
}



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
	int i;
	pid_t pid;
	struct sockaddr_in laddr;
	struct sigaction sa,osa1,osa2;
	sigset_t set,oset;

	sa.sa_handler = SIG_IGN;// 信号的默认动作//linux 下这样做，不会产生僵尸进程，父进程不用wait，其他系统在信号处理函数里wait，功能类似.
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_NOCLDWAIT;// 只在SIG_DEF时才不产生僵尸进程，SIG_IGN时没啥用
	sigaction(SIGCHLD,&sa,&osa1);//子进程退出时，会向父进程放送SIGCHLD信号
	/*if error*/

	sa.sa_handler = usr1_handler;
	sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
	sigaction(SIG_NOTIFY,&sa,&osa2);//用户预留信号，用于通知一些状态改变
	/*if error*/

	sigemptyset(&set);
	sigaddset(&set,SIG_NOTIFY);
	sigprocmask(SIG_BLOCK,&set,&oset);


	serverpool = mmap(NULL,sizeof(struct server_st)*MAXCLIENT,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
	if(serverpool == MAP_FAILED)
	{
		perror("mmap()");
		exit(1);
	}

	for(i = 0 ; i < MAXCLIENT; i++)
		serverpool[i].pid = -1;

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
	

	for(i = 0 ; i < MINSPARESERVER ; i++)//开启通道（允许的最小的空闲的）
		add_1_server();


	while(1)
	{
		sigsuspend(&oset);

		scan_pool();

		if(idle_count > MAXSPARESERVER)//空闲的太多
		{
			for(i = 0 ; i < idle_count-MAXSPARESERVER; i++)
				del_1_server();
		}
		else if(idle_count < MINSPARESERVER) //空闲的太少
			{
				for(i = 0 ; i < MINSPARESERVER-idle_count; i++)
					add_1_server();
			}

		for(i = 0 ; i < MAXCLIENT; i++)
		{
			if(serverpool[i].pid == -1)
				putchar(' ');//通道关闭
			else if(serverpool[i].state == STATE_IDLE)
					putchar('.');//通道空闲
				else 
					putchar('x');//通道忙
               
		}
		printf("\n");

	}

	/*free destroy*/
	sigprocmask(SIG_SETMASK,&oset,NULL);

	close(sd);

	exit(0);
}

static void server_loop(int pos)
{
	int newsd;
    struct sockaddr_in raddr;
    socklen_t raddr_len;
    char ipstr[STRSIZE];
	pid_t ppid;

	ppid = getppid();
	
	raddr_len = sizeof(raddr);

	while(1)
	{
		serverpool[pos].state = STATE_IDLE;
		kill(ppid,SIG_NOTIFY);//告诉父进程服务器状态发生改变
		
		newsd = accept(sd,(void *)&raddr,&raddr_len);
		if(newsd < 0)
		{
			if(errno == EINTR || errno == EAGAIN)
				continue;
			perror("accept()");
			exit(1);
		}
	
		serverpool[pos].state = STATE_BUSY;
		kill(ppid,SIG_NOTIFY);

//		inet_ntop(AF_INET,&raddr.sin_addr,ipstr,STRSIZE);
//		printf("Client:%s:%d\n",ipstr,ntohs(raddr.sin_port));
		server_job(newsd);
		close(newsd);
	}

	close(sd);
}


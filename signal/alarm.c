/* in liunx alarm() doesn't conflict against sleep() ?*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void alrm_handler(int s)
{
	alarm(1); //循环产生alarm信号
	printf("Got SIGALRM\n");
	return ;
}

int main(int argc, char *argv[])
{
	signal(SIGALRM,alrm_handler);
	alarm(1);// 调用一次，只产生一次信号

	while(1);

	exit(0);
}


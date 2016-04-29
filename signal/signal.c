#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void int_handler(int s)
{
	write(1,"!",1);
}

int main(int argc, char *argv[])
{
	int i;

	signal(SIGINT,int_handler);
	
	for(i = 0; ++i;) 
	{
		write(1,"*",1);
		sleep(1);
	}

	exit(0);
}

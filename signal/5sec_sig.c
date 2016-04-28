#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static volatile int loop = 1;//每次去内存读取，而不在寄存器里度缓存的值.

static void alrm_handler(int s)
{
	loop = 0;
}

int main()
{
	int64_t count = 0;

	signal(SIGALRM,alrm_handler);
	alarm(5);

	while(loop)
		count++;

	printf("%lld\n",count);

	exit(0);
}

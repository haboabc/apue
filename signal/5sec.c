#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
	time_t end;// time_t 存储的是妙数.
	int64_t count = 0;

	end = time(NULL)+5;

	while(time(NULL) <= end) //函数调用相比访问内存会消耗大量时间
		count++;

	printf("%lld\n",count);

	exit(0);
}


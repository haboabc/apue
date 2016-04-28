#include <stdio.h>
#include <stdlib.h>


/*
 *	缓冲区作用：大多数缓冲区的存在是件好事，作用是合并系统调用。
 *
 *	行缓冲：换行的时候刷新，满了的时候刷新，强制刷新（stdout,因为涉及到终端设备）
 *
 *	全缓冲：满了的时候刷新，强制刷新（默认，只要不是终端设备）
 *
 *	无缓冲：需要立即输出，如stderr
 *
 *
 * */


int main()
{
	int i = 1;

	i = 10;

	printf("[%s:%d]before while()",__FUNCTION__,__LINE__);
//	fflush(stdout);
	fflush(NULL);

	while(1)
		;

	printf("[%s:%d]after while()",__FUNCTION__,__LINE__);
	fflush(NULL);
	
	printf("i = %d\n",i);

	exit(0);
}



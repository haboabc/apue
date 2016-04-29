#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFSIZE		1024

int main(int argc,char **argv)
{
	int sfd,dfd;
	char buf[BUFSIZE];
	int len,ret;
	int pos;

	if(argc < 3)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}

	do // 有信号参与的文件循环打开方法 do while () 至少作一次
	{
		sfd = open(argv[1],O_RDONLY);
		if(sfd < 0)
		{
			if(errno != EINTR)// EINTR 表示有信号打断
			{
				perror("open()");
				exit(1);
			}
		}
	}while(sfd < 0);

	do
	{
		dfd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0600);
		if(dfd < 0)
		{
			if(errno != EINTR)
			{
				perror("open()");
				exit(1);
			}
		}
	}while(dfd < 0);

	while(1)// 缺点，阻塞，
	{
		len = read(sfd,buf,BUFSIZE);
		if(len < 0) //读取出错。
		{
			if(errno == EINTR)
				continue;
			perror("read()");
			break;
		}
		if(len == 0)//读取完毕, 即到了EOF.
			break;

		// len > 0 未读取完毕。

		pos = 0;
		while(len > 0) //写够len 个字节
		{
			ret = write(dfd,buf+pos,len);
			if(ret < 0)
			{
				if(errno == EINTR)
					continue;
				perror("write()");
				exit(1);
			}
			pos += ret;
			len -= ret;
		}
	}	

	close(dfd);
	close(sfd);

	exit(0);
}

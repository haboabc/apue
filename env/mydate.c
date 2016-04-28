#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define STRSIZE		128

int main(int argc,char **argv)
{
	time_t stamp;	
	struct tm *tm;
	char timestr[STRSIZE];
	int ch;
	char fmt[STRSIZE] = {'\0'};
	FILE *fp = stdout;

	stamp = time(NULL);
	tm = localtime(&stamp);

	while(1)
	{		
		ch = getopt(argc,argv,"-y:mdH:MS");//argv 为以尾0为结尾的字符串数组
										   // "-y..." 是各选项
		if(ch < 0)
			break;

		switch(ch)
		{
			case 1: //? 由于解析只识别-开头的参数,非－开头的有不是选项参数的返回1
				printf("why got this ch = %d\n", ch);
				if(fp == stdout)//只打开一次文件
				{	
					fp = fopen(argv[optind-1],"w");// optind 永远指向下一个要解析的
													// 当前解析的是optind - 1
					if(fp == NULL)
					{
						perror("fopen()");
						fp = stdout;	
					}
				}
				break;	

			case 'y':// 带冒号的必须后跟参数且参数字符串指针为optarg
				if(strcmp(optarg,"2") == 0)
					strncat(fmt,"%y ",STRSIZE);
				else if(strcmp(optarg,"4") == 0)
						strncat(fmt,"%Y ",STRSIZE);
					 else
						fprintf(stderr,"Invalid argument of -y\n");
				break;

			case 'm':
				strncat(fmt,"%m ",STRSIZE);
				break;

			case 'd':
				strncat(fmt,"%d ",STRSIZE);
				break;
	
			case 'H':
				if(strcmp(optarg,"12") == 0)
                    strncat(fmt,"%I(%P) ",STRSIZE);
                else if(strcmp(optarg,"24") == 0)
                        strncat(fmt,"%H ",STRSIZE);
                     else
                        fprintf(stderr,"Invalid argument of -H\n");
                break;


				break;
		
			case 'M':
				strncat(fmt,"%M ",STRSIZE);
				break;

			case 'S':
				strncat(fmt,"%S ",STRSIZE);
				break;

			default:
				/*do sth*/
				break;
		}
	}

	strncat(fmt,"\n",STRSIZE);
	strftime(timestr,STRSIZE,fmt,tm);
	fputs(timestr,fp);

	if(fp != stdout)
		fclose(fp);

	exit(0);
}




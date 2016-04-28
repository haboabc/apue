#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STRSIZE		128

int main()
{
	time_t stamp;	
	struct tm *tm;
	char timestr[STRSIZE];

	stamp = time(NULL);
	tm = localtime(&stamp);
	strftime(timestr,STRSIZE,"Now:%Y-%m-%d",tm);
	puts(timestr);

	tm->tm_mday += 100;
	mktime(tm);
	strftime(timestr,STRSIZE,"100 days later:%Y-%m-%d",tm);
    puts(timestr);



	exit(0);
}




#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ssize_t mygetline(char **lineptr, size_t *n, FILE *stream)
{


}

#if 0
void mygetline_free(char *)
{


}
#endif

int main(int argc,char *argv[])
{
	FILE *fp;
	char *linebuf = NULL;
	size_t linebufsize = 0;

	if(argc < 2)
	{
		fprintf(stderr,"Usage....\n");
		exit(1);
	}
	
	fp = fopen(argv[1],"r");
	if(fp = NULL)
	{
		perror("fopen()");
		exit(1);
	}

	while(1)
	{
		if(mygetline(&linebuf,&linebufsize,fp) < 0)
			break;

		printf("linebufsize = %d\n",linebufsize);
		printf("%d\n",strlen(linebuf));
	}

//	free(linebuf);

	fclose(fp);
	//mygetline_free();

	exit(0);
}



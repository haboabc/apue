#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

#define PAT		"/etc/a*.conf"


int gl_errfunc(const char *epath, int eerrno)
{
	fprintf(stderr,"[%s]failed:%s",epath,strerror(eerrno));
	return 0;
}

int main()
{
	int err,i;
	glob_t globres;

	err = glob(PAT,0,NULL,&globres);
	if(err)
	{
		fprintf(stderr,"glob() error:%d\n",err);
		exit(1);
	}

	for(i = 0 ; i < globres.gl_pathc ; i++)
		puts(globres.gl_pathv[i]);

	
	globfree(&globres);
	
	exit(0);
}




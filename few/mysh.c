#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>
#include <unistd.h>

#define DELIMS		" \t\n"

struct cmd_st
{
	glob_t globres;
};

static void prompt(void)
{
	printf("mysh-0.1$ ");
}

static void parse(char *line,struct cmd_st *res)
{// ls -a /etc -i -l /tmp  
	char *tok;
	glob_t globres;	
	int i = 0;

	while(1)
	{
		tok = strsep(&line,DELIMS);
		if(tok == NULL)
			break;	
		if(tok[0] == '\0')
			continue;
		
		glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL,&res->globres);
		i = 1;
		/*if error*/
	}
}

int main()
{
	char *linebuf = NULL;
	size_t linebufsize = 0;
	struct cmd_st cmd;
	pid_t pid;

	while(1)
	{
		prompt();
	
		if(getline(&linebuf,&linebufsize,stdin) < 0)
			break;

		parse(linebuf,&cmd);	
			
		if(0/*是内部命令*/)
		{	}
		else		// 外部命令处理
		{
			pid = fork();
			if(pid < 0)
			{
				perror("fork()");
				exit(1);
			}

			if(pid == 0)
			{
				execvp(cmd.globres.gl_pathv[0],cmd.globres.gl_pathv);
				perror("execvp()");
				exit(1);
			}

			wait(NULL);
		}
	}
	exit(0);
}

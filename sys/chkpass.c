#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>

int main(int argc,char **argv)
{
	struct spwd *shdowline;	
	char *inputpass;
	char *crypted_pass;

	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}	

	inputpass = getpass("Password:");
	/*if error*/

	shdowline = getspnam(argv[1]);
	/*if error*/
		
	crypted_pass = crypt(inputpass,shdowline->sp_pwdp);
	/*if error*/
	

	if(strcmp(crypted_pass,shdowline->sp_pwdp) == 0)
		puts("ok");
	else
		puts("failed");

	exit(0);
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int opt;

	while ( -1 != (opt = getopt(argc, argv, "-m:M")) ) {
		switch (opt) {
			case 1:
				printf(" got 1 stands for not a member of optstring\n");
				break;
			case 'm':
				puts(optarg);
				break;
			case 'M':
				puts("got M");
				break;
			default :
				break;

		}			
	}

	exit(0);
}

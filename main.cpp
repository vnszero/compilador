#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	if(argc!=2){
		printf("usage: %s [program]\n", argv[0]);
		exit(1);
	}
	return 0;
}

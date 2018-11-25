#include <stdio.h>

int main(int argc, char *argv[]) {

#ifdef DEBUG
	printf("DEBUG is defined\n");
#else
	printf("DEBUG is not defined\n");
#endif

	return 0;
}


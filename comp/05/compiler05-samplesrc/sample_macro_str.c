#include <stdio.h>

#define STR(var) #var

typedef enum {
	IntNum,
	Symbol
} Kind;

int main(int argc, char *argv[]) {

	printf("%s\n", STR(IntNum));
	printf("%s\n", STR(int));

	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* definition */

/* global variable */
FILE *fp;

/* prototype declaration */
int nextChar(void);


int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("no file name\n");
		exit(EXIT_FAILURE);
	}

	/* open file argv[1] */
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("error: file %s can not open\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	/* Step1の動作確認用コード */
	int ch;
	while ((ch = nextChar()) != EOF) {
		printf("%c,", (char)ch);
	}
	printf("\n");


	/* close file argv[1] */
	fclose(fp);

	exit(EXIT_SUCCESS);
}

/*
 * ファイルから次の1文字を取得する
 * (nextCharのファイル版)
 */
int nextChar(void) {
	static int ch;

	ch = fgetc(fp);
	
	return ch;
}


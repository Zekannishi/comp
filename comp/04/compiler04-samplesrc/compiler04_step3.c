#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/* definition */
#define STR_SIZE 256

/* global variable */
FILE *fp;
char Str[STR_SIZE + 1];

/* prototype declaration */
int nextChar(void);
char* nextBinaryToken();
int binaryToken2int(char *p);


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

	/* nextBinaryToken関数によるトークンの取得と表示 */
	char *head;
	int val;
	while ((head = nextBinaryToken()) != NULL) {
		val = binaryToken2int(head);
		printf("%s (%d)\n", head, val);
	}


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

	/* Step1 */
	
	return ch;
}

/*
 * ファイルから記号を1文字ずつ読み，トークンを取得する
 */
char* nextBinaryToken() {
	static int ch = ' ';
	char *pStr = Str;
	char *head = pStr;

	/* 空白の読み飛ばし */
	ch = nextChar();
	while (isspace(ch)) {
		ch = nextChar();
	}

	/* 最後に読んだ文字がEOFの場合，NULLを返す */
	if (ch == EOF) {
		return NULL;
	}

	/* トークンの切り出し */
	/* Step2 */

	return head;
}


/*
 * 2進数を表わすトークンを整数値に変換する
 */
int binaryToken2int(char *p) {

	/* Step3 */

	return val;
}

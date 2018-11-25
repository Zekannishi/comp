#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 255

/* グローバル変数 */
char Str[MAX_SIZE+1]; /* 文字列格納用の配列 */

/* プロトタイプ宣言 */
void getWord(void);
char nextChar(void);

int main(void) {
	char ch;

	/* 記号列の読み込み */
	getWord();

P:
	ch = nextChar();
	if (ch == 'a') { printf("d(p, %c) = q\n", ch); goto Q; }
	if (ch == '\0') { goto REJECT; }
	goto D;
	
Q:
	ch = nextChar();
	if (ch == 'a') { printf("d(q, %c) = q\n", ch); goto Q; }
	if (ch == 'b') { printf("d(q, %c) = q\n", ch); goto R; }
	if (ch == '\0') { goto REJECT; }
	goto D;

R:
	ch = nextChar();
	if (ch == 'b') { printf("d(r, %c) = r\n", ch); goto R; }
	if (ch == '\0') { goto ACCEPT; }
	goto D;

D:
	ch = nextChar();
	if (ch == '\0') { goto REJECT; }
	goto D;

ACCEPT:
	printf("yes\n");
	exit(EXIT_SUCCESS);

REJECT:
	printf("no\n");
	exit(EXIT_SUCCESS);

	return 0;
}

void getWord(void) {
	printf("w=");
	scanf("%255s", Str);
}

char nextChar(void) {
	static char *pStr = Str;
	char c;

	if (*pStr != '\0') {
		c = *pStr;
		pStr++;
	} else {
		c = *pStr;
	}

	return c;
}


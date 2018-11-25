#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 255

/* 状態の定義 */
typedef enum {
	Sp, Sq, Sr, Sd
} STATE;

/* グローバル変数 */
char Str[MAX_SIZE+1]; /* 文字列格納用の配列 */

/* プロトタイプ宣言 */
void getWord(void);
char nextChar(void);

int main(void) {
	STATE s = Sp; /* q0 = p */
	char ch;

	/* 記号列の読み込み */
	getWord();

	/* 1文字ずつ読みオートマトンを動作させる */
	while ((ch = nextChar()) != '\0') {
		switch (s) {
			case Sp:
				/* 演習3-1 */
		}
	}

	/* 受理判定 */
	if (s == Sr) { /* F = {r} */
		printf("yes\n");
	} else {
		printf("no\n");
	}

	exit(EXIT_SUCCESS);
}

void getWord(void) {
	printf("w = ");
	scanf("%255s", Str);
}

char nextChar(void) {
	static char *pStr = Str;
	char c;

	/* lesson02, Step2 */

	return c;
}


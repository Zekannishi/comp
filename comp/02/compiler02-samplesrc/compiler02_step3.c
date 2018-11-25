#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // C99

#define MAX_SIZE 255

/* グローバル変数 */
char Sigma[] = {'a', 'b'}; /* アルファベットの有限集合 */
char Str[MAX_SIZE + 1]; /* 文字列格納用の配列 */

/* プロトタイプ宣言 */
void getWord(void);
char nextChar(void);
bool isAlphabet(char c);

int main(void) {
	/* ローカル変数の宣言 */
	char ch;

	/* 記号列の読み込み */
	getWord();
	printf("word: %s\n", Str);

	while ((ch = nextChar()) != '\0') {
		if (isAlphabet(ch)) {
			printf("%c(Y) ", ch); /* Step3の動作確認用 */
		} else {
			printf("%c(N) ", ch); /* Step3の動作確認用 */
		}
	}
	printf("\n"); /* Step3の動作確認用 */

	exit(EXIT_SUCCESS);
}

void getWord(void) {
	/* Step1をここに実装する */
}

char nextChar(void) {
	static char *pStr = Str;
	char c;

	/* Step2をここに実装する */

	return c;
}

bool isAlphabet(char c) {
	char *p = Sigma;

	/* Step3をここに実装する */

	return false;
}


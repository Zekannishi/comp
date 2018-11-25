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
	bool isWord;

	/* 記号列の読み込み */
	getWord();
	printf("word: %s\n", Str);

	/* 判定 */
	/* Step4をここに実装する */
	/*
	 * 参考: 演習課題提出用システムに対応したprintf
	 * printf("%s is a word of Sigma\n", Str);
	 * printf("%s is NOT a word of Sigma\n", Str);
	 */

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


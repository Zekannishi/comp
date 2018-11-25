#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* 定義 */
#define TOKEN_SIZE 128 // トークンの長さの最大値 
#define STR(var) #var

/* 文字・トークンの種類 */
/* step1, step3 */

/* トークン */
/* step4 */

/* グローバル変数 */
Kind charKind[256]; /* 文字種表 */
FILE *fp;

/* プロトタイプ宣言 */
void initializeCharKind(void);
int nextChar(void);
Token nextToken(void);
void writeTokenStr(char **p, char c); 
void printToken(Token *t);


int main(int argc, char *argv[]) {
	Token token;

	/* 文字種表charKindの初期化 */
//	printf("initializing charKind[]... "); /* 動作確認用．提出時には表示しないこと */
	initializeCharKind();
//	printf("done.\n"); /* 動作確認用．提出時には表示しないこと */

	/* open file argv[1] */
	if (argc < 2) {
		printf("no file name\n");
		exit(EXIT_FAILURE);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("error: file \"%s\" can not open\n", argv[1]);
		exit(EXIT_FAILURE);
	} else {
//		printf("file \"%s\" is opened.\n", argv[1]);
	}

	/* nextToken関数によるトークンの取得と表示 */
	token = nextToken();
	while (token.kind != EOFToken) {
		printToken(&token);
		token = nextToken();
	}

	/* close file argv[1] */
	fclose(fp);
//	printf("file \"%s\" is closed.\n", argv[1]); /* 動作確認用．提出時には表示しないこと */

	exit(EXIT_SUCCESS);
}

/*
 * 文字種表charKindの初期化
 */
void initializeCharKind(void) {
	/* step2 */
}

/*
 * ファイルから次の1文字を取得する
 */
int nextChar(void) {
	static int ch;

	if (ch == EOF) { // 最後に読んだ文字がEOFならば，EOFを返す
		return ch;
	} 
	ch = fgetc(fp);
	
	return ch;
}

/*
 * ファイルから記号を1文字ずつ読み，トークンを取得する
 */
Token nextToken(void) {
	static int ch = ' ';
	Token token = {NULLToken, "", 0};
	char *pStr = token.str;
	int val = 0;

	/* step5 */

	return token;
}

void writeTokenStr(char **p, char c) {
	**p = c;
	(*p)++;
}

/* Tokenの表示 */
void printToken(Token *t) {
	printf("%s, val=%d, kind=", t->str, t->val);
	switch (t->kind) {
		case LParen: printf("%s", STR(LParen)); break;
		case RParen: printf("%s", STR(RParen)); break;
		case Plus: printf("%s", STR(Plus)); break;
		case Minus: printf("%s", STR(Minus)); break;
		case Multi: printf("%s", STR(Multi)); break;
		case Div: printf("%s", STR(Div)); break;
		case IntNum: printf("%s", STR(IntNum)); break;
		case Symbol: printf("%s", STR(Symbol)); break;
		case EOFToken: printf("%s", STR(EOFToken)); break;
		case NULLToken: printf("%s", STR(NULLToken)); break;
		case Other: printf("%s", STR(Other)); break;
		default: printf("not implemented"); break;
	}
	printf("\n");
}


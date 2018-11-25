/*
 * 逆ポーランド記法による式の評価
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/* 定義 */
#define TOKEN_SIZE 128 // トークンの長さの最大値
#define STACK_SIZE 24 // スタックのサイズの最大値 
#define LINE_SIZE 128 // 1行に含まれるトークン数の最大値 
#define STR(var) #var
#define VERBOSE

/** 文字・トークンの種類 **/
typedef enum {
	LParen = '(', RParen =')', // '(', ')'
	Plus = '+', Minus = '-', Multi = '*', Div = '/', // '+', '-', '*', '/'
	Digit, // 0-9
	Letter, // _, a - z, A - Z
	IntNum, // integer
	Symbol, // symbol
	Variable, // variable (a sequence of Letter)
	EOFToken, NULLToken, // EOF, NULL
	Other // 上記のいずれでもない
} Kind;

/** トークン **/
typedef struct {
	Kind kind;                // トークンの種類
	char str[TOKEN_SIZE + 1]; // トークンの文字列
	int val;                  // トークンが定数のとき，その値
} Token;

/* グローバル変数 */
FILE *fp;
Kind charKind[256]; /* 文字種表 */
Token stack[STACK_SIZE]; // スタック
int stack_num = 0; // スタック内のデータ数
Token Tin[LINE_SIZE]; // 読み込んだトークンの列
Token RPN_out[LINE_SIZE]; // 逆ポーランド記法の出力先

/* プロトタイプ宣言 */
void initializeCharKind(void);
int nextChar(void);
Token nextToken(void);
void writeTokenStr(char **p, char c); 
/** プロトタイプ宣言: スタック **/
void push(Token t); // スタック: push 
Token pop(void); // スタック: pop
void printStack(void); // スタック: スタックの内容を表示 
/** プロトタイプ宣言: 逆ポーランド記法 **/
void rpn(Token *in, Token *out); // RPN: Reverse Polish Notation
int getOrder(Token *t); // 逆ポーランド記法: 演算子の優先順位
int evaluate(Token *array); // 逆ポーランド記法の式を評価する
/** プロトタイプ宣言: データ表示関数 **/
void printTokenArray(char *msg, Token *array);
void printToken(Token *t);


/********1*********2*********3*********4*********5*********6*********7*/

int main(int argc, char *argv[]) {
	Token token;
	Token *pTin = Tin;
	Token nullToken = {NULLToken, "", 0};


	/* 文字種表charKindの初期化 */
#ifdef VERBOSE
	printf("initializing charKind[]... ");
#endif
	initializeCharKind();
#ifdef VERBOSE
	printf("done.\n");
#endif

	/* open file argv[1] */
	if (argc < 2) {
		printf("no file name\n");
		exit(EXIT_FAILURE);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("error: file \"%s\" can not open\n", argv[1]);
		exit(EXIT_FAILURE);
	} else {
#ifdef VERBOSE
		printf("file \"%s\" is opened.\n", argv[1]);
#endif
	}

	/* nextToken関数によるトークンの取得とTinへの格納 */
	token = nextToken();
	while (token.kind != EOFToken) {
		*pTin = token;
		pTin++;
		token = nextToken();
	}
	*pTin = nullToken;
	printTokenArray("in: ", Tin);

	/* Tinを逆ポーランド記法に変換 */
	rpn(Tin,RPN_out); 
	printTokenArray("out: ", RPN_out);

	/* 逆ポーランド記法の式の値を評価 */
	int result;
	result = evaluate(RPN_out);
	printf("calculation result = %d\n", result);

	/* close file argv[1] */
	fclose(fp);
#ifdef VERBOSE
	printf("file \"%s\" is closed.\n", argv[1]);
#endif

	exit(EXIT_SUCCESS);
}

/*
 * 文字種表charKindの初期化
 */
void initializeCharKind(void) {
	int i;

	/* すべての要素をOtherとして初期化 */
	for (i = 0; i < 256; i++) {
		charKind[i] = Other;
	}

	/* '0'-'9'の文字をDigitとする */
	for (i = '0'; i <= '9'; i++) {
		charKind[i] = Digit;
	}

	/* _, a - z, A - Z */
	charKind['_'] = Letter;
	for (i = 'a'; i <= 'z'; i++) {
		charKind[i] = Letter;
	}
	for (i = 'A'; i <= 'Z'; i++) {
		charKind[i] = Letter;
	}

	/* 個々の文字の割当て */
	charKind['('] = LParen;
	charKind[')'] = RParen;
	charKind['+'] = Plus;
	charKind['-'] = Minus;
	charKind['*'] = Multi;
	charKind['/'] = Div;
}

/*
 * ファイルから次の1文字を取得する
 */
int nextChar(void) {
	static int ch;

	if (ch == EOF) { /* 最後に読んだ文字がEOFならば，EOFを返す */
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

	/* 空白の読み飛ばし */
	while (isspace(ch)) {
		ch = nextChar();
	}

	/* 最後の読んだ文字がEOFの場合，EOFを表わすトークンを返す */
	if (ch == EOF) {
		token.kind = EOFToken;
		return token;
	}

	/* トークンの切り出し */
	switch (charKind[ch]) {
		case Digit: /* 数字 */
			while (charKind[ch] == Digit) {
				writeTokenStr(&pStr, (char)ch);
				val = val * 10 + (ch - '0');
				ch = nextChar();
			}
			token.kind = IntNum;
			writeTokenStr(&pStr, '\0');
			token.val = val;
			break;

		case Letter:
			while (charKind[ch] == Letter) {
				writeTokenStr(&pStr, (char)ch);
				ch = nextChar();
			}
			token.kind = Variable;
			writeTokenStr(&pStr, '\0');
			break;

		default: /* 記号 */
			switch (ch) {
				case LParen:
				case RParen:
				case Plus:
				case Minus:
				case Multi:
				case Div:
					token.kind = charKind[ch];
					break;
				default:
					token.kind = Symbol;
					break;
			}
			writeTokenStr(&pStr, (char)ch);
			writeTokenStr(&pStr, '\0');
			ch = nextChar();
			break;
	}

	return token;
}

void writeTokenStr(char **p, char c) {
	**p = c;
	(*p)++;
}

/* スタック: push */
void push(Token t) {
	if (stack_num < STACK_SIZE) {
		stack[stack_num] = t;
		stack_num++;
	} else {
		printf("stack overflow\n");
		exit(EXIT_SUCCESS);
	}
}

/* スタック: pop */
Token pop(void) {
	Token t;

	/* step 1 */

	return t;
}

/* スタック: スタックの内容を表示 */
void printStack(void) {
	int i;
	printf("stack: ");
	for (i = 0; i < stack_num; i++) {
		printf("%s ", stack[i].str);
	} 
	printf("\n");
}

/* 逆ポーランド記法 */
void rpn(Token *in, Token *out) {
	int i;
	Token t, tmp;
	Token nullToken = {NULLToken, "", 0};

#ifdef VERBOSE
	printf("begin: Reverse Polish Notation\n");
#endif

	i = 0;
	t = *in;
	while (t.kind != NULLToken) {
#ifdef VERBOSE
		printf("-> %s:\n", t.str);
#endif

		/* step 2-2 */

#ifdef VERBOSE
		/* 注意: 次の3行はRPN_outを表示するためのサンプル */
		/* 用いる場合，step 2-2 の適切な位置に記述する必要があります */
		printStack(); 
		*out = nullToken; // 次の行の経過表示用に最後にNULLTokenを書き込む
		printTokenArray("out: ", RPN_out);
#endif

	}

	/* 残りを出力 */
	while (stack_num > 0) {
		tmp = pop();
		if (tmp.kind == LParen) {
			printf("error: much '('\n");
			exit(EXIT_SUCCESS);
		}
		*out = tmp;
		out++;
	}

	/* 最後にNULLTokenを書き込む */
	*out = nullToken;
}

/* 逆ポーランド記法: 演算子の優先順位 */
int getOrder(Token *t) {
	int order = -1;

	switch (t->kind) {
		case Multi:
		case Div:
			order = 3;
			break;
		case Plus:
		case Minus:

			/* step 2-1 */

			break;
		case LParen:
			order = 1;
		default:
			order = -1;
			break;
	}

	return order;
}

/* 逆ポーランド記法の式を評価する */
int evaluate(Token *array) {
	Token token;
	Token t1, t2, tmp = {NULLToken, "", 0};
	int i;

	stack_num = 0;
	i = 0;
	token = *array;
	while (token.kind != NULLToken) {
		switch (token.kind) {
			case IntNum:
				push(token);
				break;
			case Plus:
			case Minus:
			case Multi:
			case Div:

				/* step 3 */

				break;
			default:
				printf("error\n");
				break;
		}
		i++;
		token = *(array + i);
	}
	tmp = pop();

	return tmp.val;
}

/* Token型配列の表示 */
void printTokenArray(char *msg, Token *array) {
	int i;
	Token t;

	printf("%s", msg);
	i = 0;
	t = *array;
	while (t.kind != NULLToken) {
		printf("%s ", (array+i)->str);
		i++;
		t = *(array + i);
	}
	printf("\n");
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
		case Variable: printf("%s", STR(Variable)); break;
		case EOFToken: printf("%s", STR(EOFToken)); break;
		case NULLToken: printf("%s", STR(NULLToken)); break;
		case Other: printf("%s", STR(Other)); break;
		default: printf("not implemented"); break;
	}
	printf("\n");
}


#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 255

/* グローバル変数 */
char Sigma[] = {'a', 'b'}; /* アルファベットの有限集合 */
char Str[MAX_SIZE + 1]; /* 文字列格納用の配列 */

/* プロトタイプ宣言 */
void getWord(void);
char nextChar(void);

int main(void) {
  /* ローカル変数の宣言 */
  char ch;

  /* 記号列の読み込み */
  getWord();
  printf("word: %s\n", Str);

  while ((ch = nextChar()) != '\0') {
    printf("%c ", ch); /* Step2の動作確認用 */
  }
  printf("\n"); /* Step2の動作確認用 */
  
  exit(EXIT_SUCCESS);
}

void getWord(void) {
  /* Step1をここに実装する */
  printf("Input word: ");
  scanf("%s", Str); 
}

char nextChar(void) {
  static char *pStr = Str;
  char c;

  /* Step2をここに実装する */
  c = *pStr;
  pStr++;
  return c;
}


#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 255

/* グローバル変数 */
char Sigma[] = {'a', 'b'}; /* アルファベットの有限集合 */
char Str[MAX_SIZE + 1]; /* 文字列格納用の配列 */

/* プロトタイプ宣言 */
void getWord(void);

int main(void) {
  
  /* 記号列の読み込み */
  getWord();
  printf("word: %s\n", Str);
  
  exit(EXIT_SUCCESS);
}

void getWord(void) {
  /* Step1をここに実装する */
  printf("Input word: ");
  scanf("%s", Str); 
}


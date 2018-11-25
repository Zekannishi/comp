#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>

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


  while ((ch = nextChar()) != '\0') {
    if (isAlphabet(ch)) {
      isWord = true;
    } else {
      isWord = false;
      break;
    }
  }

  if(isWord)
    printf("%s is a word of Sigma\n", Str);
  else
    printf("%s is NOT a word of Sigma\n", Str);
  
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

bool isAlphabet(char c) {
  char *p = Sigma;

  /* Step3をここに実装する */
  while(*p != '\0'){
    if(c == *p)
      return true;

    p++;
  }

  return false;
}

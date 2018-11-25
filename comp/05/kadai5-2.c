#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define STR(var) #var
#define TOKEN_SIZE 128

typedef enum{
  Digit,
  Letter,
  LParen,
  RParen,
  Plus,
  Minus,
  Multi,
  Div,
  Other,
  IntNum,
  Symbol,
  Variable,
  EOFToken,
  NULLToken
}Kind;

typedef struct{
  Kind kind;
  char str[TOKEN_SIZE + 1];
  int val;
}Token;

/*グローバル変数*/
Kind charKind[256];
FILE *fp;

void initializeCharKind(void);
int nextChar(void);
Token nextToken(void);
void writeTokenStr(char **p, char c); 
void printToken(Token *t);

int main(int argc, char *argv[]){
  Token token = {NULLToken, "", 0};

  initializeCharKind();

  if(argc < 2){
    printf("ni file name\n");
    exit(EXIT_FAILURE);
  }

  if((fp = fopen(argv[1], "r")) == NULL){
    printf("file not find : %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  token = nextToken();
  while(token.kind != EOFToken){
    printToken(&token);
    token = nextToken();
  }

  fclose(fp);
  return 0;
}

void initializeCharKind(void){
  int i;
  //Otherで初期化
  for(i=0;i<256;i++){
    charKind[i] = Other;
  }

  //Digit
  for(i='0';i<='9';i++){
    charKind[i] = Digit;
  }

  //Letter
  for(i='a';i<='z';i++){
    charKind[i] = Letter;
  }

  for(i='A';i<='Z';i++){
    charKind[i] = Letter;
  }

  charKind['_'] = Letter;;

  //Paren
  charKind['('] = LParen;
  charKind[')'] = RParen;
  
  // +,-,*,/
  charKind['+'] = Plus;
  charKind['-'] = Minus;
  charKind['*'] = Multi;
  charKind['/'] = Div;
}

int nextChar(void) {
  static int ch;

  if (ch == EOF) { // 最後に読んだ文字がEOFならば，EOFを返す
    return ch;
  } 
  ch = fgetc(fp);
  
  return ch;
}

Token nextToken(void) {
  static int ch = ' ';
  Token token = {NULLToken, "", 0};
  char *pStr = token.str;
  int val = 0;

  /* 空白の読み飛ばし */
  while (isspace(ch)) {
    ch = nextChar();
  }

  /*EOF*/
  if(ch == EOF){
    token.kind = EOFToken;
    token.str[0] = ch;
    token.val = 0;
    return token;
  }

  switch(charKind[ch]){
      /*数字*/
    case Digit:
      while(charKind[ch] == Digit){
	writeTokenStr(&pStr,(char)ch);
	val = val*10 + (ch - '0');
	ch = nextChar();
      }
      token.kind = IntNum;
      token.val = val;
      break;

      /*文字系*/
    case Letter:
      while(charKind[ch] == Letter){
        writeTokenStr(&pStr,(char)ch);
        ch = nextChar();
      }
      token.kind = Variable;
      token.val = 0;
      break;

      /*記号系*/
    case LParen:
      writeTokenStr(&pStr,(char)ch);
      token.kind = LParen;
      token.val = 0;
      ch = nextChar();
      break;

    case RParen:
      writeTokenStr(&pStr,(char)ch);
      token.kind = RParen;
      token.val = 0;
      ch = nextChar();
      break;

    case Plus:
      writeTokenStr(&pStr,(char)ch);
      token.kind = Plus;
      token.val = 0;
      ch = nextChar();
      break;

    case Minus:
      writeTokenStr(&pStr,(char)ch);
      token.kind = Minus;
      token.val = 0;
      ch = nextChar();
      break;

    case Multi:
      writeTokenStr(&pStr,(char)ch);
      token.kind = Multi;
      token.val = 0;
      ch = nextChar();
      break;

    case Div:
      writeTokenStr(&pStr,(char)ch);
      token.kind = Div;
      token.val = 0;
      ch = nextChar();
      break;

    case Symbol:
      writeTokenStr(&pStr,(char)ch);
      token.kind = Symbol;
      token.val = 0;
      ch = nextChar();
      break;

    case Other:
      writeTokenStr(&pStr,(char)ch);
      token.kind = Other;
      token.val = 0;
      ch = nextChar();
      break;

    default:
      break;
    }
  
  writeTokenStr(&pStr,'\0');

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
  case Variable: printf("%s", STR(Variable)); break;
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

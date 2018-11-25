#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<stdbool.h>

/*デバッグ*/
//#define DEBUG

#define STR(var) #var
#define TOKEN_SIZE 128 //トークンの長さの最大値
#define STACK_SIZE 24 //スタックのサイズ
#define LINE_SIZE 128 //一行のトークンの最大数

/* トークンの種類 */
typedef enum{
  LParen = '(', RParen = ')', // '(', ')' 
  Plus = '+', Minus = '-', Multi = '*', Div = '/',
  Digit, //0-9
  Letter, // _, a-z, A-Z
  IntNum, //数値
  Symbol, //記号
  Variable, //文字列
  EOFToken, //EOF
  NULLToken, //空
  Other //上記以外
}Kind;

/* トークン解析 */
typedef struct{
  Kind kind; //トークンの種類
  char str[TOKEN_SIZE + 1]; //トークン文字列
  int val; //数値トークンの値
}Token;

/* グローバル変数 */
FILE *fp;
Kind charKind[256]; //文字種表
Token stack[STACK_SIZE]; //スタック
int stack_num = 0; //スタックデータ数
Token Tin[LINE_SIZE]; //読み込んだトークン列
Token RPN_out[LINE_SIZE]; //逆ポーランド記法の出力先

/* 関数 */
void initializeCharKind(void);
int nextChar(void);
Token nextToken(void);
void writeTokenStr(char **p, char c); 
/* スタック系 */
void push(Token t); //push
Token pop(void); //pop
void printStack(void); //スタック内容の表示
/* 逆ポーランド系 */
void rpn(Token *in, Token *out); //逆ポーランド
int getOrder(Token *t); //記号の優先順位取得
int evaluate(Token *array); //式の評価
/* データ表示系 */
void printTokenArray(char *msg, Token *array);
void printToken(Token *t);



int main(int argc, char *argv[]){
  Token token;
  Token *pTin = Tin;
  Token nullToken = {NULLToken, "", 0};

  /*文字種表生成*/
  initializeCharKind();

  if(argc < 2){
    printf("no file name\n");
    exit(EXIT_FAILURE);
  }

  if((fp = fopen(argv[1], "r")) == NULL){
    printf("file not find : %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  /*  トークン読み込み */
  token = nextToken();
  while(token.kind != EOFToken){
    *pTin = token;
    pTin++;
    token = nextToken();
  }
  *pTin = nullToken;
  printTokenArray("in: ", Tin);

  /* Tinを逆ポーランドに変換 */
  rpn(Tin, RPN_out);
  printTokenArray("out: ", RPN_out);
#ifdef DEBUG
  printf("test point\n");
#endif
  /* 逆ポーランドの式の評価 */
  int result;
  result = evaluate(RPN_out);
  printf("calculation result = %d\n", result);
#ifdef DEBUG 
  printf("test point\n");
#endif
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
    case RParen:
    case Plus:
    case Minus:
    case Multi:
    case Div:
    case Symbol:
    case Other:
      writeTokenStr(&pStr,(char)ch);
      token.kind = charKind[ch];
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

/* push */
void push(Token t) {
  if (stack_num < STACK_SIZE) {
    stack[stack_num] = t;
    stack_num++;
  } else {
    printf("stack overflow\n");
    exit(EXIT_SUCCESS);
  }
}

/* pop */
Token pop(void) {
  Token t;

  //スタックが空
  if(stack_num > 0){
    stack_num--;
    t = stack[stack_num];
  }else{
    printf("stack underflow\n");
    exit(EXIT_SUCCESS);
  }
  return t;
}

/* スタックの内容を表示 */
void printStack(void) {
  int i;
  printf("stack: ");
  for (i = 0; i < stack_num; i++) {
    printf("%s ", stack[i].str);
  } 
  printf("\n");
}

/* 逆ポーランド */
void rpn(Token *in, Token *out){
  int i, type;
  Token t, tmp;
  Token nullToken = {NULLToken, "", 0};

  i = 0;
  t = *in;

  while(t.kind != NULLToken){
#ifdef DEBUG
    printf("-> %s: %d\n", t.str, t.kind);
#endif
    switch(t.kind){
     
     case IntNum:
       *out = t;
       out++;
       break;

     case Plus:
     case Minus:
     case Multi:
     case Div:
       type = getOrder(&t);
       while(type <= getOrder( &stack[stack_num-1] ) ){
	 *out = pop();
	 out++;
       }
       push(t);
       break;
       
     case LParen:
       push(t);
       break;
       
     case RParen:
       tmp = pop();
       while(tmp.kind != LParen){
	 *out = tmp;
	 out++;
	 if(stack_num == 0){                    
           printf("error: less '('\n");
           exit(EXIT_SUCCESS);
         }
	 tmp = pop();
       }
       break;

     default:
       printf("error: rpn\n");
       break;
    }
#ifdef DEBUG
    printStack(); 
    *out = nullToken; // 次の行の経過表示用に最後にNULLTokenを書き込む
    printTokenArray("out: ", RPN_out);
#endif
    i++;
    t = *(in + i);

  }

  /*残りを出力*/
  while(stack_num > 0){
    tmp = pop();
    if(tmp.kind == LParen){
      printf("error: much '('\n");
      exit(EXIT_SUCCESS);
    }
    *out = tmp;
    out++;
  }

  *out = nullToken;
}

/* 演算子の優先順位 */
int getOrder(Token *t) {
  int order = -1;

  switch (t->kind) {
  case Multi:
  case Div:
    order = 3;
    break;

  case Plus:
  case Minus:
    order = 2;
    break;

  case LParen:
    order = 1;
  default:
    order = -1;
    break;
  }

  return order;
}

/* 逆ポーランドの式の評価 */
int evaluate(Token *array) {
  Token token;
  Token t1, t2, t3, tmp = {NULLToken, "", 0};
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
      t2 = pop();
      t1 = pop();
      switch(token.kind){
       case Plus:
	 t3.val = t1.val + t2.val;
	 push(t3);
	 break;

       case Minus:
	 t3.val = t1.val - t2.val; 
         push(t3);
	 break;

       case Multi:
	 t3.val = t1.val * t2.val; 
         push(t3);
	break;

       case Div:
	 t3.val = t1.val / t2.val; 
         push(t3);
	break;

       default:
	 printf("error: evaluate\n");
	 break;
      }
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

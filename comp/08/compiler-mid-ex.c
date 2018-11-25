#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/*デバッグ*/
//#define DEBUG

/*定数*/
#define STR(var) #var
#define TOKEN_SIZE 128 //トークンの長さの最大値
#define STACK_SIZE 24 //スタックのサイズ
#define LINE_SIZE 128 //一行のトークンの最大数
#define TABLE_SIZE 64 //記号表に格納できるトークン数

/* トークンの種類 */
typedef enum{
  Cr = '\n', //改行 
  LParen = '(', RParen = ')', // '(', ')' 
  Plus = '+', Minus = '-', Mult = '*', Div = '/', Sur = '%',
  Assign = '=', Question = '?',
  Digit, //0-9
  Letter, // _, a-z, A-Z
  IntNum, //数値
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

/* 記号表 */
typedef struct {
  Token table[TABLE_SIZE];
  Token *tail;
} SymbolTable;

/* グローバル変数 */
FILE *fp;
Kind charKind[256]; //文字種表
Token stack[STACK_SIZE]; //スタック
int stack_num = 0; //スタックデータ数
Token token; /* 次に処理するトークン */
SymbolTable STable; /* 記号表(変数表) */
#ifdef DEBUG
int _depth = 0;
#endif

/* プロトタイプ宣言 */
/* 構文解析 */
void program(void); /* <program> */
void statement(void); /* <statement> */
void expression(void); /* <expression> */
void term(void); /* <term> */
void factor(void); /* <factor> */
void evaluate(Kind kind); /* スタックの演算 */
void copyToken(Token *to, Token *from); /* トークンのコピー */
bool checkToken(Token *t, Kind kind); /* トークンの種類の確認 */

/* 記号表 */
void initializeTable(SymbolTable *st); /* 記号表の初期化 */
void addTable(SymbolTable *st, Token t); /* 記号表にトークンを追加する */
void printTable(SymbolTable *st);
Token searchTable(SymbolTable *st, char *name); /* 記号表の要素を取得する */
bool replaceElementOfTable(SymbolTable *st, Token t); /* 記号表の要素を入れ替える */
bool containTable(SymbolTable *st, char *name); /* 記号表の要素の確認 */

/* 字句解析 */
void initializeCharKind(void);
int nextChar(void);
Token nextToken(void);
void writeTokenStr(char **p, char c); 

/* スタック */
void push(Token t); /* スタック: push */ 
Token pop(void); /* スタック: pop */
void initializeStack(void); /* スタック: スタックを初期化 */
void printStack(void); /* スタック: スタックの内容を表示  */

/* データ表示関数 **/
void printToken(Token *t);
#ifdef DEBUG
void _printIndent();
#endif

int main(int argc, char *argv[]){
  //文字種表作成
  initializeCharKind();
  //変数表作成
  initializeTable(&STable);
  
  /* ファイルオープン */
  if (argc < 2) {
    printf("no file name\n");
    exit(EXIT_FAILURE);
  }
  if ((fp = fopen(argv[1], "r")) == NULL) {
    printf("error: file \"%s\" can not open\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  /* 構文解析 */
  do{
    token = nextToken();
    program();  
  }while(!checkToken(&token, EOFToken));

#ifdef DEBUG
  /* スタックの内容を表示 */
  printf("stack:\n");
  printStack();

  /* 記号表を表示 */
  printf("symbol table:\n");
  printTable(&STable);
#endif 
  /* ファイルクローズ */
  fclose(fp);

  return 0;
}

/************/
/* 構文解析 */
/************/
//program
void program(void){
  #ifdef DEBUG
  _printIndent(); printf("-> %s, ", __func__); printToken(&token);
  _depth++;
#endif
  statement();

  while(token.kind != EOFToken){ /*改行のとき*/
    statement();
  }
#ifdef DEBUG
  _depth--;
  _printIndent(); printf("<- %s\n", __func__);
#endif
}

//statement
void statement(void){
  Token var, tmp;
#ifdef DEBUG
  _printIndent(); printf("-> %s, ", __func__); printToken(&token);
  _depth++;
#endif

  switch(token.kind){
    case Variable:
      /*変数保存*/
      copyToken(&tmp, &token);
      token = nextToken();
      /*次は必ず'='*/
      if(!checkToken(&token, Assign)){
	printf("error: '=' is expected.\n");
	exit(EXIT_SUCCESS);
      }
      token = nextToken();
      expression();
      /*式の評価が終了*/
      var = pop();
      strcpy(var.str, tmp.str);
      if(containTable(&STable, var.str) == true){
	replaceElementOfTable(&STable, var);
      }else{
	addTable(&STable, var);
      }
      break;

      
    case Question:
      token = nextToken();
      if(token.kind == EOFToken){
	printf("error: <expression> is expected\n");
	exit(EXIT_SUCCESS);
      }
      expression();
      var = pop();
      printf("result: %d\n", var.val);
      break;

    default:
      printf("error: %s\n", token.str);
      exit(EXIT_SUCCESS);
      break;
  }

#ifdef DEBUG
  _depth--;
  _printIndent(); printf("<- %s\n", __func__);
#endif
}

//expression
void expression(void) {
  Kind op;
#ifdef DEBUG
  _printIndent(); printf("-> %s, ", __func__); printToken(&token);
  _depth++;
#endif

  term();
  switch(token.kind){
    case Plus:
    case Minus:
      op = token.kind;
      token = nextToken();
      term();
      evaluate(op);
      break;

    default: /*ここではなにもしない*/
       break;
  }

#ifdef DEBUG
  _depth--;
  _printIndent(); printf("<- %s\n", __func__);
#endif
}

//term
void term(void){
  Kind op;
#ifdef DEBUG
  _printIndent(); printf("-> %s, ", __func__); printToken(&token);
  _depth++;
#endif
  
  factor();
  switch(token.kind){
    case Mult:
    case Div:
    case Sur:
      op = token.kind;
      token = nextToken();
      factor();
      evaluate(op);
      break;

  default: /*何もせず終了*/
    break;
  }
  
#ifdef DEBUG
  _depth--;
  _printIndent(); printf("<- %s\n", __func__);
#endif
}

//factor
void factor(void) {
  static bool Lp = false; /*左括弧の処理中かどうか*/
  Token tmp;
  Token NullToken = {NULLToken, "", 0};
#ifdef DEBUG
  _printIndent(); printf("-> %s, ", __func__); printToken(&token);
  _depth++;
#endif

  switch(token.kind){
    case IntNum:
      push(token);
      break;

    case Variable:
      tmp = searchTable(&STable, token.str);
      if(tmp.kind == NullToken.kind){
	printf("error: \"%s\" is not in the symbol table\n", token.str);
	exit(EXIT_SUCCESS);
      }else{
	token = tmp;
	push(token);
      }
      break;

    case LParen:
      Lp = true;
      token = nextToken();
      expression();
      Lp = false;
      if(token.kind != RParen){
	printf("error: ')' is expected\n");
	exit(EXIT_SUCCESS);
      }
      break;

    case RParen:
      if(Lp != true){
	printf("error: ')'");
        exit(EXIT_SUCCESS);
      }	
      break;

    case EOFToken: //EOF,改行は解析終了
    case Cr:
      break;

    case Other:
      printf("error: %s\n", token.str);
      break;
      
    default:
      break;
  }

  token = nextToken();

#ifdef DEBUG
  _depth--;
  _printIndent(); printf("<- %s\n", __func__);
#endif
}

void evaluate(Kind kind) {
  Token t1, t2, tmp = {NULLToken, "", 0};

  t2 = pop();
  t1 = pop();
  switch (kind) {
    case Plus:
      tmp.val = t1.val + t2.val;
      snprintf(tmp.str,  TOKEN_SIZE, "%d", tmp.val);
      tmp.kind = IntNum;
      break;

    case Minus:
      tmp.val = t1.val - t2.val;
      snprintf(tmp.str,  TOKEN_SIZE, "%d", tmp.val);
      tmp.kind = IntNum;
      break;
      
    case Mult:
      tmp.val = t1.val * t2.val;
      snprintf(tmp.str,  TOKEN_SIZE, "%d", tmp.val);
      tmp.kind = IntNum;
      break;

    case Div:
      if(t2.val == 0){
	printf("error: division by zero\n");
	exit(EXIT_SUCCESS);
      }
      tmp.val = t1.val / t2.val;
      snprintf(tmp.str,  TOKEN_SIZE, "%d", tmp.val);
      tmp.kind = IntNum;
      break;

    case Sur:
      if(t2.val == 0){
	printf("error: division by zero\n");
	exit(EXIT_SUCCESS);
      }
      tmp.val = t1.val % t2.val;
      snprintf(tmp.str,  TOKEN_SIZE, "%d", tmp.val);
      tmp.kind = IntNum;
      break;
      
    default:
      printf("error: eva\n");
      exit(EXIT_SUCCESS);
      break;
  }
  push(tmp);
}

void copyToken(Token *to, Token *from) {
  to->kind = from->kind;
  strcpy(to->str, from->str);
  to->val = from->val;
}

bool checkToken(Token *t, Kind kind) {
  return (t->kind == kind); 
}

/******************/
/* 記号表(変数表) */
/******************/
void initializeTable(SymbolTable *st) {
  st->tail = st->table;
}

void addTable(SymbolTable *st, Token t) {
  *(st->tail) = t;
  (st->tail)++;
}

void printTable(SymbolTable *st) {
  Token *it = st->table;
  while (it != st->tail) {
    printToken(it);
    it++;
  }
}

bool containTable(SymbolTable *st, char *name) {
  Token *it = st->table;
  while(it != st->tail){
    if(strcmp(it->str, name) == 0)
      return true;

    it++;
  }
  return false;
}

bool replaceElementOfTable(SymbolTable *st, Token t) {
  bool replaced = false;
  Token *it = st->table;

  while(it != st->tail){
    if(strcmp(it->str, t.str) == 0){
      *it = t;
      replaced = true;
    }
    it++;
  }

  return replaced;
}

Token searchTable(SymbolTable *st, char *name) {
  Token *t = st->table;
  Token NullToken = {NULLToken, "", 0}; 
  
  while(t != st->tail){
    if(strcmp(t->str, name) == 0)
      return *t;

    t++;
  }
  *t = NullToken;
  return *t;
}

/************/
/* 文字解析 */
/************/
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

  // +,-,*,/,(,),=,?
  charKind['+'] = Plus;
  charKind['-'] = Minus;
  charKind['*'] = Mult;
  charKind['/'] = Div;
  charKind['%'] = Sur;
  charKind['='] = Assign;
  charKind['?'] = Question;
  charKind['('] = LParen;
  charKind[')'] = RParen;
  charKind['\n'] = Cr;
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

  /*改行*/
  if(ch == '\n'){
    token.kind = Cr;
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
  case Plus:
  case Minus:
  case Mult:
  case Div:
  case Sur: 
  case Assign:
  case Question:
  case LParen:
  case RParen:
    writeTokenStr(&pStr,(char)ch);
    token.kind = charKind[ch];
    token.val = 0;
    ch = nextChar();
    break;

  default:
    writeTokenStr(&pStr,(char)ch);
    token.kind = Other;
    token.val = 0;
    ch = nextChar();
    break;
  }
  
  writeTokenStr(&pStr,'\0');

  return token;
}

void writeTokenStr(char **p, char c) {
  **p = c;
  (*p)++;
}

/************/
/* スタック */
/************/
/* push */
void push(Token t) {
  if (stack_num < STACK_SIZE) {
    stack[stack_num] = t;
    stack_num++;
  } else {
    printf("error: stack overflow\n");
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
    printf("error: stack underflow\n");
    exit(EXIT_SUCCESS);
  }
  return t;
}

/* スタックを初期化 */
void initializeStack(void) {
  stack_num = 0;
}

/* スタックの内容を表示 */
void printStack(void) {
  int i;

  if(!stack_num){ /*スタックが空*/
    printf("(empty)");
  }else{ /*中身があるとき*/
    printf("stack: ");
    for (i = 0; i < stack_num; i++) {
      printf("%s ", stack[i].str);
    } 
  }
  printf("\n");
}

/******************/
/* データ表示関数 */
/******************/

/* Tokenの表示 */
void printToken(Token *t){
  printf("%s, val=%d, kind=", t->str, t->val);
  switch (t->kind) {
  case Plus: printf("%s", STR(Plus)); break;
  case Minus: printf("%s", STR(Minus)); break;
  case Mult: printf("%s", STR(Mult)); break;
  case Div: printf("%s", STR(Div)); break;
  case Sur: printf("%s", STR(Sur)); break;
  case LParen: printf("%s", STR(LParen)); break;
  case RParen: printf("%s", STR(RParen)); break;
  case Cr: printf("%s", STR(Cr)); break;
  case Assign: printf("%s", STR(Assign)); break;
  case Question: printf("%s", STR(Question)); break;
  case IntNum: printf("%s", STR(IntNum)); break;
  case Variable: printf("%s", STR(Variable)); break;
  case EOFToken: printf("%s", STR(EOFToken)); break;
  case NULLToken: printf("%s", STR(NULLToken)); break;
  case Other: printf("%s", STR(Other)); break;
  default: printf("not implemented"); break;
  }
  printf("\n");
}

#ifdef DEBUG
void _printIndent() {
  int i;
  for (i = 0; i < _depth; i++) {
    printf(" ");
  }
}
#endif

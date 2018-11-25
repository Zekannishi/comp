#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void exe(void);

int main(int argc, char *argv[]){
  printf("デモを実行\n");
  exe();
  printf("終了\n");
  return 0;
}

void exe(void){
  int i;
  FILE *fp = fopen("test_list.txt", "r");
  char filename[50] = "null";
  char tmp[100] = "compiler08-mid-executables/mid compiler08-mid-testcases/";
  char command[100];

  if(fp == NULL){
    printf("'test_list.txt' not found.\n");
    exit(1);
  }
  
  printf("実行ファイル名を入力(サンプルの時はsampを入力) : ");
  scanf("%s", command);
  
  if(strcmp(command, "samp")){
    strcpy(tmp, command);
    strcat(tmp, " compiler08-mid-testcases/");
  }
  
  while(1){
    i=0;
    while(1){
      filename[i] = fgetc(fp);
#ifdef DEBUG
      printf("filename[%d] -> %c\n", i, filename[i]);
      if(filename[i] == '\n') printf("改行\n");
#endif
      if(filename[i] == '\n') break;
      i++;
      }
    filename[i] = '\0';
    printf("insert -> %s\n", filename);
    if(!strcmp(filename, "end")) break;
    strcpy(command, tmp);
    strcat(command, filename);
    system(command);
    printf("output <- %s\n\n\n", filename);
  }
  fclose(fp);
}

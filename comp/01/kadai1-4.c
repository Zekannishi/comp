#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
  char str;
  FILE *fp = fopen(argv[1], "r"); 

  if(fp == NULL){
    printf("%s is not found.\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  while( (str = fgetc(fp)) != EOF){
    printf("%c", str);
  }

  fclose(fp);
  
  return 0;
}

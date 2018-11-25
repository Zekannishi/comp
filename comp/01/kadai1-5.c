#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

int main(int argc, char *argv[]){
  char str;
  FILE *fp = fopen(argv[1], "r"); 

  if(fp == NULL){
    printf("%s is not found.\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  while( (str = fgetc(fp)) != EOF){
    if(islower(str)){
      str = toupper(str);
    }
    printf("%c", str);
  }

  fclose(fp);

  return 0;
}

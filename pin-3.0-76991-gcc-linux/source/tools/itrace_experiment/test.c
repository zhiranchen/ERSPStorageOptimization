#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
void foo()
{
  int a = 22222;
  printf("Address of a int: %ld",&a);
  float b = 12.1;
  printf("Size int:%d", sizeof(a));

  printf("Address of b float: %ld",&b);
  double d = 21.22;

  printf("Size float:%d", sizeof(b));
  printf("Address of d double: %ld",&d);
  char e = 'a';
  printf("Size of double:%d",sizeof(d));

  printf("Size char:%d", sizeof(e));
  printf("Address of e char: %ld",&e);
//  a = 111111;
  int c=a;
  a= a+1;
  int k = a +a;
  //c=c+1

}

int main(int ac, char **av)
{

  FILE * myFile;
  //overwrite the stack trace output file
  myFile = fopen("ImmediateOutput.txt", "w");
  foo();
/*
  myFile = fopen("output.txt", "a");
  for(int j=0; j < 10; j++){
     fprintf(myFile, "%d\n", end = 301 + j);
  }
  fclose(myFile);*/

}

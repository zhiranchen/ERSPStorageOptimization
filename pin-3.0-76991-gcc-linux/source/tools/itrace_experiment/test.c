#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
void foo()
{
  int a = 22222;
  printf("Address of a int: %ld\n",(long int)&a);
  printf("Size int:%ld", sizeof(a));

  float b = 12.1;
  printf("Address of b float: %ld\n", (long int)&b);
  printf("Size float:%ld", sizeof(b));

  double d = 21.22;
  printf("Address of d double: %ld\n", (long int)&d);
  printf("Size of double:%ld",sizeof(d));

  char e = 'a';
  printf("Address of e char: %ld\n", (long int)&e);
  printf("Size char:%ld", sizeof(e));

  //  a = 111111;
  int c=a;
  a= a+1;
  int k = a + a;
  //c=c+1*/

}

int main(int ac, char **av)
{

  FILE * myFile;
  myFile = fopen("ImmediateOutput.txt", "w");
  
  foo();

}

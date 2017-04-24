#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void foo()
{
  int a = 222222;

  a = 111111;
  int c=a;
  a= a+1;
  //c=c+1

}

int main(int ac, char **av)
{

  FILE * myFile;

  myFile = fopen("output.txt", "a");
  int start;
  int end;
  for(int i = 0; i < 10; i++){
    fprintf(myFile, "%d\n", start = 201 + i);
  }
  fclose(myFile);

  myFile = fopen("output.txt", "w");
 
  foo();

  myFile = fopen("output.txt", "a");
  for(int j=0; j < 10; j++){
     fprintf(myFile, "%d\n", end = 301 + j);
  }
  fclose(myFile);
}

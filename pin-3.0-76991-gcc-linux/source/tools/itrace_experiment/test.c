
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void foo()
{
  int a=22222;
  int c=a;
  a= a+1;
  //c=c+1

}

int main(int ac, char **av)
{

  foo();
}

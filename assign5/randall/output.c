#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



bool writebytes1(unsigned long long x, int nbytes)
{
  do
      {
        if (putchar (x) < 0)
            return false;
        x >>= CHAR_BIT;
        nbytes--;
      }
    while (0 < nbytes);

    return true;
}

bool writebytes2(unsigned long long x, int nbytes)
{
      while(nbytes > 0){
        char* pt = (char*) malloc(sizeof(char));
        *pt = x;
        if(!write(1, pt, 1)) return false;
        free(pt);
        x >>= CHAR_BIT;
        nbytes--;
    }
  return true;
}



#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>



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

bool writebytes2(char* ptr, int nbytes)
{
  // if(nbytes < 0) return false;
  // printf("here is the output %s\n", ptr);
  return write(1, ptr, nbytes);
}



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
  unsigned long long* ptr = (unsigned long long* ) malloc(nbytes);
  *ptr = x;
  if(!write(1, ptr, nbytes)) return false;
  free(ptr);
  return true;
}



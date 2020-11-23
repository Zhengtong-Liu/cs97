#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;
struct drand48_data buf1;
struct drand48_data buf2;
/* Initialize the software rand64 implementation.  */
void
software_rand64_init (char* file)
{
  urandstream = fopen (file, "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

void mrand48_rng_init(void){
  srand48_r(time(NULL), &buf1);
  srand48_r(time(NULL), &buf2);

}

unsigned long long
mrand48_rng (void)
{
  long int a, b;
  mrand48_r(&buf1, &a);
  mrand48_r(&buf2, &b);
  unsigned long long x = (((unsigned long long) a) << 32) | 
  ((unsigned long long) b & 0x00000000FFFFFFFF);
  return x;
  // srand48(time(NULL));
  // return mrand48();
}

void mrand48_fini (void)
{
  
}

/* Finalize the software rand64 implementation.  */
void
software_rand64_fini (void)
{
  fclose (urandstream);
}

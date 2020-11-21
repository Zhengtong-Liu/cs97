#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;
struct drand48_data buf = {0};
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
  srand48_r(time(0), &buf);
}

unsigned long long
mrand48_rng (void)
{
  long int a, b;
  mrand48_r(&buf, &a);
  mrand48_r(&buf, &b);
  return (((unsigned long long) a) << 32) | ((unsigned long long) b & 0x00000000FFFFFFFF);
  // srand48(time(NULL));
  // return mrand48();
}

/* Finalize the software rand64 implementation.  */
void
software_rand64_fini (void)
{
  fclose (urandstream);
}

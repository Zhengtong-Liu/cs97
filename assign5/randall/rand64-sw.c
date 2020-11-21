#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;
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

unsigned long long
software_mrand48_r (void)
{
  srand48(time(NULL));
  return mrand48();
}

/* Finalize the software rand64 implementation.  */
void
software_rand64_fini (void)
{
  fclose (urandstream);
}

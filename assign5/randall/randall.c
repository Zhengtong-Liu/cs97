/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */


#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./output.h"
#include "./rand64-hw.h"
#include "./rand64-sw.h"
#include "./options.h"

int
main (int argc, char **argv)
{

  struct opts options;
  options.valid = false;
  read_options(argc, argv, &options);

  if(!options.valid)
  {
    fprintf (stderr, "%s: incorrect usage\n", argv[0]);
    return 1;
  }
  if (options.nbytes == 0)
    return 0;
  if (options.nbytes < 0)
    return 1;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  // void (*initialize) (char* file) = NULL;
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  // printf("%d\n", options.input);
  if(options.input == MRAND48_R)
  {
    software_rand64_init("/dev/random");
    rand64 = software_mrand48_r;
    finalize = software_rand64_fini;
  }
  else if(options.input == SLASH_F)
  {
    software_rand64_init(options.r_src);
    rand64 = software_rand64;
    finalize = software_rand64_fini;
  }
  else
  {
    if(!rdrand_supported ())
    {
      fprintf(stderr, "rdrand is not available.\n");
      return 1;
    }
    else
    {
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    }
  }
  

  int wordsize = sizeof rand64 ();
  // printf("%d", wordsize);
  int output_errno = 0;

  int nbytes = options.nbytes;

 
  do
  {
    // printf("there are %d to be printed \n", nbytes);
    unsigned long long x = rand64 ();
    // printf("%lld\n", x);
    int outbytes = nbytes < wordsize ? nbytes : wordsize;
      
    if (!writebytes2(x, outbytes))
    {
      output_errno = errno;
      break;
    }
        nbytes -= outbytes;
  }
  while (0 < nbytes);
  

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }

  finalize ();
  return !!output_errno;
}

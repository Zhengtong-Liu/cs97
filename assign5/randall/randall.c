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
    fprintf (stderr, "%s: Incorrect usage\n", argv[0]);
    exit(1);
  }
  if (options.nbytes == 0)
    return 0;
  // if (options.nbytes < 0)
  //   exit(1);

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  // void (*initialize) (char* file) = NULL;
  unsigned long long (*rand64) (void);
  void (*finalize) (void);
  bool rdrand_s = rdrand_supported();
  // printf("%d\n", options.input);
  if(options.input == MRAND48_R)
  {
    mrand48_rng_init();
    rand64 = mrand48_rng;
    finalize = software_rand64_fini;
  }
  else if(options.input == SLASH_F)
  {
    software_rand64_init(options.r_src);
    rand64 = software_rand64;
    finalize = software_rand64_fini;
  }
  else if(options.input == RDRAND)
  {
    if(!rdrand_s)
    {
      fprintf(stderr, "rdrand is not available.\n");
      exit(1);
    }
    rand64 = hardware_rand64;
    finalize = hardware_rand64_fini; 
  }
  else{
    if (rdrand_s)
    {
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    }
    else
    {
      software_rand64_init("/dev/random");
      rand64 = software_rand64;
      finalize = software_rand64_fini;
    }
  }
  

  int wordsize = sizeof rand64 ();
  // printf("%d", wordsize);
  int output_errno = 0;

  int nbytes = options.nbytes;

// printf("the output option is %d \n", options.output);
if(options.output == N)
{
  // printf("the block size is %d \n", options.block_size);
  // if(options.block_size < 0)
  // {
  //   fprintf(stderr, "the block size specified is negative.\n");
  //   exit(1);
  // }
  int block_size = options.block_size * 1000;
  while(nbytes >= block_size)
  {
    char* pt = (char*) malloc(block_size);
    if(pt == NULL)
    {
      fprintf(stderr, "unable to satisfy this memory allocation request.\n");
      exit(1);
    }
    for(int k = 0; k < block_size; k++)
    {
      pt[k] = rand64 ();
    }
    pt[block_size] = '\0';
    if(!writebytes2(pt, block_size))
    {
      free(pt);
      fprintf(stderr, "fail to write to stdout.\n");
      exit(1);
    }
    else
      free(pt);
    nbytes -= block_size;
  }
  if(nbytes > 0)
  {
    char* pt = (char*) malloc(nbytes);
    if(pt == NULL)
    {
      fprintf(stderr, "unable to satisfy this memory allocation request.\n");
      exit(1);
    }
    for(int k = 0; k < nbytes; k++)
      pt[k] = rand64 ();
    pt[nbytes] = '\0';
    if(!writebytes2(pt, nbytes))
    {
      free(pt);
      fprintf(stderr, "fail to write to stdout.\n");
      exit(1);
    }
    else
      free(pt);
  }
}
else
{
   do
  {
    unsigned long long x = rand64 ();
    // printf("%lld\n", x);
    int outbytes = nbytes < wordsize ? nbytes : wordsize;
      
    if (!writebytes1(x, outbytes))
    {
      output_errno = errno;
      break;
    }
        nbytes -= outbytes;
  }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;
}



  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }


  finalize ();
  return !!output_errno;
}

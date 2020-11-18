#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
//for debug
#include <stdio.h>
#include "./options.h"

void read_options(
    int argc,
    char* argv[],
    struct opts* opts  
) {
    opts -> valid = false;
    int c;
    if (argv[optind] == NULL) {
        printf("Mandatory argument(s) missing\n");
        exit(1);
    }
    if(argv[optind+1] == NULL) {
        opts -> nbytes = atoi(argv[optind]);
        opts -> valid = true;
        return;
    }
    while((c = getopt(argc, argv, ":i:o:")) != -1){
        switch(c){
            case 'i':
                if(strcmp(optarg, "rdrand") == 0)
                    opts -> input = RDRAND;
                else if(strcmp(optarg, "mrand48_r") == 0)
                    opts -> input = MRAND48_R;
                else if(optarg[0] == '/'){
                    opts -> input = SLASH_F;
                    opts -> r_src = optarg;
                }else
                    break;
            case 'o':
                if(strcmp(optarg, "stdio") == 0)
                    opts -> output = STDOUT;
                else{
                    opts -> output = N;
                    opts -> block_size = atoi(optarg);
                }
                opts -> valid = true;
                break;
            case ':':
                break;
            case '?':
                break;
        }
        if(optind >= argc) return;
        opts -> nbytes = atoi(argv[optind]);
        if(opts -> nbytes >= 0)
        {
            opts -> valid = true;
        }
    }
}
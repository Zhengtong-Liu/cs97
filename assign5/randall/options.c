#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
//for debug
// #include <stdio.h>
#include "./options.h"

void read_options(
    int argc,
    char* argv[],
    struct opts* opts  
) {
    opts -> valid = false;
    int c;
    if (argc == 1) return;
    if(argc == 2) {
        // if(atoi(argv[optind]) >= 0)
        // {
        //     opts -> nbytes = atoi(argv[optind]);
        //     opts -> valid = true;
        //     return;
        // } 
        char *endptr;
        errno = 0;
        opts -> nbytes = strtoll (argv[1], &endptr, 10);
        // if (errno) perror (argv[1]);
        // else
	    opts -> valid = !*endptr && 0 <= (opts -> nbytes);
        return; 
    }
    // bool haha = (c = getopt(argc, argv, ":i:o:"));
    // printf("pass here, haha is %d, c is %d\n", haha, c);
    bool flag = true;
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
                }else{
                    flag = false;
                };
                break;
            case 'o':
                if(atoi(optarg)){
                    opts -> output = N;
                    opts -> block_size = atoi(optarg);
                }
                else if(strcmp(optarg, "stdio") == 0){
                    opts -> output = STDOUT;
                }
                else{
                    flag = false;
                }; 
                break;
            case ':':
            case '?':
                flag = false;
                break;
        }
        opts -> valid = flag;
        if(optind >= argc) return;
        opts -> nbytes = atoi(argv[optind]);
        
        if(opts -> nbytes >= 0 && flag)
             opts -> valid = true;
    }
}
#include <stdbool.h>

enum INPUT { RDRAND, MRAND48_R, SLASH_F };
enum OUTPUT { STDOUT, N };

struct opts {
    bool valid;
    long long nbytes;
    enum INPUT input;
    enum OUTPUT output;
    char* r_src;
    unsigned int block_size;
};

void read_options(
    int argc,
    char* argv[],
    struct opts* opts  
);
#include <stdio.h>

#include "options.h"
#include "dumper.h"

int main(int argc, char **argv) {

    // from the caller function, allocate an struct to store options
    struct options options;

    // options_get internally will process argv and write it into options struct
    if (!options_get(&options, argc, argv)) {
        return 1;
    }

    if (dump(&options) != 0) {
        fprintf(stderr, "Dumping failed!\n");
        return 2;
    } 

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "options.h"

static void checkArgc (int i, int argc){
    if(i+1 > argc-1){
        fprintf(stderr, "Not enough arguments. Use the flag --help to consult usage.\n");
        exit(1);
    }
}

static void checkDuplicateOption (char *optionName, bool hasBeenDetected){
    if(hasBeenDetected){
        fprintf(stderr, "Duplicate option: %s\n", optionName);
        exit(1);
    }
}

int options_get(struct options *options, int argc, char **argv) {
    options->inputfile = NULL; // interpret default as stdin
    options->outputfile = NULL; // interpret default as stdout
    options->columns = 16;

    bool columnsDetected = false;
    bool inputFileDetected = false;
    bool outputFileDetected = false;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {

            fprintf(stderr,
                "Usage: %s [-c cols] [-o output] [--] [input]\n"
                "Options:\n"
                "  -c, --columns    Set output max columns (min 1, default 16)\n"
                "  -o, --output     Set output file (stdout default)\n"
                "  -h, --help       Show this help\n"
                "  --               Stop interpreting options (filename is \"--help\" etc.)\n"
                "  input            Read the file (stdin default)\n", argv[0]);
            exit(0);

        } else if(strcmp(argv[i], "--columns") == 0 || strcmp(argv[i], "-c") == 0){

            checkArgc(i, argc);
            if(!atoi(argv[i+1]) || atoi(argv[i+1]) <= 0){
                fprintf(stderr, "Incorrect usage of flag -c, --columns. Use the flag --help to consult usage.\n");
                exit(1);
            }
            checkDuplicateOption("column size", columnsDetected);
            columnsDetected = true;

            options->columns = atoi(argv[i+1]);

        } else if(strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0){

            checkArgc(i, argc);
            checkDuplicateOption("output", outputFileDetected);
            outputFileDetected = true;

            options->outputfile = argv[i+1]; 

        }  else if(strcmp(argv[i], "--") == 0){
            // explicit end of options, next arg should be the filename (if present)
            checkArgc(i, argc);
            inputFileDetected = true;

            options->inputfile = argv[i+1]; 
            break;

        }  
    }

    // scanned entire argv, no options found; safe to interpret argument as inputFile
    if (!(columnsDetected || inputFileDetected || outputFileDetected)){
        if(argc > 2){
            fprintf(stderr, "Too many inputs.\n");
            exit(1);
        }else if(argc == 2){
            options->inputfile = argv[1];
        }
    }

    if(!inputFileDetected && argc > 1){
        options->inputfile = argv[argc-1];
    }

    return 1;
}

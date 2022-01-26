#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "options.h"

int dump(struct options *options) {
    
    // Open files for I/O, with error checking
    FILE *input, *output;
    
    if(options->inputfile){
        if (!(input = fopen(options->inputfile, "r"))){
            fprintf(stderr, "Error opening input file!\n");
            exit(4);
        }
    } else{
        input = stdin;
    }

    if(options->outputfile){
        if (!(output = fopen(options->outputfile, "w"))){  // TODO: fix
            fprintf(stderr, "Error opening output file!\n");
            exit(4);
        }
    } else{
        output = stdout;
    }

    // Begin processing input

    int usedColumns = 0;
    char *lineBuffer = malloc(options->columns) ;
    bool isStartOfLine = true;
    int currentLine = 0;

    int c;
    while((c = fgetc(input)) != EOF){
        
        // Start-of-Line actions
        if(isStartOfLine){
            fprintf(output, "%08x %s", currentLine*(options->columns),"| ");   
            isStartOfLine= false;
            ++currentLine;;
        }

        // Actually print read character as Hex
        fprintf(output, "%02x ", c);

        // Place read character in buffer for printing when column width reached 
        // (first replace with dot if not in ASCII range)
        if(c < 0x20 || c > 0x7e){
           c = '.';
        }
        lineBuffer[usedColumns] = c;

        // Update because character read = column position used
        ++usedColumns;
        
        // End-of-line actions
        if (usedColumns >= options->columns){
            // Print separator and chars in buffer
            fprintf(output, "| ");   
            for(int i = 0; i<options->columns; i++){
                fprintf(output, "%c",  lineBuffer[i]);
                lineBuffer[i] = '-';        
            }
            fprintf(output, " |\n");
            usedColumns = 0;
            isStartOfLine = true;
        }
    }

    // Fill unused columns with --
    if(!isStartOfLine && usedColumns < options->columns){
        for(int i = usedColumns; i<options->columns; i++){
            fprintf(output, "-- ");        
        }
        fprintf(output, "| ");   
        for(int i = 0; i<options->columns; i++){
            if( i >= usedColumns){
                fprintf(output, "%c", '-');        
            }else{
                fprintf(output, "%c", lineBuffer[i]);        
            }
        }
        fprintf(output, " |\n");        
    }

    // Free allocated buffer, close opened files
    free(lineBuffer);
    if (fclose( input ) != 0){
        fprintf(stderr, "Error closing input file!\n");
        return 1;
    }
    if (fclose( output ) != 0){
        fprintf(stderr, "Error closing output file!\n");
        return 1;
    }
    return 0;
}


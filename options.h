#pragma once

struct options {
    char *inputfile;
    char *outputfile;
    int columns;
};

int options_get(struct options *, int argc, char **argv);


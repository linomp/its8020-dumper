#!/bin/sh
MAXTIME=3s
DUMPER="../dumper"
printf "\e[36;1m--- STARTING TESTING ---\e[m\n";

cd data
find . -name "*.last" -delete

failure=0

fail() {
    printf "\e[31;1m$1\e[m\n";
    failure=1
}

test_data() {
    printf "$1 \e[90m...\e[m ";
    text=$(timeout $MAXTIME sh -c "$1" 2>&1)
    ret=$?
    if [ $ret -eq 124 ]; then
        fail "Timeout occurred ($MAXTIME)!";
        return 1;
    elif [ $ret -eq 139 ]; then
        fail "SEGFAULT occurred!";
        return 1;
    elif [ $ret -ne 0 ]; then
        fail "Program returned error: \"$text\"";
        return 2;
    fi
    
    dout=$(diff -b -- "$2" "$3")
    dret=$?
    if [ $dret -ne 0 ]; then
        fail "Output is different!";
        printf "%s\n" "$dout";
        return 3;
    fi
    
    printf "\e[32;1mOK\e[m\n";
    return 0;
}

test_error() {
    printf "$1 \e[90m...\e[m ";
    text=$(timeout $MAXTIME sh -c "$1" < /dev/null 2>&1)
    ret=$?
    if [ $ret -eq 124 ]; then
        fail "Timeout occurred ($MAXTIME)!";
        return 1;
    elif [ $ret -eq 139 ]; then
        fail "SEGFAULT occurred!";
        return 1;
    elif [ $ret -eq 0 ]; then
        fail "$2";
        return 2;
    fi

    printf "\e[32;1mOK: \"$text\"\e[m\n";
    return 0;
}

test_data "$DUMPER < test_01.in > test_01.last" \
    test_01.out test_01.last

test_data "$DUMPER < test_02.in > test_02.last" \
    test_02.out test_02.last

test_data "$DUMPER -c 8 -o test_03.last test_03.in" \
    test_03.out test_03.last    
    
test_data "$DUMPER --columns 1 --output test_04.last test_04.in" \
    test_04.out test_04.last

test_data "$DUMPER -- --help > --help.last" \
    --help.out --help.last

test_error "$DUMPER -c" "Should check for column number!";
test_error "$DUMPER -c asdf" "Should check for column format!";
test_error "$DUMPER -c 0" "Should check for column size < 1";
test_error "$DUMPER -c -20" "Should check for negative column size";
test_error "$DUMPER -c 5 -c 6" "Should have only one column size!";
test_error "$DUMPER --output" "Should check for output filename!";
test_error "$DUMPER -o" "Should check for output filename!";
test_error "$DUMPER -o file1.last -o file2.last < /dev/null" "Should have only one output!";
test_error "$DUMPER nonexist" "Should check for input filename existing!";
test_error "$DUMPER -o /" "Should check for output file!";
test_error "$DUMPER test_01.in test_02.in" "Should check for too many inputs!";

if [ $failure -ne 0 ]; then
    printf "\e[31;1m--- FAILED TESTING ---\e[m\n";
    exit 1;
else
    printf "\e[32;1m--- SUCCEEDED TESTING ---\e[m\n";
fi

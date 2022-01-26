# Systems Programming - hexdump

## TL;DR:

Clone the repo & run:

```
make test
```

## Description (by prof. [Henri Vasserman](https://github.com/SlyEcho))


This assignment is about writing a hexadecimal binary dumping program called `dumper`.
It works similarily as the programs [xxd](https://linux.die.net/man/1/xxd) and [hexdump](https://linux.die.net/man/1/hexdump).

The program reads in from the input some data and then shows the bytes in hexadecimal numerals as well as plain ASCII.
Each line also shows the offset of the address that these bytes are at.

## Sample

Given this five byte file (inluding the final `\n`):

```
abcd

```

The program will output:

```
00000000 | 61 62 63 64 0a -- -- -- -- -- -- -- -- -- -- -- | abcd.----------- |
```

## Output format

There are three parts:

- The offset: it's 8 hexadecimal digits (good enough for 4 GiB)
- The bytes in hexadecimals or `--` if the file has ended.
- The ASCII representation or `-` if file is ended.
  The character range that should be shown is from `' '` to `'~'` or `0x20` to `0x7e` inclusive, other wise it shows just `'.'`
  
The three parts are separated by pipe symbols `'|'` to create a sort of a table.
The end also has a `'|'` symbol.

All hexadecimal characters `'a'` to `'f'` are lowercase.

By default the program will show 16 bytes per line.

## Command line arguments

The second part of this assignment is to properly parse the command line arguments using some standard conventions that are used to parse arguments.

There are a few arguments as can be seen from the help option:

```
Usage: ./dumper [-c cols] [-o output] [--] [input]
Options:
  -c, --columns    Set output max columns (min 1, default 16)
  -o, --output     Set output file (stdout default)
  -h, --help       Show this help
  --               Stop interpreting options (filename is "--help" etc.)
  inputs           Read all the files (stdin default)
```

There are options to give the input and output files on the command line.
The output with the `-o` option and the input with just the filename.

If there is no input filename, the the standard input should be read.
Same with the output filename, if no `-o` option is present, then standard output is used.

The `--` option is special because it only signifies that there are no more options, this is useful for situations where the input file name is the same as an option string (like `--help` or `-o` etc).

## Exit code

Your program should give a non-zero exit code on any kind of error that occurs.

If any of the command line arguments are wrong tha would also mean an error code that is not zero.
You can decide yourself what codes to use.

The program should also give the user some error message on `stderr` and not on the standard output.

## Template

The files here in this directory are a template that you can use to start writing the program.

## Testing

There is a shell script `test.sh` and some test files in `data`.
To run the test script just run it on the command line:

```
./test.sh
```

The provided `Makefile` also has a target `test` that you can invoke:

```
make test
```

which also builds the program before running the tests.

For correct output it should look something like this:

```
--- STARTING TESTING ---
../dumper < test_01.in > test_01.last ... OK
../dumper < test_02.in > test_02.last ... OK
../dumper -c 8 -o test_03.last test_03.in ... OK
../dumper -- --help > --help.last ... OK
../dumper -c ... OK: "Expected column count after -c"
../dumper -c asdf ... OK: "Invalid number: asdf"
../dumper -c 3 ... OK: "Minimum columns is 4"
../dumper -c -20 ... OK: "Minimum columns is 4"
../dumper --output ... OK: "Expected input file after --output argument"
../dumper -o ... OK: "Expected input file after -o argument"
../dumper nonexist ... OK: "Couldn't open file 'nonexist'"
../dumper test_01.in test_02.in ... OK: "Too many inputs: 'test_01.in' and 'test_02.in'."
--- SUCCEEDED TESTING ---
```

It should also show colors on compatible terminals.

The command line arguments are checked to see if an error result is exited from the program (error means anything not `0`).
It will also show whatever error message you show to the user.

If the test fails then it shows you an error message in case of the command line options test or the output of the `diff` command for the set dataset tests.

The script has been tested on Linux (Ubuntu and Arch) as well as NetBSD.
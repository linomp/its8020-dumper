program = dumper
SOURCES = dumper.c main.c options.c
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
CFLAGS = -Wall -Wextra

tstOutput = someOutputFile.txt
tstInput = data/test_01.in
tstCols = 16

ifeq ($(DEBUG), 1)
CFLAGS += -DNDEBUG -g -O0
else
CFLAGS += -O3
endif

.PHONY: all clean test
all: $(program)

$(program): $(OBJECTS)
	$(CC) -o $@ $(LDLIBS) $(LDFLAGS) $^

$(OBJECTS): %.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm *.o $(program)

test: $(program)
	./test.sh

deps.mk: $(SOURCES)
	$(CC) -MM $^ > deps.mk

run:
	./$(program) -c $(tstCols) -o $(tstOutput) -- $(tstInput)

runWout:
	./$(program) -c $(tstCols) -o $(tstOutput) -- $(tstInput)

new: 
	make clean && make $(program)
	
include deps.mk

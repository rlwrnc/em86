cc=gcc
flags=-g -Wall

all: test

em86: em86.o
	$(cc) $(flags) -o $@ $<

%.o: %.c
	$(cc) $(flags) -c $< -o $@

test: em86.o
	$(cc) $(flags) em86.o test/test.c -o test/$@
	test/test

clean:
	rm *.o test/test

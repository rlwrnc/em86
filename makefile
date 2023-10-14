cc=gcc
flags=-g -Wall

all: em86

em86: em86.o
	$(cc) $(flags) -o $@ $<

%.o: %.c
	$(cc) $(flags) -c $< -o $@

clean:
	rm em86 *.o

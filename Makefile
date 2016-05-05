.PHONY:clean all
CC=gcc
CFLAGS=-Wall -c
BIN=myshell
all:$(BIN)
myshell:main.o shell.o init.o
	$(CC) $^ -o $@
main.o:main.c shell.h init.h def.h 
	$(CC) $(CFLAGS) $< -o $@
shell.o:shell.c shell.h def.h init.h
	$(CC) $(CFLAGS) $< -o $@
init.o:init.c init.h
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -f *.o $(BIN)

all: main

main: main.c
	cc -Wall -o hipposhell main.c

run: main
	./hipposhell

clean:
	rm -f hipposhell


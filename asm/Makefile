# Makefile
all: div

div: div.o
	ld -o div div.o

div.o: div.s
	as -o div.o div.s


clean:
	rm -vf *.o

uninstall: clean
	rm -vf div

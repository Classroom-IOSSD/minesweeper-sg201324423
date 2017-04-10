all: minesweeper

minesweeper: minesweeper.o conio.o
	gcc minesweeper.o conio.o -o minesweeper

conio.o: conio.c conio.h
	gcc -c conio.c

minesweeper.o: minesweeper.c conio.h
	gcc -c minesweeper.c


.PHONY: all clean
clean:
	rm minesweeper.o conio.o

CC = gcc
CFLAGS = -Wall -pedantic -ansi -g
OBJS = main.o file_io.o game.o color.o terminal.o linkedlist.o
EXEC = escape

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

main.o: main.c file_io.h game.h terminal.h linkedlist.h
	$(CC) $(CFLAGS) -c main.c

file_io.o: file_io.c file_io.h
	$(CC) $(CFLAGS) -c file_io.c

game.o: game.c game.h color.h
	$(CC) $(CFLAGS) -c game.c

color.o: color.c color.h
	$(CC) $(CFLAGS) -c color.c

terminal.o: terminal.c terminal.h
	$(CC) $(CFLAGS) -c terminal.c

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c

clean:
	rm -f $(OBJS) $(EXEC)
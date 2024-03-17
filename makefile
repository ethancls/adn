CC=gcc
main: main.o gen.o 
	$(CC) -o main main.o gen.o -lm
clean:
	rm *.o main

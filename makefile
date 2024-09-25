CC=gcc
main: main.o gen.o 
	$(CC) -o main main.o gen.o -lm -O3
clean:
	rm *.o main

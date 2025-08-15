all: main.o vaderSentiment.o
	gcc -o main main.o vaderSentiment.o -lm

# Compile each .c file into an object file
main.o: main.c
	gcc -c main.c

vaderSentiment.o: vaderSentiment.c
	gcc -c vaderSentiment.c

clean:
	rm -f main main.o vaderSentiment.o

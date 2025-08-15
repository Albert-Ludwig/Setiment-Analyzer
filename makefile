all: 
	gcc -O3 -shared -o libmysort.so -fPIC mySort.c

.PHONY: all 
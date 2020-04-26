all: p00

p00: testafila.o queue.o 
	gcc queue.o testafila.o -o p00 -no-pie

testafila.o: testafila.c
	gcc -c testafila.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p00

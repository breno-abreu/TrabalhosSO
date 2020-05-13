all: p10

p10: pingpong.o pingpong-prodcons.o queue.o
	gcc pingpong.o pingpong-prodcons.o  queue.o -o p10 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-prodcons.o: pingpong-prodcons.c 
	gcc -c pingpong-prodcons.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p10

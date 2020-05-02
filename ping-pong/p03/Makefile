all: p03

p03: pingpong.o pingpong-dispatcher.o queue.o
	gcc pingpong.o pingpong-dispatcher.o queue.o -o p03 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-dispatcher.o: pingpong-dispatcher.c
	gcc -c pingpong-dispatcher.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p03

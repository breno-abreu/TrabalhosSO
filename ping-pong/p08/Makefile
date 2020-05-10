all: p08

p08: pingpong.o pingpong-join.o queue.o
	gcc pingpong.o pingpong-join.o  queue.o -o p08 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-join.o: pingpong-join.c 
	gcc -c pingpong-join.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p08

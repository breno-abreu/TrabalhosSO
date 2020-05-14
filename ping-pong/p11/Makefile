all: p11

p11: pingpong.o pingpong-barrier.o queue.o
	gcc pingpong.o pingpong-barrier.o  queue.o -o p11 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-barrier.o: pingpong-barrier.c 
	gcc -c pingpong-barrier.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p11

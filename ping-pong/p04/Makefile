all: p04

p04: pingpong.o pingpong-scheduler.o queue.o
	gcc pingpong.o pingpong-scheduler.o queue.o -o p04 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-scheduler.o: pingpong-scheduler.c
	gcc -c pingpong-scheduler.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p04

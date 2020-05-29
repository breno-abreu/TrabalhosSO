all: p02-1

p02-1: pingpong.o pingpong-tasks1.o queue.o
	gcc pingpong.o pingpong-tasks1.o queue.o -o p02-1 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-tasks1.o: pingpong-tasks1.c
	gcc -c pingpong-tasks1.c

queue.o: queue.c
	gcc -c queue.c
clean:
	rm -rf *.o p02-1

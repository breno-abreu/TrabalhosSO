all: p02-2

p02-2: pingpong.o pingpong-tasks2.o queue.c
	gcc pingpong.o pingpong-tasks2.o queue.c -o p02-2 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-tasks1.o: pingpong-tasks2.c
	gcc -c pingpong-tasks2.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p02-2

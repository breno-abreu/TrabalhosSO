all: p02-3

p02-3: pingpong.o pingpong-tasks3.o queue.c
	gcc pingpong.o pingpong-tasks3.o queue.c -o p02-3 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-tasks1.o: pingpong-tasks3.c
	gcc -c pingpong-tasks3.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p02-3

all: p02-2

p02-2: pingpong.o pingpong-tasks2.o
	gcc pingpong.o pingpong-tasks2.o -o p02-2 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-tasks1.o: pingpong-tasks2.c
	gcc -c pingpong-tasks2.c

clean:
	rm -rf *.o p02-2

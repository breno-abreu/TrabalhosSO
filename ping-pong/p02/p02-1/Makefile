all: p02-1

p02-1: pingpong.o pingpong-tasks1.o
	gcc pingpong.o pingpong-tasks1.o -o p02-1 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-tasks1.o: pingpong-tasks1.c
	gcc -c pingpong-tasks1.c

clean:
	rm -rf *.o p02-1

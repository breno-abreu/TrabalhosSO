all: p02-3

p02-3: pingpong.o pingpong-tasks3.o
	gcc pingpong.o pingpong-tasks3.o -o p02-3 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-tasks1.o: pingpong-tasks3.c
	gcc -c pingpong-tasks3.c

clean:
	rm -rf *.o p02-3

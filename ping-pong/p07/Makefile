all: p07

p07: pingpong.o pingpong-maintask.o queue.o
	gcc pingpong.o pingpong-maintask.o  queue.o -o p07 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-maintask.o: pingpong-maintask.c 
	gcc -c pingpong-maintask.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p07

all: p09

p09: pingpong.o pingpong-sleep.o queue.o
	gcc pingpong.o pingpong-sleep.o  queue.o -o p09 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-sleep.o: pingpong-sleep.c 
	gcc -c pingpong-sleep.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p09

all: p06

p06: pingpong.o pingpong-contab-prio.o queue.o
	gcc pingpong.o pingpong-contab-prio.o  queue.o -o p06 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-contab-prio.o: pingpong-contab-prio.c 
	gcc -c pingpong-contab-prio.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p06

all: p10

p10: pingpong.o pingpong-semaphore.o queue.o
	gcc pingpong.o pingpong-semaphore.o  queue.o -o p10 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-semaphore.o: pingpong-semaphore.c 
	gcc -c pingpong-semaphore.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p10

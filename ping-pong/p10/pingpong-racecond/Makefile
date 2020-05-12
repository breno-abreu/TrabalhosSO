all: p10

p10: pingpong.o pingpong-racecond.o queue.o
	gcc pingpong.o pingpong-racecond.o  queue.o -o p10 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-racecond.o: pingpong-racecond.c 
	gcc -c pingpong-racecond.c

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p10

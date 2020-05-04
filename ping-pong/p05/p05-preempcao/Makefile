all: p05

p05: pingpong.o pingpong-preempcao.o queue.o
	gcc pingpong.o pingpong-preempcao.o  queue.o -o p05 -no-pie

pingpong.o: pingpong.c
	gcc -c pingpong.c

pingpong-dispatcher.o: pingpong-preempcao.o 
	gcc -c pingpong-preempcao.o

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o p05

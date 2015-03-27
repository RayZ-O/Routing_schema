CC=g++ -g -std=c++11 

heap.out: main.o F_heap.o
	$(CC) -o heap.out main.o F_heap.o

main.o: main.cc
	$(CC) -c main.cc -o main.o

F_heap.o: F_heap.cc
	$(CC) -c F_heap.cc -o F_heap.o

clean: 
	rm -f heap.out
	rm -f *.o

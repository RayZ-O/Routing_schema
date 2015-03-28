CC=g++ -g -std=c++11 -Wall

ssp: main.o FibonacciHeap.o Dijkstra.o
	$(CC) -o ssp main.o FibonacciHeap.o Dijkstra.o

main.o: main.cc
	$(CC) -c main.cc -o main.o

Dijkstra.o: Dijkstra.cc
	$(CC) -c Dijkstra.cc -o Dijkstra.o

FibonacciHeap.o: FibonacciHeap.cc
	$(CC) -c FibonacciHeap.cc -o FibonacciHeap.o

clean: 
	rm -f ssp
	rm -f *.o

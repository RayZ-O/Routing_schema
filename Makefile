CC=g++ -g -std=c++11 -Wall
SOURE=src

routing: main.o FibonacciHeap.o Dijkstra.o Trie.o
	$(CC) -o routing  main.o FibonacciHeap.o Dijkstra.o Trie.o

main.o: $(SOURE)/main.cc
	$(CC) -c $(SOURE)/main.cc -o main.o

Dijkstra.o: $(SOURE)/Dijkstra.cc
	$(CC) -c $(SOURE)/Dijkstra.cc -o Dijkstra.o

Trie.o: $(SOURE)/Trie.cc
	$(CC) -c $(SOURE)/Trie.cc -o Trie.o

FibonacciHeap.o: $(SOURE)/FibonacciHeap.cc
	$(CC) -c $(SOURE)/FibonacciHeap.cc -o FibonacciHeap.o

clean: 
	rm -f routing
	rm -f *.o
	rm -f *~


#include "FibonacciHeap.cc"
#include "Dijkstra.h"
#include <iostream>




int main (int argc, char *argv[]) {
	FibonacciHeap<int> fh;
	Graph G;

	G.initGraph("input_1000_50_part1.txt");
	G.print();
}

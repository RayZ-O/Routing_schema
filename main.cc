#include "F_heap.cc"
#include <iostream>

int main () {
	FibonacciHeap<int> fh;
	for(int i=1; i<12; i++){
		fh.insert(i);
	}
	std::cout << "size is " << fh.size() << std::endl;
	fh.print();
	// int min;
	// fh.remove_min(min);
	// std::cout << "min is " << min << std::endl;
	// std::cout << "size is " << fh.size() << std::endl;
	// fh.print();
	// fh.remove_min(min);
	// std::cout << "min is " << min << std::endl;
	// std::cout << "size is " << fh.size() << std::endl;
	// fh.print();


	FibonacciHeap<int> fh2;
	for(int i=12; i<17; i++){
		fh2.insert(i);
	}
	fh.meld(fh2);
	std::cout << "size is " << fh.size() << std::endl;
	fh.print();
	int min;
	fh.remove_min(min);
	std::cout << "min is " << min << std::endl;
	std::cout << "size is " << fh.size() << std::endl;
	fh.print();
}
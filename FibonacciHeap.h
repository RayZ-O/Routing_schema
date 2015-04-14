#ifndef _FIBONACCI_HEAP_H
#define _FIBONACCI_HEAP_H

#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <limits>
#include <cmath>

using std::cout;
using std::cerr;
using std::endl;

//node of fibonacci heap
class HeapNode	{
friend class FibonacciHeap;
private:
	long data;
	long verID;
	int degree;
	bool childCut;
	HeapNode *parent;
	HeapNode *child;
	HeapNode *lsibling;
	HeapNode *rsibling;
	HeapNode () : data(-1), verID(-1), degree(0), childCut(false), parent(nullptr), child(nullptr), lsibling(nullptr), rsibling(nullptr) { }
	//delete default copy constructor and copy assignment
	HeapNode (const HeapNode &copyMe) = delete;
	HeapNode & operator = (const HeapNode &copyMe) = delete; 
	~HeapNode() {
		if (child != nullptr) {
			delete child;
			child = nullptr;
		} else if (rsibling != nullptr) {
			lsibling->rsibling = nullptr;
			delete rsibling;
			rsibling = nullptr;
		}
	}
};

class FibonacciHeap {
private:	
	HeapNode *minNode;	//pointer to minimum node 
	int numItem;	//number of nodes in the heap
	// remove node from doubly linked list
	void list_remove(HeapNode *eraseMe);
	// meld two doubly linked list
	void meld_list (HeapNode *firstListNode, HeapNode *secondListNode);
	// cut the subtree rooted in rootIn and meld the tree to top list
	void cut_subtree (HeapNode *rootIn);
	// cascading cut from beginNode to root
	void cascading_cut (HeapNode *beginNode);
	// exchange two node pointer
	void exchange(HeapNode **x, HeapNode **y);
	// combine two tree, insert the tree with bigger root to the children list of another
	void combine_trees (HeapNode *y, HeapNode *x);
	// pairwise combine all trees with the same degree in the heap
	void pairwise_combine ();
	
public:
	FibonacciHeap ();
	~FibonacciHeap ();
	//delete default copy constructor and copy assignment
	FibonacciHeap (const FibonacciHeap &copyMe) = delete;
	FibonacciHeap & operator = (const FibonacciHeap &copyMe) = delete;
	// insert node into the heap and set corresponding vertex ID
	HeapNode* insert (long item, long verID);
	// remove min element in the heap
	long remove_min (long &verID);
	// remove arbitrary node
	void remove (HeapNode *removeMe);
	// meld heap
	void meld (FibonacciHeap &heap);
	// decrease key of decMe to be newData
	void decrease_key (HeapNode *decMe, long newData);
	// return size of this heap
	int size();
};


#endif

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
class Node	{
friend class FibonacciHeap;
private:
	long data;
	int verID;
	int degree;
	bool childCut;
	Node *parent;
	Node *child;
	Node *lsibling;
	Node *rsibling;
	Node () : data(-1), verID(-1), degree(0), childCut(false), parent(nullptr), child(nullptr), lsibling(nullptr), rsibling(nullptr) { }
	//delete default copy constructor and copy assignment
	Node (const Node &copyMe) = delete;
	Node & operator = (const Node &copyMe) = delete; 
};

class FibonacciHeap {
private:	
	Node *minNode;	//pointer to minimum node 
	int numItem;	//number of nodes in the heap
	// remove node from doubly linked list
	void list_remove(Node *eraseMe);
	// meld two doubly linked list
	void meld_list (Node *firstListNode, Node *secondListNode);
	// cut the subtree rooted in rootIn and meld the tree to top list
	void cut_subtree (Node *rootIn);
	// cascading cut from beginNode to root
	void cascading_cut (Node *beginNode);
	// exchange two node pointer
	void exchange(Node **x, Node **y);
	// combine two tree, insert the tree with bigger root to the children list of another
	void combine_trees (Node *y, Node *x);
	// piarwise combine all trees with the same degree in the heap
	void pairwise_combine ();
	
public:
	FibonacciHeap ();
	~FibonacciHeap ();
	//delete default copy constructor and copy assignment
	FibonacciHeap (const FibonacciHeap &copyMe) = delete;
	FibonacciHeap & operator = (const FibonacciHeap &copyMe) = delete;
	// insert node into the heap and set corresponding vertex ID
	Node* insert (long item, int verID);
	// remove min element in the heap
	int remove_min (int &verID);
	// remove arbitrary node
	void remove (Node *removeMe);
	// meld heap
	void meld (FibonacciHeap &heap);
	// decrease key of decMe to be newData
	void decrease_key (Node *decMe, long newData);
	// return size of this heap
	int size();
};


#endif

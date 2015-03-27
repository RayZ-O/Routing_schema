#ifndef _FIBONACCI_HEAP_H
#define _FIBONACCI_HEAP_H

#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <cmath>

template <class Type>
class FibonacciHeap{
private:	
	struct Node	{
		Type data;
		int degree;
		bool childCut;
		Node *parent;
		Node *child;
		Node *lsibling;
		Node *rsibling;

		Node () : data (), degree (0), childCut(false), parent (nullptr), child (nullptr), lsibling (nullptr), rsibling(nullptr) {}
		~Node () {
			parent = nullptr;
			child = nullptr;
			lsibling = nullptr;
			rsibling = nullptr;
		}
	};
	Node *minNode;
	int numItem;

	void list_insert(Node *insertMe, Node *besideMe);
	void list_remove(Node *eraseMe);
	void meld_list (Node *firstListNode, Node *secondListNode);
	void cut_subtree (Node *rootIn);
	void cascading_cut (Node *beginNode);
	void find_min (Node *aNode);
	void join_min_trees (Node * &root1, Node * &root2);
	void pairwise_combine ();
	
public:
	FibonacciHeap ();

	~FibonacciHeap ();

	void insert (Type item);

	int remove_min (Type &item);

	void remove (Node *removeMe);

	void meld (FibonacciHeap &heap);

	void decrease_key (Node *decreaseMe, Type amount);

	int size();
	void print();
};


#endif

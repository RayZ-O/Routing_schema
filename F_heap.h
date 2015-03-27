#ifndef _FIBONACCI_HEAP_H
#define _FIBONACCI_HEAP_H

#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <cmath>

template <class Type>
struct Node	{
	Type data;
	int degree;
	bool childCut;
	Node *parent;
	Node *child;
	Node *lsibling;
	Node *rsibling;

	Node () : data (0), degree (0), childCut(false), parent (0), child (0), lsibling (0), rsibling(0) {}
		
	~Node () {
	}
};

template <class Type>
class FibonacciHeap{
private:	
	Node *minNode;
	int numItem;

	void InsertIntoDoublyList(Node *insertMe, Node *besideMe);

	void CutSubTree (Node *rootIn);
	void Reinsert (Node *firstChild, int degree);
	void CascadingCut (Node *beginNode);
	void FindNextMin (Node* aNode);
	void JoinMinTrees (Node *root1, Node *root2);
	void PairwiseCombine ();
	
public:
	FibonacciHeap ();

	~FibonacciHeap ();

	void Insert (Type &item);

	int RemoveMin (Type &item);

	//void RemoveMax (Type *item);

	void Remove (Node *removeMe);

	void Meld (FibonacciHeap *heap1, FibonacciHeap *heap2);

	void DecreaseKey (Node *decreaseMe, Type &amount);

	//void IncreaseKey (Node *increaseMe, Type amount);
};


#endif

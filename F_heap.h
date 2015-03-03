#ifndef _FIBONACCI_HEAP_H
#define _FIBONACCI_HEAP_H

template <class Type>
class FibonacciHeap{
private:
	struct Node	{
		Type *data;
		int degree;
		bool childCut;
		Node *parent;
		Node *child;
		Node *lsibling;
		Node *rsibling;

		Node () : data (0), degree (0), childCut(false), parent (0), child (0), lsibling (0), rsibling(0) {}
			
		~Node () {
			delete data;
		}
	};
	Node *minNode;
	int numItem;
	void FindNextMin(Node* nextMin);
	void RemoveMinTree(Type *item);
	void CutSubTree(Node *rootIn);
	void Reinsert(Node *firstChild, int degree);
	void JoinMinTrees(Node *root1, Node *root2);
	void PairwiseCombine();
	
public:
	FibonacciHeap ();

	~FibonacciHeap ();

	void Insert (Type *item);

	int RemoveMin (Type *item);

	//void RemoveMax (Type *item);

	void Remove (Node *removeMe);

	void Meld (FibonacciHeap *heap1, FibonacciHeap *heap2);

	void DecreaseKey (Node *decreaseMe, Type amount);

	//void IncreaseKey (Node *increaseMe, Type amount);
};


#endif

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
	
public:
	FibonacciHeap ();

	~FibonacciHeap ();

	void Insert (Type *item);

	Type* RemoveMin ();

	Type* RemoveMax ();

	void Remove (Noed *removeMe);

	void Meld (FibonacciHeap *heap1, FibonacciHeap *heap2);

	void DecreaseKey (Node *decreaseMe, Type amount);

	void IncreaseKey (Node *increaseMe, Type amount);
};


#endif
#include "FibonacciHeap.h"




FibonacciHeap :: FibonacciHeap() : minNode(nullptr), numItem(0) {
}


FibonacciHeap :: ~FibonacciHeap (){

}

void FibonacciHeap :: print() {
	Node *p = minNode;
	do {
		cout << p->data << " ";
		p = p->rsibling;
	} while(p != minNode);
	cout << endl;
}

// Add a new single-node min tree to the collection
// Update min-element pointer if necessary
Node* FibonacciHeap :: insert (long item, int verID){
	Node *temp = new (std::nothrow) Node;
	if (nullptr == temp) {
		cerr << "ERROR : Not enough memory. EXIT !!!\n";
		exit(1);
	}
	//add item to data field
	temp->data = item;
	temp->verID = verID;
	temp->lsibling = temp;
	temp->rsibling = temp;
	//if the heap is empty, left and right sibling point to itself
	if (nullptr == minNode) {
		minNode = temp;
		minNode->lsibling = minNode;
		minNode->rsibling = minNode;
	}
	//if the heap is not empty, insert to top circular list
	else {
		meld_list(temp, minNode);
		//update min-element pointer if necessary
		if (temp->data < minNode->data) {
			minNode = temp;
		}
	}
	//increment number of items
	++numItem;
	return temp;
} 

// meld heap2 to heap1, heap2 will be empty after meld
// Combine the 2 top-level circular lists.
void FibonacciHeap :: meld (FibonacciHeap &heap){
	//if heap2 is empty, nothing to meld to heap1
	if (nullptr == heap.minNode) {
		return;
	}
	//if heap1 is empty, copy heap2 to heap1
	else if (nullptr == minNode) {
		minNode = heap.minNode;
		numItem = heap.numItem;
		//consumed heap2
		heap.minNode = nullptr;
		heap.numItem = 0;
		return;
	} else {
		//update the heap pointer
		meld_list(minNode, heap.minNode);
		//update number of items
		numItem += heap.numItem;
		//update min pointer if necessary
		minNode = (minNode->data < heap.minNode->data)? minNode : heap.minNode;
		//consumed heap2
		heap.minNode = nullptr;
		heap.numItem = 0;
	}	
}

// Remove a min tree
// Reinsert subtrees of removed min tree
// Update binomial heap pointer
int FibonacciHeap :: remove_min (int &verID){
	if (minNode == nullptr) {
		cerr << "ERROR: Remove from empty heap\n";
		exit(1);
	}
	verID = minNode->verID;
	int item = minNode->data;

	if (minNode->child != nullptr) {
		Node *c = minNode->child;
		//set all children's parent field to nullptr and childCut field to false
		for (int i = 0; i < minNode->degree; ++i) {
			c->parent = nullptr;
			c->childCut = false;
			c = c->rsibling;
		}
		meld_list(c, minNode);
	}

	minNode->rsibling->lsibling = minNode->lsibling;
	minNode->lsibling->rsibling = minNode->rsibling;
	if (minNode->rsibling == minNode) {
		destroy_min();
		minNode = nullptr;
	} else {
		Node *temp = minNode->rsibling;
		destroy_min();
		minNode = temp;
		pairwise_combine();
	}	
	--numItem;
	//print();
	return item;
}

void FibonacciHeap :: destroy_min() {
	minNode->parent = nullptr;
	minNode->child = nullptr;
	minNode->lsibling = nullptr;
	minNode->rsibling = nullptr;
	delete minNode;
}

void FibonacciHeap :: remove (Node *rmMe){
	if (nullptr == rmMe) {
		cerr << "ERROR: Attempt to remove non-exist node\n";
		exit(1);
	}
	//if the node is min node
	if (rmMe == minNode) {
		int dummy;
		remove_min(dummy);
	} else {
		Node *parent = rmMe->parent;
		//cut the subtree rooted in the node
		cut_subtree(rmMe);		
		//update parent and childCut for all children of rmMe
		//and find the minimum node
		if (rmMe->child != nullptr) {
			Node *iter = rmMe->child;
			Node *min = rmMe->child;
			do {
				iter->parent = nullptr;
				iter->childCut = false;
				if (iter->data < min->data){
					min = iter;
				}
				iter = iter->rsibling;
			} while (iter != rmMe->child);
			//meld the children list with top list
			meld_list(iter, minNode);
			//update min pointer if necessary
			if (min->data < minNode->data) {
				minNode = min;
			}
		}		
		//delete rmMe
		delete rmMe;
		//decrement number of items
		--numItem;
		//cascading cut from the node to root
		cascading_cut(parent);
	}
}



void FibonacciHeap :: decrease_key (Node *decMe, long newData){
	if (nullptr == decMe) {
		cerr << "ERROR: Attempt to decrease non-exist node\n";
		exit(1);
	} else if (newData > decMe->data) {
		cerr << "ERROR: Attempt to increas key\n";
		exit(1);
	}
	//decrease the key of decMe by amount
	decMe->data = newData;
	Node *p = decMe->parent;
	//if decMe has parent and the key of the node less than parent, cut the subtree root in the node
	if (p != nullptr && decMe->data < p->data) {
		//cut the subtree rooted in the node
		cut_subtree(decMe);
		//cascading cut from the node to root
		cascading_cut(p);
	}
	//update min pointer if necessary
	if (decMe->data < minNode->data) {
		minNode = decMe;
	}
}

int FibonacciHeap :: size () {
	return numItem;
}

//insert into doubly linked list

void FibonacciHeap :: list_remove(Node *eraseMe) {
	if(eraseMe->lsibling == nullptr || eraseMe->rsibling == nullptr || eraseMe->lsibling->rsibling == nullptr || eraseMe->rsibling->lsibling ==nullptr){
		cerr << "Null pointer exception in list_remove\n";
		exit(1);
	}
	eraseMe->lsibling->rsibling = eraseMe->rsibling;
	eraseMe->rsibling->lsibling = eraseMe->lsibling;
	eraseMe->lsibling = eraseMe;
	eraseMe->rsibling = eraseMe;
}


//meld two doubly linked list

void FibonacciHeap :: meld_list (Node *firstListNode, Node *secondListNode) {
	if(firstListNode->lsibling == nullptr || firstListNode->rsibling == nullptr || secondListNode->lsibling == nullptr || secondListNode->rsibling == nullptr){
		cerr << "Null pointer exception in meld_list\n";
		exit(1);
	}
	firstListNode->rsibling->lsibling = secondListNode->lsibling;
	secondListNode->lsibling->rsibling = firstListNode->rsibling;
	firstListNode->rsibling = secondListNode;
	secondListNode->lsibling = firstListNode;
}

//cut the subtree rooted in rootIn
void FibonacciHeap :: cut_subtree(Node *rootIn){
	Node *p = rootIn->parent;
	//decrement degree
	p->degree -= 1;
	if (rootIn == p->child && p->degree > 0) {
		p->child = rootIn->rsibling;
	} 
	//if rootIn's parent have no more children
	else if (0 == p->degree) {
		p->child = nullptr;
	}
	//take rootIn out from sibling list
	list_remove(rootIn);
	//meld the cut tree into top list
	meld_list(rootIn, minNode);	
	//update parent and childcut
	p = nullptr;
	//rootIn go into top list, set childcut to false
	rootIn->childCut = false;
}

//cascading cut when performing remove and decrease key

void FibonacciHeap :: cascading_cut(Node *curNode){
	Node *p = curNode->parent;
	//if curNode has parent
	if (p != nullptr) {
		if (false == curNode->childCut) {
			curNode->childCut = true;
		} else {
			cut_subtree(curNode);
			cascading_cut(p);
		}
	}
}

void FibonacciHeap :: exchange(Node **x, Node **y) {
	Node *temp;
	temp = *x;
	*x = *y;
	*y = temp;
}


void FibonacciHeap :: combine_trees (Node *y, Node *x) {
	list_remove(y);
	if (nullptr == x->child) {
		x->child = y;
	} else {
		meld_list(y, x->child);
	}
	y->parent = x;
	x->degree += 1;
	y->childCut = false;
}

void FibonacciHeap :: pairwise_combine(){
	//compute golden ratio
	constexpr float logGoldenRatio = log2(0.5*(1 + sqrt(5)));
	//the max degree of trees in the heap is log2(n)
	int max_degree = log2(numItem)/logGoldenRatio + 1;
	//create the degree table
	std::vector<Node*> tree;
	tree.reserve(max_degree); 
	for(int i = 0; i < max_degree; i++) {
		tree[i] = nullptr;
	}

	Node *x = minNode;
	//traverse all trees in the heap
	do {
		Node *next = x->rsibling;
		int d = x->degree;
		while (tree[d] != nullptr) {
			Node *y = tree[d];
			if (x->data > y->data) {
				exchange(&x, &y);
			}
			combine_trees(y, x);
			tree[d] = nullptr;
			++d;
		}
		tree[d] = x;
		x = next;
	} while(x != minNode);
	minNode = nullptr;
	for (int i = 0; i < max_degree; ++i) {
		if (tree[i] != nullptr) {
			if (nullptr == minNode) {
				minNode = tree[i];
				minNode->lsibling = minNode;
				minNode->rsibling = minNode;
			} else {
				meld_list(tree[i], minNode);
				if (tree[i]->data < minNode->data) {
					minNode = tree[i];
				}
			}
		}
	}
}


#include "FibonacciHeap.h"




FibonacciHeap :: FibonacciHeap() : minNode(nullptr), numItem(0) { }


FibonacciHeap :: ~FibonacciHeap (){
	delete minNode;
}

// Add a new single-node min tree to the collection
// Update min-element pointer if necessary
HeapNode* FibonacciHeap :: insert (long item, long verID){
	HeapNode *temp = new (std::nothrow) HeapNode;
	if (nullptr == temp) {
		cerr << "ERROR : Not enough memory. EXIT !!!\n";
		exit(1);
	}
	//initialize new node
	temp->data = item;
	temp->verID = verID;
	temp->lsibling = temp;
	temp->rsibling = temp;
 	//if the heap is empty
	if (nullptr == minNode) {
		minNode = temp;
	}
	//if the heap is not empty
	else {
		// insert to top circular list
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
long FibonacciHeap :: remove_min (long &verID){
	if (minNode == nullptr) {
		cerr << "ERROR: Remove from empty heap\n";
		exit(1);
	}
	//get corresponding vertex ID in the graph
	verID = minNode->verID;
	long item = minNode->data;
	//if minNode has children
	if (minNode->child != nullptr) {
		HeapNode *c = minNode->child;
		//set all children's parent field to nullptr and childCut field to false
		for (int i = 0; i < minNode->degree; ++i) {
			c->parent = nullptr;
			c->childCut = false;
			c = c->rsibling;
		}
		//clear minNode's child pointer and degree
		minNode->child = nullptr;
		minNode->degree = 0;
		//meld children list to top list
		meld_list(c, minNode);
	}
	//remove minNode from top list
	minNode->rsibling->lsibling = minNode->lsibling;
	minNode->lsibling->rsibling = minNode->rsibling;
	//if only one node in the heap
	if (minNode->rsibling == minNode) {
		delete minNode;
		minNode = nullptr;
	} else {
		HeapNode *temp = minNode->rsibling;
		minNode->rsibling = nullptr;
		minNode->lsibling = nullptr;
		delete minNode;
		//set temporary minNode
		minNode = temp;
		//pariwise combine all trees with the same degree
		pairwise_combine();
	}
	//decrement number of nodes
	--numItem;
	return item;
}

// Decrease rmMe to minus infinite then remove min
void FibonacciHeap :: remove (HeapNode *rmMe){
	if (nullptr == rmMe) {
		cerr << "ERROR: Attempt to remove non-exist node\n";
		exit(1);
	}
	constexpr long infinite = std::numeric_limits<long>::max();
	//decrease key of rmMe to minus infinite
	decrease_key(rmMe, -infinite);
	long dummy;
	remove_min(dummy);
}

// Decrease the key of decMe to newData
void FibonacciHeap :: decrease_key (HeapNode *decMe, long newData){
	if (nullptr == decMe) {
		cerr << "ERROR: Attempt to decrease non-exist node\n";
		exit(1);
	} else if (newData > decMe->data) {
		cerr << "ERROR: Attempt to increas key\n";
		exit(1);
	}
	//decrease the key of decMe
	decMe->data = newData;
	//if decMe has parent and the key of the node less than parent, cut the subtree root in the node
	HeapNode *p = decMe->parent;
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

// Return number of nodes in the heap
int FibonacciHeap :: size () {
	return numItem;
}

// Remove node from doubly linked list
void FibonacciHeap :: list_remove(HeapNode *eraseMe) {
	// if(eraseMe->lsibling == nullptr || eraseMe->rsibling == nullptr || eraseMe->lsibling->rsibling == nullptr || eraseMe->rsibling->lsibling ==nullptr){
	// 	cerr << "Null pointer exception in list_remove\n";
	// 	exit(1);
	// }
	eraseMe->lsibling->rsibling = eraseMe->rsibling;
	eraseMe->rsibling->lsibling = eraseMe->lsibling;
	eraseMe->lsibling = eraseMe;
	eraseMe->rsibling = eraseMe;
}


// Meld two doubly linked list
void FibonacciHeap :: meld_list (HeapNode *firstListNode, HeapNode *secondListNode) {
	// if(firstListNode->lsibling == nullptr || firstListNode->rsibling == nullptr || secondListNode->lsibling == nullptr || secondListNode->rsibling == nullptr){
	// 	cerr << "Null pointer exception in meld_list\n";
	// 	exit(1);
	// }
	firstListNode->rsibling->lsibling = secondListNode->lsibling;
	secondListNode->lsibling->rsibling = firstListNode->rsibling;
	firstListNode->rsibling = secondListNode;
	secondListNode->lsibling = firstListNode;
}

// Cut the subtree rooted in rootIn
void FibonacciHeap :: cut_subtree(HeapNode *rootIn){
	HeapNode *p = rootIn->parent;
	//decrement degree
	p->degree -= 1;
	//if rootIn's parent has no more child
	if (0 == p->degree) {
		p->child = nullptr;
	} 
	//if rootIn's parent's child pointer point to rootIn
	else if (rootIn == p->child) {
		p->child = rootIn->rsibling;
	} 
	//take rootIn out from sibling list
	list_remove(rootIn);
	//meld the cut tree into top list
	meld_list(rootIn, minNode);	
	//update parent and childcut
	rootIn->parent = nullptr;
	rootIn->childCut = false;
}

// Cascading cut when performing remove and decrease key
void FibonacciHeap :: cascading_cut(HeapNode *curNode){	
	//if curNode has parent
	if (curNode->parent != nullptr) {		
		if (false == curNode->childCut) {
			curNode->childCut = true;
		} else {
			HeapNode *p = curNode->parent;
			cut_subtree(curNode);
			cascading_cut(p);
		}
	}
}

// Exchange two node pointer
void FibonacciHeap :: exchange(HeapNode **x, HeapNode **y) {
	HeapNode *temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

// Combine two tree, insert the tree with bigger root to the children list of another
void FibonacciHeap :: combine_trees (HeapNode *y, HeapNode *x) {
	//remove y from top list
	list_remove(y);
	//if x has no child
	if (nullptr == x->child) {
		x->child = y;
	}
	//otherwise meld y into x's children list
	else {
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
	std::vector<HeapNode*> tree;
	tree.reserve(max_degree); 
	// initialize tree table
	for(int i = 0; i < max_degree; i++) {
		tree[i] = nullptr;
	}
	// store all nodes in the top list
	std::vector<HeapNode*> nodes;
	HeapNode *x = minNode;
	do {
		nodes.push_back(x);
		x = x->rsibling;
	} while (x != minNode);
	//traverse all trees in the heap
	for (auto root : nodes) {
		x = root;
		int d = x->degree;
		// x's degree appears in tree table
		while (tree[d] != nullptr) {
			HeapNode *y = tree[d];
			if (x->data > y->data) {
				exchange(&x, &y);
			}
			//combine two tree with the same degree
			combine_trees(y, x);
			//clear the origin position
			tree[d] = nullptr;
			//increment degree
			++d;
			// if(d > max_degree) {
			// 	cerr << "ERROR: Degree greater than max degree!\n";
			// 	exit(1);
			// }
		}
		//put x into tree table
		tree[d] = x;
	} 
	// find min node and fix top list
	minNode = nullptr;
	//traverse tree table
	for (int i = 0; i < max_degree; ++i) {
		if (tree[i] != nullptr) {
			//if minNode is not set
			if (nullptr == minNode) {
				//set the root of the tree which is first met to be min
				minNode = tree[i];
				minNode->lsibling = minNode;
				minNode->rsibling = minNode;
			}
			//otherwise, meld tree[i] into the heap 
			else {				
				tree[i]->lsibling = tree[i];
				tree[i]->rsibling = tree[i];
				meld_list(tree[i], minNode);
				if (tree[i]->data < minNode->data) {
					minNode = tree[i];
				}
			}
		}
	}
}


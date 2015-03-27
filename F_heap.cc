#include "F_heap.h"



template <class Type>
FibonacciHeap <Type> :: FibonacciHeap() minNode(NULL), numItem(0) {
}

template <class Type>
FibonacciHeap <Type> :: ~FibonacciHeap (){

}

// Add a new single-node min tree to the collection
// Update min-element pointer if necessary
template <class Type> void
FibonacciHeap <Type> :: Insert (Type &item){
	Node *temp = new (std::nothrow) Node;
	if (NULL == temp) {
		std::cerr << "ERROR : Not enough memory. EXIT !!!\n";
		exit(1);
	}
	//move semantic, consume the item
	temp->data = std::move(item);
	//if the heap is empty, left and right sibling point to itself
	if (NULL == minNode) {
		minNode = temp;
		minNode->lsibling = minNode;
		minNode->rsibling = minNode;
	}
	//if the heap is not empty, insert to top circular list
	else {
		InsertIntoDoublyList(temp, minNode);
		//update min-element pointer if necessary
		if (temp->data < minNode->data) {
			minNode = temp;
		}
	}
	//increment number of items
	++numItem;
} 

// Meld heap2 to heap1, heap2 will be empty after meld
// Combine the 2 top-level circular lists.
template <class Type> void 
FibonacciHeap <Type> :: Meld (FibonacciHeap<Type> *heap1, FibonacciHeap<Type> *heap2){
	if (NULL == heap1 || NULL == heap2){
		std::cerr << "ERROR: Attempt to MELD non-exist heap\n";
		exit(1);
	}
	//if heap2 is empty, nothing to meld to heap1
	if (NULL == heap2->minNode) {
		return;
	}
	//if heap1 is empty, copy heap2 to heap1
	else if (NULL == heap1->minNode) {
		heap1->minNode = heap2->minNode;
		heap1->numItem = heap2->numItem;
		//consumed heap2
		heap2->minNode = NULL;
		heap2->numItem = 0;
		return;
	} else {
		//update the heap pointer
		heap1->minNode->rsibling->lsibling = heap2->minNode->lsibling;
		heap2->minNode->lsibling->rsibling = heap1->minNode->rsibling;
		heap1->minNode->rsibling = heap2->minNode;
		heap2->minNode->lsibling = heap1->minNode;
		//update min pointer if necessary
		heap1->minNode = (heap1->minNode->data < heap2->minNode->data)? heap1->minNode : heap2->minNode;
		//consumed heap2
		heap2->minNode = NULL;
		heap2->numItem = 0;
	}	
}

// Remove a min tree
// Reinsert subtrees of removed min tree
// Update binomial heap pointer
template <class Type> int
FibonacciHeap <Type> :: RemoveMin (Type &item){
	if (minNode == NULL) {
		return 0;
	}	
	Node *childptr = minNode->child;
	//set all children's parent field to NULL and childCut field to false
	do {
		childptr->parent = NULL;
		childptr->childCut = false;
		childptr = iter->rsibling;
	} while (childptr != minNode->child);
	//meld the children list into the top doubly linked list
	childptr->lsibling->rsibling = minNode->rsibling;
	minNode->rsibling->lsibling = childptr->lsibling;
	childptr->lsibling = minNode->lsibling;
	minNode->lsibling->rsibling = childptr;
	//move data of minNode to item
	item = std::move(minNode->data);
	delete minNode;
	//set temporary minNode for traversing in pairwise combine
	FindNextMin(childptr);
	//pairwise combine trees in heap and update heap pointer
	PairwiseCombine();
	return 1;
}

template <class Type> void 
FibonacciHeap <Type> :: Remove (Node *rmMe){
	//if the node is min node
	if(rmMe == minNode){
		Type dummy;
		removeMin(&dummy);
	}else{
		Node *parent = rmMe->parent;
		//cut the subtree root in the node
		CutSubTree(rmMe);
		//reinsert its children
		Reinsert(rmMe->child, rmMe->degree);
		delete rmMe;
		--numItem;
		//cascading cut from the not to root
		CascadingCut(parent);
	}
}



template <class Type> void 
FibonacciHeap <Type> :: DecreaseKey (Node *decMe, Type amount){
	//if decrease the root, nothing to do
	if(decMe->parent == NULL){
		return;
	}
	//if the key of the node less than parent, cut the subtree root in the node
	if(*(decMe->data) - amount < *(decMe->parent->data)){
		Node *parent = decMe->parent;
		CutSubTree(decMe);
		Reinsert(decMe, 1);
		CascadingCut(parent);
	}
}

//cut the subtree rooted in rootIn
template <class Type>
void FibonacciHeap <Type> :: CutSubTree(Node *rootIn){
	rootIn->lsibling->rsibling = rootIn->rsibling;
	rootIn->rsibling->lsibling = rootIn->lsibling;
	rootIn->lsibling = rootIn;
	rootIn->rsibling = rootIn;
	--rootIn->parent->degree;
	rootIn->parent = NULL;
}

//reinsert number of degree subtrees in the heap
template <class Type>
void FibonacciHeap <Type> :: Reinsert(Node *firstChild, int degree){
	Node *temp = firstChild;
	for(int i = 0; i< degree; i++){
		temp->parent = NULL;
		FibonacciHeap *p = new (std::nothrow) FibonacciHeap();
		if (p == NULL){
			cout << "ERROR : Not enough memory. EXIT !!!\n";
			exit(1);
		}
		p->minNode = temp;
		Meld(this, p);
		temp = temp->rsibling;
	}
}

//cascading cut when performing remove and decrease key
template <class Type>
void FibonacciHeap <Type> :: CascadingCut(Node *curNode){
	//from curNode to root, if childCut=true, cut the node
	while(curNode->childCut == true && curNode != NULL){
		Node *parent = curNode->parent;
		CutSubTree(curNode);
		Reinsert(curNode, 1);
		curNode = parent;
	}
	curNode->childCut = true;
}

//find the second minimal node in the heap
template <class Type>
void FibonacciHeap <Type> :: FindNextMin(Node* aNode){
	Node *iter = aNode;
	minNode = aNode;
	do {
		if (iter->data < minNode->data) {
			minNode = iter;
		}
		iter = iter->rsibling;
	} while (iter != aNode);
}

//join two min tree when performing remove min
template <class Type>
void FibonacciHeap <Type> :: JoinMinTrees(Node *root1, Node *root2){
	if (NULL == root1 || NULL == root2) {
		std::cerr << "ERROR: Join non-exist min tree in Pairwise Combine\n";
		exit(1);
	}
	Node *prevTree = root2->lsibling;
	Node *nextTree = root1->rsibling;
	//root1 will be the new root
	if (root1->data <= root2->data) {
		//root1 become root2's parent
		root2->parent = root1;
		//insert root2 into root1's children list
		InsertIntoDoublyList(root2, root1->child);
		//root2 become root1's child
		root1->child = root2;
		//increment degree of root1
		root1->degree += 1;
		//update the top linked list
		root1->lsibling = prevTree;
		prevTree->rsibling = root1;
	} else {
		//root2 become root1's parent
		root1->parent = root2;
		//insert root1 into root2's children list
		InsertIntoDoublyList(root1, root2->child);
		root2->child = root1;
		//increment degree of root2
		root2->degree += 1;
		//update the top linked list
		root1->rsibling = nextTree;
		nextTree->lsibling = root1;
	}
}

template <class Type>
void FibonacciHeap <Type> :: PairwiseCombine(){
	//the max degree of trees in the heap is log2(n)
	unsigned int max_degree = static_cast<int>(log2(numItem)) + 1;
	//create the degree table
	std::vector<Node*> tree(max_degree, NULL);
	//traverse all trees in the heap
	Node *p = minNode;
	int degree = 0;
	do {
		//if there are two trees with the same degree in the heap
		for (degree = p->degree; tree[degree]; degree++) {
			//join two trees
			JoinMinTrees(p, tree[degree]);
			//update tree table
			tree[degree] = NULL;
		}
		tree[degree] = p;
		p = p->rsibling;	
	} while (p != minNode);
}


template <class Type>
void FibonacciHeap <Type> :: InsertIntoDoublyList(Node *insertMe, Node *besideMe) {
	insertMe->rsibling = besideMe;
	insertMe->lsibling = besideMe->lsibling;
	besideMe->lsibling->rsibling = insertMe;
	besideMe->lsibling = insertMe;
}
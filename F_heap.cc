#include "F_heap.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>

using namespace std;

template <class Type>
FibonacciHeap <Type> :: FibonacciHeap(){
	minNode = NULL;
	numItem = 0;
}

template <class Type>
FibonacciHeap <Type> :: ~FibonacciHeap (){

}

template <class Type> void
FibonacciHeap <Type> :: Insert (Type *item){
	Node *temp = new (std::nothrow) Node;
	if (temp == NULL){
		cout << "ERROR : Not enough memory. EXIT !!!\n";
		exit(1);
	}
	//consume the item
	temp->data = item;
	item = NULL;
	//if the heap is empty
	if(minNode == NULL){
		minNode = temp;
		minNode->lsibling = minNode;
		minNode->rsibling = minNode;
	}
	//if the heap is not empty, insert to top circular list
	else{
		temp->lsibling = minNode;
		temp->rsibling = minNode->rsibling;
		minNode->rsibling->lsibling = temp;
		minNode->rsibling = temp;
	}
	++numItem;
} 

template <class Type> int
FibonacciHeap <Type> :: RemoveMin (Type *item){
	if(minNode == NULL){
		return 0;
	}
	//store infomation of min node which will be used later
	int degree = minNode->degree;
	Node *firstChild = minNode->child;
	//remove the min tree
	RemoveMinTree(item);
	//reinsert subtree of min node
	Reinsert(firstChild, degree);
	//pairwise combine trees in heap and update heap pointer
	PairwiseCombine();
	return 1;
}

/*
template <class Type> void
FibonacciHeap <Type> :: RemoveMax (Type *item){

}
*/
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

//meld heap2 to heap1, heap2 is consumed after operation
template <class Type> void 
FibonacciHeap <Type> :: Meld (FibonacciHeap<Type> *heap1, FibonacciHeap<Type> *heap2){
	//if heap2 is empty, nothing to meld to heap1
	if(heap2->minNode == NULL){
		return;
	}
	//if heap1 is empty, copy heap2 to heap1
	else if(heap1->minNode == NULL){
		heap1->minNode = heap2->minNode;
		heap1->numItem = heap2->numItem;
		heap2 = NULL;
		return;
	}else{
		//update the heap pointer
		heap1->minNode->rsibling->lsibling = heap2->minNode;
		heap2->minNode->rsibling->lsibling = heap1->minNode;
		Node *temp = heap1->minNode->rsibling;
		heap1->minNode->rsibling = heap2->minNode->rsibling;
		heap2->minNode->rsibling = temp;
		heap1->minNode = (*(heap1->minNode->data) < *(heap2->minNode->data))? heap1->minNode : heap2->minNode;
		delete heap2;
		heap2 = NULL;
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
/*
template <class Type> void 
FibonacciHeap <Type> :: IncreaseKey (Node *incMe, Type amount){

}*/

//find the second minimal node in the heap
template <class Type>
void FibonacciHeap <Type> :: FindNextMin(Node* nextMin){
	Node *p = minNode->rsibling;
	nextMin = p;
	while(p!=minNode){
		if(*(p->data) < *(nextMin->data)){
			nextMin = p;
		}
		p = p->rsibling;
	}
}

//remove the tree rooted in min node and delete the min node
template <class Type>
void FibonacciHeap <Type> :: RemoveMinTree(Type *item){
	//consume data of minNode
	item = minNode->data;
	minNode->data = NULL;
	Node *nextMin;
	FindNextMin(nextMin);
	minNode->lsibling->rsibling = minNode->rsibling;
	minNode->rsibling->lsibling = minNode->lsibling;
	delete minNode;
	minNode = nextMin;
	--numItem;
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

//join two min tree when performing remove min
template <class Type>
void FibonacciHeap <Type> :: JoinMinTrees(Node *root1, Node *root2){
	if(root2 == NULL){
		return;
	}else if(root1 == NULL){
		root1 = root2;
		root2 = NULL;
	}else{
		root2->parent = root1;
		root2->rsibling = root1->child;
		root2->lsibling = root1->child->lsibling;
		root1->child->lsibling->rsibling = root2;
		root1->child->lsibling = root2;
		++root1->degree;
		root1->childCut = false;
		delete root2;
		root2 = NULL;
	}
}

template <class Type>
void FibonacciHeap <Type> :: PairwiseCombine(){
	//create the degree table
	vector<Node*> tree;
	//the max degree of trees in the heap is log2(n)
	int max_degree = log2(numItem);
	tree.reserve(max_degree);
	//initial tree table
	for(int i = 0; i < max_degree; ++i){
		tree.push_back(NULL);
	}
	//traverse all trees in the heap
	Node *p = minNode;
	while(1){
		p = p->rsibling;
		int degree;
		//if there are two trees with the same degree in the heap
		for(degree = p->degree; tree[degree]; degree++){
			//join two trees
			joinMinTrees(p, tree[degree]);
			//update tree table
			tree[degree] = NULL;
		}
		tree[degree] = p;
		if(p == minNode){
			break;
		}
	}
	//traverse all trees in the heap, update heap pointer
	Node *prev = minNode;
	for(int i = 0; i < max_degree; ++i){
		if(tree[i]!=NULL){
			prev->rsibling = tree[i];
			tree[i]->lsibling = prev;
			prev = tree[i];
		}
	}
	prev->rsibling = minNode;
	minNode->lsibling = prev;
}

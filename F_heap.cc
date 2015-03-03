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
	temp->data = item;
	item = NULL;
	if(minNode == NULL){
		minNode = temp;
		minNode->lsibling = minNode;
		minNode->rsibling = minNode;
	}else{
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
	if(rmMe == minNode){
		Type dummy;
		removeMin(&dummy);
	}else{
		Node *parent = rmMe->parent;
		CutSubTree(rmMe);
		Reinsert(rmMe->child, rmMe->degree);
		delete rmMe;
		--numItem;
		CascadingCut(parent);
	}
}

template <class Type> void 
FibonacciHeap <Type> :: Meld (FibonacciHeap<Type> *heap1, FibonacciHeap<Type> *heap2){
	if(heap2->minNode == NULL){
		return;
	}else if(heap1->minNode == NULL){
		heap1->minNode == heap2->minNode;
	}else{
		heap1->minNode->rsibling->lsibling = heap2->minNode;
		heap2->minNode->rsibling->lsibling = heap1->minNode;
		Node *temp = heap1->minNode->rsibling;
		heap1->minNode->rsibling = heap2->minNode->rsibling;
		heap2->minNode->rsibling = temp;
		heap1->minNode = (*(heap1->minNode->data) < *(heap2->minNode->data))? heap1->minNode : heap2->minNode;
	}
	delete heap2;
}

template <class Type> void 
FibonacciHeap <Type> :: DecreaseKey (Node *decMe, Type amount){
	if(decMe->parent == NULL){
		return;
	}
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

template <class Type>
void FibonacciHeap <Type> :: RemoveMinTree(Type *item){
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

template <class Type>
void FibonacciHeap <Type> :: CutSubTree(Node *rootIn){
	rootIn->lsibling->rsibling = rootIn->rsibling;
	rootIn->rsibling->lsibling = rootIn->lsibling;
	rootIn->lsibling = rootIn;
	rootIn->rsibling = rootIn;
	--rootIn->parent->degree;
	rootIn->parent = NULL;
}

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

template <class Type>
void FibonacciHeap <Type> :: CascadingCut(Node *curNode){
	while(curNode->childCut == true && curNode != NULL){
		Node *parent = curNode->parent;
		CutSubTree(curNode);
		Reinsert(curNode, 1);
		curNode = parent;
	}
	curNode->childCut = true;
}


template <class Type>
void FibonacciHeap <Type> :: JoinMinTrees(Node *root1, Node *root2){
	if(root2 == NULL){
		return;
	}else if(root1 == NULL){
		root1 = root2;
	}else{
		root2->parent = root1;
		root2->rsibling = root1->child;
		root2->lsibling = root1->child->lsibling;
		root1->child->lsibling->rsibling = root2;
		root1->child->lsibling = root2;
		++root1->degree;
		root1->childCut = false;
	}
}

template <class Type>
void FibonacciHeap <Type> :: PairwiseCombine(){
	//create the degree table
	vector<Node*> tree;
	int max_degree = log2(numItem);
	tree.reserve(max_degree);
	for(int i = 0; i < max_degree; ++i){
		tree.push_back(NULL);
	}
	//traverse all trees in the heap
	Node *p = this->minNode;
	while(1){
		p = p->rsibling;
		int degree;
		for(degree = p->degree; tree[degree]; degree++){
			joinMinTrees(p, tree[degree]);
			tree[degree] = NULL;
		}
		tree[degree] = p;
		if(p == this->minNode){
			break;
		}
	}
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

#include "Trie.h"

BinaryTrie::~BinaryTrie() {
	delete root;
}

void BinaryTrie::insert (DataPair &data) {
	TNode *temp = new TNode();
	temp->flag = false;
	temp->data = std::move(data);

	TNode *x = root;
	// if the trie is empty, add the element as new root
	if (nullptr == x) {
		root = temp;
		return;
	} 
	TNode *y = x;
	// scan from high to low
	int pos = BITWIDTH - 1;
	while (false != x->flag) {
		// pointer to record the parent node
		y = x;
		if (temp->data.first[pos]) {
			// if current branch node has no right child
			if (nullptr == x->rchild) {
				// add a new element node on the right
				x->rchild = temp;
				return;
			}
			x = x->rchild;
			--pos;
		} else {
			// if current branch node has no left child
			if (nullptr == x->lchild) {
				// add a new element node on the left
				x->lchild = temp;
				return;
			}
			x = x->lchild;
			--pos;
		}
	}
	// if only one node in the trie, add new branch node as root
	if (BITWIDTH - 1 == pos) {
		root = new TNode();
		y = root;
	} else {
		// move back one bit for building branch node
		pos++;
	}
	// if the key is not equal to the element node
	if (data.first != x->data.first) {
		// add branch node until find the first unequal bit
		while (temp->data.first[pos] == x->data.first[pos]) {
			//if current bit is 1, add branch node on the right
			if (temp->data.first[pos]) {
				y->rchild = new TNode();
				y = y->rchild;
			} 
			//if current bit is 1, add branch node on the left
			else {
				y->lchild = new TNode();
				y = y->lchild;
			}
			--pos;
		}
		// add two element node to the children filed of newest branch node
		if (temp->data.first[pos]) {
			y->rchild = temp;
			y->lchild = x;
		} else {
			y->lchild = temp;
			y->rchild = x;
		}
	} 
	// otherwise update the element
	else {
		x->data.second = data.second;
	}
}


void BinaryTrie::remove (IpAddr ip) {
	TNode *x = root;	
	if (nullptr == x) {
		return;
	}
	//store the first redudant branch node
	TNode *y, *p;
	int pos = BITWIDTH - 1;
	bool side;
	// keep going down until meet a element node
	while (false != x->flag) {
		// pointer to record the parent node
		y = x;
		//if current bit is 1, go to the right
		if (ip[pos]) {
			// if fall to an empty node, the key doesn't exist
			if (nullptr == x->rchild) {
				return;
			}
			if (nullptr != x->lchild && false != x->rchild->flag) {
				//store the parent of first redundant branch node
				p = x;
				//store which side the first redundant branch node belong
				side = RIGHT;
			}
			x = x->rchild;
			--pos;
		}
		// if current bit is 0, go to the left
		else {
			// if fall to an empty node, the key doesn't exist
			if (nullptr == x->lchild) {
				return;
			}
			if (nullptr != x->rchild && false != x->lchild->flag) {
				//store the parent of first redundant branch node
				p = x;
				//store which side the first redundant branch node belong
				side = LEFT;
			}
			x = x->lchild;
			--pos;
		}
	}
	// if fall to an element node, check equality
	if(ip == x->data.first) {
		// delete the element node
		delete x;
		// store another node of the deleted node's parent
		TNode *temp = nullptr;
		if(ip[pos+1]) {
			temp = y->lchild;
			y->lchild = nullptr;
			y->rchild = nullptr;
		} else {
			temp = y->rchild;
			y->rchild = nullptr;
			y->lchild = nullptr;
		}
		//delete redundant branch node
		// shrink another child to right position
		if (LEFT == side) {
			delete p->lchild;
			p->lchild = temp;
			temp = nullptr;
		} else {
			delete p->rchild;
			p->rchild = temp;
			temp = nullptr;
		}		
	} 
}

long BinaryTrie::find (const IpAddr ip) const {
	TNode *x = root;
	if (nullptr == x) {
		return -1;
	}
	int pos = BITWIDTH - 1;
	// keep going down until meet a element node
	while (false != x->flag) {
		//if current bit is 1, go to the right
		if (ip[pos]) {
			// if fall to an empty node, the key doesn't exist
			if (nullptr == x->rchild) {
				return -1;
			}
			x = x->rchild;
			--pos;
		}
		// if current bit is 0, go to the left
		else {
			// if fall to an empty node, the key doesn't exist
			if (nullptr == x->lchild) {
				return -1;
			}
			x = x->lchild;
			--pos;
		}
	}
	// if fall to an element node, check equality
	if(ip == x->data.first) {
		return x->data.second;
	} else {
		return -1;
	}
}





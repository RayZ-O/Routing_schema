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
	int pos = BITWIDTH - 1;
	TNode *y = x;
	// scan from high to low	
	while (false != x->flag) {
		// pointer to record the parent node
		y = x;
		if (temp->data.first[pos]) {
			// if current branch node has no right child
			if (nullptr == x->rchild) {
				temp->position = pos;
				// add a new element node on the right
				x->rchild = temp;
				return;
			}
			x = x->rchild;
			--pos;
		} else {
			// if current branch node has no left child
			if (nullptr == x->lchild) {
				temp->position = pos;
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
		y->rchild->position = pos;
		y->lchild->position = pos;
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
	int pos = BITWIDTH - 1, ppos;
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
				ppos = pos;
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
				ppos = pos;
			}
			x = x->lchild;
			--pos;
		}
	}
	// if fall to an element node, check equality
	if(ip == x->data.first) {
		// delete the element node
		delete x;
		// if only one node in the trie
		if (BITWIDTH - 1 == pos) {
			root = nullptr;
			return;
		}
		// if only two node in the trie 
		else if (BITWIDTH - 2 == pos) {
			// determine which child to keep
			if (ip[++pos]) {
				root = y->lchild;
				y->lchild = nullptr;
				y->rchild = nullptr;
			} else {
				root = y->rchild;
				y->lchild = nullptr;
				y->rchild = nullptr;
			}
			root->position = pos + 1; 
			delete y;	
		}
		else {
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
			temp->position = ppos;
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
		// cout << x->position << '\t';
		return x->data.second;
	} else {
		return -1;
	}
}


void BinaryTrie::postorder_traversal (TNode *node) {
	if (nullptr == node) {
		return;
	}
	postorder_traversal(node->lchild);
	postorder_traversal(node->rchild);
	TNode* left = node->lchild;
	TNode* right = node->rchild;
	if (nullptr != left && nullptr != right && false == left->flag && false == right->flag) {
		if (left->data.second == right->data.second) {
			remove(right->data.first);
		}
	}	
}

int BinaryTrie::get_prefix(const IpAddr ip, int verID, Prefix &pf) {
	postorder_traversal(root);
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
			pf.push_back(1);
			x = x->rchild;
			--pos;
		}
		// if current bit is 0, go to the left
		else {
			// if fall to an empty node, the key doesn't exist
			if (nullptr == x->lchild) {
				return -1;
			}
			pf.push_back(0);
			x = x->lchild;
			--pos;
		}
	}
	// if fall to an element node, check equality
	if(verID == x->data.second) {
		return 0;
	} else {
		return -1;
	}
}

void BinaryTrie::clear() {
	delete root;
	root = nullptr;
}
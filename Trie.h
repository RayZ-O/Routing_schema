#ifndef _TRIE_H
#define _TRIE_H

#include <bitset>
#include <iostream>
#include <utility>
#include <string>

using std::cout;
using std::endl;

#define BITWIDTH 4
#define LEFT 0
#define RIGHT 1

typedef std::bitset<BITWIDTH> IpAddr;
typedef std::pair<std::bitset<BITWIDTH>, long> DataPair;

struct TNode {
	bool flag;
	DataPair data;
	TNode *lchild;
	TNode *rchild;
	int h;
	TNode() : flag(true), data(), lchild(nullptr), rchild(nullptr) { }
	~TNode() {
		if(nullptr != lchild) {
			delete lchild;
			lchild = nullptr;
		} else if (nullptr != rchild) {
			delete rchild;
			rchild = nullptr;
		}
	}
};

class BinaryTrie {
private:
	TNode *root;
public:
	BinaryTrie() : root(nullptr) { }
	BinaryTrie(BinaryTrie const &copyMe) = delete;
	BinaryTrie& operator = (BinaryTrie const &copyMe) = delete;
	~BinaryTrie();
	void insert(DataPair &elem);
	void remove(IpAddr ip);
	long find(const IpAddr ip) const;
	void postorder_traversal();
};

#endif

#ifndef _TRIE_H
#define _TRIE_H

#include <stdlib.h>
#include <bitset>
#include <iostream>
#include <utility>
#include <string>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;

#define BITWIDTH 32
#define LEFT 0
#define RIGHT 1

typedef std::vector<bool> Prefix;
typedef std::bitset<BITWIDTH> IpAddr;
typedef std::pair<IpAddr, long> DataPair;

struct TNode {
	bool flag;
	DataPair data;
	TNode *lchild;
	TNode *rchild;
	int position;
	TNode() : flag(true), data(), lchild(nullptr), rchild(nullptr), position(BITWIDTH) { }
	TNode(const TNode &copyMe) = delete;
	TNode& operator = (const TNode &copyMe) = delete;
	~TNode() {
		delete lchild;
		delete rchild;
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
	void postorder_traversal(TNode *node);
	int get_prefix(const IpAddr ip, int verID, Prefix &pf);
	void clear();
};

#endif

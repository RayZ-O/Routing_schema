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
#define SUCCESS 0
#define FAILED -1

typedef std::vector<bool> Prefix;
typedef std::bitset<BITWIDTH> IpAddr;
typedef std::pair<IpAddr, long> DataPair;

class TrieNode {
friend class BinaryTrie;
private:
	bool flag;
	DataPair data;
	TrieNode *lchild;
	TrieNode *rchild;
public:
	TrieNode() : flag(true), data(), lchild(nullptr), rchild(nullptr) { }
	TrieNode(const TrieNode &copyMe) = delete;
	TrieNode& operator = (const TrieNode &copyMe) = delete;
	//recursively delete left and right child
	~TrieNode() {
		delete lchild;
		lchild = nullptr;
		delete rchild;
		rchild = nullptr;
	}
};

class BinaryTrie {
private:
	TrieNode *root;
	//postorder traversal the trie and delete subtries with the same next hop
	void postorder_traversal(TrieNode *node);
public:
	BinaryTrie() : root(nullptr) { }
	BinaryTrie(BinaryTrie const &copyMe) = delete;
	BinaryTrie& operator = (BinaryTrie const &copyMe) = delete;
	~BinaryTrie();
	//insert a ip, next hop pair into the trie
	void insert(DataPair &elem);
	//remove the node whose key is equal to ip
	void remove(IpAddr ip);
	//find the node whose key is equal to ip
	long find(const IpAddr ip) const;
	//get the longest prefix of the given ip
	int get_prefix(const IpAddr ip, Prefix &prefix);
	//clear the trie
	void clear();
};

#endif

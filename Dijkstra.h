#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

#include "FibonacciHeap.h"
#include <string>
#include <fstream>
#include <algorithm>

typedef struct _adjacent {
	int adjID;
	int weight;
}adjacent;

typedef struct vertex {
	std::vector<adjacent> adjacent_list;
	long minWeight;
	int previous;
	Node* myNode;
	vertex() : minWeight(0), previous(-1), myNode(nullptr) { }
}Vertex;

class Graph {
private:
	long numVertices;
	long numEdges;
public:
	std::vector<Vertex> vertex_table;
	Graph ();
	void init_graph (std::string filename);
	void init_node (int i, Node *myNode);
	void set_previous(int verID, int previous);
	int get_num_vertices();
	Node* get_node(int i);
	void print();

	void shortest_path (int source, int destination);
	void relax (FibonacciHeap &fh, int verID);	
};


#endif

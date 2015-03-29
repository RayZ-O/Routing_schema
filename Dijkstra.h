#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

#include "FibonacciHeap.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <stack>

typedef std::pair<int, int> adjacent;

class Vertex {	
friend class Graph;
private:
	long minWeight;	//minimum weight to source 
	int previous;	//previous vertex on shortest path
	Node* myNode;	//corresponding node in the heap
	std::vector<adjacent> adjacent_list;	//adjacent list
public:
	Vertex();
	//delete default copy constructor and copy assignment
	Vertex(const Vertex & copyMe) = delete;
	Vertex & operator = (const Vertex & copyMe) = delete;
	//move constructor
	Vertex(Vertex && moveMe);
	//move assignment
	Vertex & operator = (Vertex && moveMe);
};

class Graph {
private:
	long numVertices;	//number of vertices
	long numEdges;		//number of edges
	std::vector<Vertex> vertex_table;	//vertex table
public:	
	Graph ();
	//delete default copy constructor and copy assignment
	Graph(const Graph & copyMe) = delete;
	Graph & operator = (const Graph &copyMe) = delete;
	//read and initialize graph from a file
	void init_graph (std::string filename);
	//initialize corresponding heap node for vertex i
	void init_node (int i, Node *myNode);
	//set previous vertex for vertex i
	void set_previous(int verID, int previous);
	//get corresponding heap node for vertex i
	Node* get_node(int i);
	//print the graph infomation
	void print();
	//find shoertest path using Dijkstra's algorithm
	void shortest_path (int source, int destination);
	//relax an edge in Dijkstra's algorithm
	void relax (FibonacciHeap &fh, int verID);	
	//print shoertest path and minimum weight
	void print_path(int source, int destination);
};


#endif

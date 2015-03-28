#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>

using std::cout;
using std::endl;

typedef std::pair<int, int> adjacent;
typedef std::vector<adjacent> adjacent_list;


class Graph {
private:
	long numVertices;
	long numEdges;
public:
	std::vector<adjacent_list> vertex_table;
	Graph ();
	void initGraph (std::string filename);
	void print();
};

#endif
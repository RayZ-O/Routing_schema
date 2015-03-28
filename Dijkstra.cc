#include "Dijkstra.h"

Graph::Graph () : numVertices(0), numEdges(0) { }

//read Graph from file
void Graph::initGraph (std::string filename) {
	std::ifstream input(filename.c_str());
	if (!input.is_open()) {
		std::cerr << "ERROR: Can not open input file!\n";
		exit(1);
	}
	input >> numVertices;
	input >> numEdges;
	vertex_table.reserve(numVertices);

	int verID, adjID, weight;
	adjacent adj;
	//init last ID
	while (input >> verID) {
		if (verID >= numVertices) {

		}	
		input >> adjID;
		input >> weight;
		adj = std::make_pair(adjID, weight);
		vertex_table[verID].push_back(adj);
		adj = std::make_pair(verID, weight);
		vertex_table[adjID].push_back(adj);
	}
	
}

void Graph::print() {
 	cout << "vertex number: " << numVertices << endl;
 	cout << "edge number: " << numEdges << endl;
 	for(int i = 0; i < numVertices; ++i){
 		cout << "verID: " << i << endl;
 		for(auto it= vertex_table[i].begin(); it != vertex_table[i].end(); ++it) {
 			cout << "adjID: " << (*it).first << "\tweight:" << (*it).second << endl;
 		}
 		cout << endl;
 	}
}

// DIJKSTRA (G, w, s)
// 1 INITIALIZE 
// 2 S = empty;
// 3 Q = G.V
// 4 while (Q!=empty) ;
// 5	u = EXTRACT-MIN(Q)
// 6	S = S U {u}
// 7	for each vertex v in G.Adj[u]
// 8		RELAX(u, v, w)




// RELAX(u, v, w)
// 1 if v.d > u.d + w(u, v)
// 2	v.d = u.d + w(u, v)
// 3	v.pai = u
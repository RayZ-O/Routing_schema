#include "Dijkstra.h"

Graph::Graph () : numVertices(0), numEdges(0) { }

//read Graph from file
void Graph::init_graph (std::string filename) {
	std::ifstream input(filename.c_str());
	if (!input.is_open()) {
		cerr << "ERROR: Can not open input file!\n";
		exit(1);
	}
	input >> numVertices;
	input >> numEdges;
	vertex_table.reserve(numVertices);

	int verID;
	adjacent adj;
	//init last ID
	while (input >> verID) {
		if (verID >= numVertices) {
			cerr << "ERROR: Number of vertices exceed the number specified at the beginning of intput file\n";
			exit(1);
		}	
		input >> adj.adjID;
		input >> adj.weight;
		vertex_table[verID].adjacent_list.push_back(adj);
		vertex_table[adj.adjID].adjacent_list.push_back(adj);
	}	
}

void Graph::init_node (int verID, Node *myNode){
	if (nullptr == myNode) {
		cerr << "Null pointer exception in init_node\n";
		exit(1);
	}
	vertex_table[verID].myNode = myNode;
}

void Graph::set_previous(int verID, int previous) {
	vertex_table[verID].previous = previous;
}

Node* Graph::get_node(int verID) {
	if (nullptr == vertex_table[verID].myNode) {
		cerr << "Null pointer exception in get_node\n";
		exit(1);
	}
	return vertex_table[verID].myNode;
}

int Graph::get_num_vertices () {
	return numVertices;
}

void Graph::print() {
 	cout << "vertex number: " << numVertices << endl;
 	cout << "edge number: " << numEdges << endl;
 	for(int i = 0; i < numVertices; ++i){
 		cout << "verID: " << i << endl;
 		for(auto it= vertex_table[i].adjacent_list.begin(); it != vertex_table[i].adjacent_list.end(); ++it) {
 			cout << "adjID: " << (*it).adjID << "\tweight:" << (*it).weight << endl;
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
void Graph::shortest_path (int source, int destination) {
	FibonacciHeap fh;
	constexpr long infinite = std::numeric_limits<long>::max();
	for(int i = 0; i < get_num_vertices(); i++){
		vertex_table[i].minWeight = infinite;
		vertex_table[i].previous = i;
		init_node(i, fh.insert(infinite, i));
	}
	fh.decrease_key(get_node(source), 0);
	vertex_table[source].minWeight = 0;
	int minID;
	while(fh.size() > 0) {
		fh.remove_min(minID);
		cerr << minID << "\t";
		vertex_table[minID].myNode = nullptr;
		relax(fh, minID);
	}

	cout << "Minimum weight is " << vertex_table[destination].minWeight << endl;
	std::vector<int> path;
	path.reserve(numVertices);
	int p = destination;
	do {		
		path.push_back(p);
		p = vertex_table[p].previous;
	} while (p != source);
	path.push_back(source);
	std::reverse(path.begin(), path.end());
	cout << "Shortest path is \n";
	for(auto it = path.begin(); it != path.end(); ++it){
		cout << *it << " ";
	}
	cout << endl;
}
	

// RELAX(u, v, w)
// 1 if v.d > u.d + w(u, v)
// 2	v.d = u.d + w(u, v)
// 3	v.pai = u
void Graph::relax (FibonacciHeap &fh, int verID) {
	for (auto it = vertex_table[verID].adjacent_list.begin(); it != vertex_table[verID].adjacent_list.end(); ++it) {
		int adjID = it->adjID;
		int weight = it->weight;
		int newWeight = vertex_table[verID].minWeight + weight;

		if (vertex_table[adjID].minWeight > newWeight) {
			vertex_table[adjID].minWeight = newWeight;
			fh.decrease_key(get_node(adjID), newWeight);
			set_previous(adjID, verID);
		}
	}
}

#include "Dijkstra.h"

Vertex::Vertex() : minWeight(-1), previous(-1), myNode(nullptr) { }

//move constructor
Vertex::Vertex (Vertex && moveMe) : minWeight(moveMe.minWeight), previous(moveMe.previous), myNode(moveMe.myNode) {
	moveMe.myNode = nullptr;
	adjacent_list = std::move(moveMe.adjacent_list);
}

//move assignment
Vertex& Vertex::operator = (Vertex && moveMe){
	minWeight = moveMe.minWeight;
	previous = moveMe.previous;
	myNode = moveMe.myNode;
	moveMe.myNode = nullptr;
	adjacent_list = std::move(moveMe.adjacent_list);
	return *this;
}

Graph::Graph () : numVertices(0), numEdges(0) { }

//read and initialize graph from a file
void Graph::init_graph (std::string filename) {
	std::ifstream input(filename.c_str());
	if (!input.is_open()) {
		cerr << "ERROR: Can not open input file!\n";
		exit(1);
	}
	input >> numVertices;
	input >> numEdges;
	vertex_table.reserve(numVertices);
	// initialize vertex table
	for(int i = 0; i < numVertices; ++i) {
		vertex_table.push_back(std::move(Vertex()));
	}
	int verID, adjID, weight;
	adjacent adj;
	//read graph infomation from the file
	while (input >> verID) {
		if (verID >= numVertices) {
			cerr << "ERROR: Number of vertices exceed the number specified at the beginning of intput file\n";
			exit(1);
		}	
		input >> adjID;
		input >> weight;
		//construct adjacent vertex pair
		adj = std::make_pair(adjID, weight);
		vertex_table[verID].adjacent_list.push_back(std::move(adj));
		//construct pair for adjacent vertex 
		adj = std::make_pair(verID, weight);
		vertex_table[adjID].adjacent_list.push_back(std::move(adj));
	}	
}

//initialize corresponding heap node for vertex i
void Graph::init_node (int verID, Node *myNode){
	if (nullptr == myNode) {
		cerr << "Null pointer exception in init_node\n";
		exit(1);
	}
	vertex_table[verID].myNode = myNode;
}

//set previous vertex for vertex i
void Graph::set_previous(int verID, int previous) {
	vertex_table[verID].previous = previous;
}

//get corresponding heap node for vertex i
Node* Graph::get_node(int verID) {
	if (nullptr == vertex_table[verID].myNode) {
		cerr << "Null pointer exception in get_node\n";
		exit(1);
	}
	return vertex_table[verID].myNode;
}

//print the graph infomation
void Graph::print() {
 	cout << "vertex number: " << numVertices << endl;
 	cout << "edge number: " << numEdges << endl;
 	for(int i = 0; i < numVertices; ++i){
 		cout << "verID: " << i << endl;
 		for(auto it= vertex_table[i].adjacent_list.begin(); it != vertex_table[i].adjacent_list.end(); ++it) {
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
void Graph::shortest_path (int source, int destination) {
	FibonacciHeap fh;
	constexpr long infinite = std::numeric_limits<long>::max();
	//initialize the corresponding fibonacci heap
	for(int i = 0; i < numVertices; i++){
		vertex_table[i].minWeight = infinite;
		vertex_table[i].previous = -1;
		init_node(i, fh.insert(infinite, i));
	}
	//decreas the key of source to 0
	fh.decrease_key(get_node(source), 0);
	vertex_table[source].minWeight = 0;
	int minID;
	while(fh.size() > 0) {
		//remove minimum element in the heap
		fh.remove_min(minID);
		//if the min node is destination, break the loop
		if (destination == minID) {
			break;
		}
		vertex_table[minID].myNode = nullptr;
		//relax adjacent list of min node
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
		int adjID = it->first;
		int weight = it->second;
		int newWeight = vertex_table[verID].minWeight + weight;
		//if the new weight less than old weight
		if (vertex_table[adjID].minWeight > newWeight) {
			vertex_table[adjID].minWeight = newWeight;
			//decrease correspond node in the heap
			fh.decrease_key(get_node(adjID), newWeight);
			// update previous vertex
			set_previous(adjID, verID);
		}
	}
}

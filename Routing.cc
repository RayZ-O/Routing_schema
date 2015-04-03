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
void Graph::init_graph (std::string filename1, std::string filename2) {
	std::ifstream input1(filename1.c_str());
	std::ifstream input2(filename2.c_str());
	if (!input1.is_open() || !input2.is_open()) {
		cerr << "ERROR: Can not open input file!\n";
		exit(1);
	}
	input1 >> numVertices;
	input1 >> numEdges;
	if (numVertices <= 0 && numEdges <0) {
		cerr << "ERROR: Invalid input file!\n";
		exit(1);
	}
	vertex_table.reserve(numVertices);
	// initialize vertex table
	for(int i = 0; i < numVertices; ++i) {
		vertex_table.push_back(std::move(Vertex()));
	}
	long verID, adjID;
	int weight;
	adjacent adj;
	//read graph infomation from the file
	while (input1 >> verID) {		
		input1 >> adjID;
		input1 >> weight;
		if (verID < 0 || verID >= numVertices || adjID < 0 || adjID >= numVertices || weight < 0) {
			cerr << "ERROR: Invalid input file!\n";
			exit(1);
		}	
		//construct adjacent vertex pair
		adj = std::make_pair(adjID, weight);
		vertex_table[verID].adjacent_list.push_back(std::move(adj));
		//construct pair for adjacent vertex 
		adj = std::make_pair(verID, weight);
		vertex_table[adjID].adjacent_list.push_back(std::move(adj));
	}	
	// initialize ip table
	ip_table.reserve(numVertices);
	std::string ipstr;
	for (int i = 0; input2 >> ipstr; i++) {
		IpAddr temp;
		ip_to_bit(ipstr, temp);
		ip_table.push_back(temp);
	}

	for(int i = 0; i<numVertices; ++i) {
		cout << ip_table[i] << endl;
	}
}

void Graph::ip_to_bit(const std::string ip, IpAddr &bits) {
	int beg = 0;
	for(unsigned int i = 0; i < ip.size(); ++i) {
		if ('.' == ip[i]) {
			std::string str(ip, beg, i - beg);
			bits |= IpAddr(std::stoi(str));
			bits <<= 8;
			beg = i + 1;
		} 
	}
	std::string str(ip, beg, ip.size() - beg);
	bits |= IpAddr(std::stoi(str));
}
//initialize corresponding heap node for vertex i
void Graph::init_node (long verID, Node *myNode){
	if (nullptr == myNode) {
		cerr << "Null pointer exception in init_node\n";
		exit(1);
	}
	vertex_table[verID].myNode = myNode;
}

//set previous vertex for vertex i
void Graph::set_previous(long verID, long previous) {
	vertex_table[verID].previous = previous;
}

//get corresponding heap node for vertex i
Node* Graph::get_node(long verID) {
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
void Graph::shortest_path (long source, long destination) {
	//if the source or destination is not in the gragh
	if (source < 0 || source >= numVertices) {
		cerr << "Please input valid source\n" << "valid vertex range: [0-" << numVertices-1 << "]  input: " << source <<"\n";		
		exit(1);
	} else if (destination < 0 || destination >= numVertices) {
		cerr << "Please input valid destination\n" << "valid vertex range: [0-" << numVertices-1 << "]  input: " << destination <<"\n";
		exit(1);
	}
	FibonacciHeap fh;
	constexpr long infinity = std::numeric_limits<long>::max();
	//initialize the corresponding fibonacci heap
	for(int i = 0; i < numVertices; i++){
		vertex_table[i].minWeight = infinity;
		vertex_table[i].previous = -1;
		init_node(i, fh.insert(infinity, i));
	}
	//decreas the key of source to 0
	fh.decrease_key(get_node(source), 0);
	vertex_table[source].minWeight = 0;
	long minID;
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
	print_path(source, destination);
}
	

// RELAX(u, v, w)
// 1 if v.d > u.d + w(u, v)
// 2	v.d = u.d + w(u, v)
// 3	v.pai = u
void Graph::relax (FibonacciHeap &fh, long verID) {
	for (auto it = vertex_table[verID].adjacent_list.begin(); it != vertex_table[verID].adjacent_list.end(); ++it) {
		long adjID = it->first;
		int weight = it->second;
		long newWeight = vertex_table[verID].minWeight + weight;
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

void Graph::print_path(long source, long destination) {
	constexpr long infinity = std::numeric_limits<long>::max();
	// if the minimum weight is infinity
	if (infinity == vertex_table[destination].minWeight) {
		cout << "There's no path between vertex " << source << " and vertex " << destination << endl;
	} else {
		//print minimum weight 
		cout << vertex_table[destination].minWeight << endl;
		std::stack<long> path;
		//push the path into a stack
		long p = destination;
		do {		
			path.push(p);
			p = vertex_table[p].previous;
		} while (p != source);
		path.push(source);
		//pop and print each vertex on the path
		while (!path.empty()) {
			cout << path.top() << " ";
			path.pop();
		}
		cout << "\n";
	}	
}

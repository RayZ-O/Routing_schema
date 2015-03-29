#include "Dijkstra.h"

int main (int argc, char *argv[]) {
	Graph G;
	// std::string filename;
	// int source, destination;
	// if(argc != 4) {
	// 	cerr << "ERROR: Please input file name, source node and destination node\n";
	// 	exit(1);
	// }
	// filename = argv[1];
	// source = atoi(argv[2]);
	// destination = atoi(argv[3]);

	// G.init_graph(filename);
	// G.shortest_path(source, destination);
	G.init_graph("input_5000_1_part1.txt");
	//G.init_graph("sample_input_part1.txt");
	//G.shortest_path(0, 999);
	G.shortest_path(0, 4999);
}

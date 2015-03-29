#include "Dijkstra.h"

int main (int argc, char *argv[]) {	
	std::string filename;
	int source, destination;
	if(argc != 4) {
		cerr << "ERROR: Invalid input\n" << "valid format: $ssp[file name][source][destination]\n\n";
		exit(1);
	}
	filename = argv[1];
	source = atoi(argv[2]);
	destination = atoi(argv[3]);

	Graph G;
	G.init_graph(filename);
	G.shortest_path(source, destination);
}

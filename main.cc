#include "Routing.h"

int main (int argc, char *argv[]) {	
	std::string filename1, filename2;
	int source, destination;
	if(argc != 5) {
		cerr << "ERROR: Invalid input\n" << "valid format: $ssp[file name][source][destination]\n\n";
		exit(1);
	}
	filename1 = argv[1];
	filename2 = argv[2];
	source = atoi(argv[3]);
	destination = atoi(argv[4]);	
	Graph G;
	G.init_graph(filename1, filename2);
	G.print_prefix_path(source, destination);

}

#include <iostream>
#include <fstream>
#include <string>

#include "graph_runner.h"
#include "graph.h"
int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    Graph* graph = Graph::fromStream(inputFile);
    graph->printToStream(std::cout);

    return 0;
}

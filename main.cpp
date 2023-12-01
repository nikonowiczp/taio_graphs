#include <iostream>
#include <fstream>
#include <string>

#include "graph_runner.h"
#include "graph.h"
int main(int argc, char* argv[]){
    char* input;
    if (argc != 2) {
        input = "./test_graph.txt";
        //std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        //return 1;
    }
    else {
        input = argv[1];
    }

    std::ifstream inputFile(input);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    Graph* graph = Graph::fromStream(inputFile);
    graph->printToStream(std::cout);
    GraphRunner runner(std::cout);

    runner.graphSize(graph);
    runner.graphMetric(graph);
    runner.maxClique(graph);
    runner.maxSubgraph({graph});

    std::cout << "Program is finished";
    return 0;
}

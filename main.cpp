#include <iostream>
#include <fstream>
#include <string>

#include "graph_runner.h"
#include "max_clique_approx.h"
#include "graph.h"
int main(int argc, char* argv[]){
    char* input1;
    char* input2;
    if (argc != 2) {
        input1 = "./../test_graphA.txt";
        input2 = "./../test_graphB.txt";
        //std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        //return 1;
    }
    else {
        input1 = argv[1];
    }

    std::ifstream inputFile(input1);
    std::ifstream inputFile2(input2);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    Graph* graph = Graph::fromStream(inputFile);
    Graph* graph2 = Graph::fromStream(inputFile2);
    graph->printToStream(std::cout);
    graph2->printToStream(std::cout);
    GraphRunner runner(std::cout);
    MaxCliqueApprox approx(std::cout);
    runner.graphSize(graph);
    auto a = runner.maxClique(graph);
    std::cout<< "max clique: ";
    for (auto i: a)
        std::cout << i << ' ';
    std::cout << std::endl;
    auto sub = runner.maxSubgraph({graph,graph2});
    std::cout<< "max subgraph: ";
    for (auto i: sub)
        std::cout << i << ' ';
    std::cout << std::endl;
    std::cout << "Distance: "<< runner.graphMetric(sub, graph->size, graph2->size);
    //auto clique = runner.maxClique(graph);

    runner.maxSubgraph({graph});
    approx.maxCliqueApprox(graph);
    std::cout << "Program is finished";
    return 0;
}

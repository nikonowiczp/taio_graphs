#include <iostream>
#include <fstream>
#include <string>
#include "graph_runner.h"
#include "graph.h"

using std::cout;
using std::endl;
using std::string;
int main(int argc, char* argv[]){
    string input1;


    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    else {
        input1 = argv[1];
    }

    std::ifstream inputFile(input1);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << argv[1] << endl;
        return 1;
    }

    Graph* graph = Graph::fromStream(inputFile);
    graph->printToStream(cout);
    GraphRunner runner(cout);
    cout<<"Size 1: "<<runner.graphSize1(graph)<<endl;
    cout<<"Size 2: "<<runner.graphSize2(graph)<<endl;

    //auto a = runner.maxClique(graph);
    //cout<< "max clique: ";
    //for (auto i: a)
        //cout << i << ' ';
    //cout << endl;
    // auto sub = runner.maxSubgraph({graph,graph2});
    // cout<< "max subgraph: ";
    // for (auto i: sub)
    //     cout << i << ' ';
    // cout << endl;
    // cout << "Distance: "<< runner.graphMetric(sub, graph->size, graph2->size);
    //auto clique = runner.maxClique(graph);

    //runner.maxSubgraph({graph});
    //runner.maxCliqueApprox(graph);
    cout << "Program is finished";
    return 0;
}

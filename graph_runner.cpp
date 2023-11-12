#include "graph_runner.h"

GraphRunner::GraphRunner(std::ostream& output): output(output){
}
GraphRunner::~GraphRunner(){

}

void GraphRunner::graphSize(Graph* graph){
    output << "TODO: Graph size"<<std::endl;
}
void GraphRunner::graphMetric(Graph* graph){
    output << "TODO: Graph metric"<<std::endl;
}
void GraphRunner::maxClique(Graph* graph){
    output << "TODO: Graph max clique"<<std::endl;
}
void GraphRunner::maxSubgraph(std::vector<Graph* > graphs){
    output << "TODO: Graph max subgraph"<<std::endl;
}

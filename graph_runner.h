#include "graph.h"
#include <vector>
class GraphRunner{
public:
    GraphRunner(std::ostream& output);
    ~GraphRunner();

    void graphSize(Graph* graph);
    double graphMetric(std::vector<Graph* > graphs);
    double graphMetric(std::vector<int> subgraph, int n, int m);
    std::vector<int> maxClique(Graph* graph);
    std::vector<int> maxSubgraph(std::vector<Graph* > graphs);
private:
    std::ostream& output;
};

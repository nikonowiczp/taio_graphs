#include "graph.h"
#include <vector>

using std::vector;
class GraphRunner{
public:
    GraphRunner(std::ostream& output);
    ~GraphRunner();

    double graphSize1(Graph* graph);
    double graphSize2(Graph* graph);

    double graphMetric(std::vector<Graph* > graphs);
    double graphMetric(std::vector<int> subgraph, int n, int m);
    vector<int> maxClique(Graph* graph);
    vector<int> maxCliqueApprox(Graph* graph, int k = 0);

    vector<int> maxSubgraph(std::vector<Graph* > graphs);
    vector<int> maxSubgraphApprox(std::vector<Graph* > graphs);

private:
    std::ostream& output;
};

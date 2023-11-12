#include "graph.h"
#include <vector>
class GraphRunner{
public:
    GraphRunner(std::ostream& output);
    ~GraphRunner();

    void graphSize(Graph* graph);
    void graphMetric(Graph* graph);
    void maxClique(Graph* graph);
    void maxSubgraph(std::vector<Graph* > graphs);
private:
    std::ostream& output;
};

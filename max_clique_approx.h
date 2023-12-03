#include "graph.h"
#include <vector>

using std::vector;

class MaxCliqueApprox{
public:
    MaxCliqueApprox(std::ostream& output);
    ~MaxCliqueApprox();

    vector<int> maxCliqueApprox(Graph* graph, int k = 0);
private:
    vector<int> max_clique;
    int k = 0;
    std::ostream& output;
};

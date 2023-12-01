#include "graph_runner.h"
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;

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

// orders vertices by the rules of the algorithm 
void orderVertices(vector<int>& vector, int n, int ** matrix){
    std::vector<int> v (vector);
    std::vector<int> verticesCount(vector.size()),
        newOrder;
    for(int i = 0; i<v.size(); i++){
        for(int j = 0; j<v.size(); j++){
            if( matrix[v[i]][v[j]] != 0){
                verticesCount[i]++;
            }
        }
    }

    for(int i=0;i<vector.size(); i++){
        int smallest_index = std::distance(std::begin(verticesCount), std::min_element(std::begin(verticesCount), std::end(verticesCount)));

        newOrder.push_back(v[smallest_index]);
        v.erase(v.begin()+smallest_index);
        verticesCount.erase(verticesCount.begin() + smallest_index);

        for(int j = 0; j<verticesCount.size(); j ++){
            if(matrix[newOrder.back()][v[j]] != 0){
                verticesCount[j]--;
            }
        }
        
    }
    vector.swap(newOrder);
}
void addNextStep(vector<vector<int>> &values, vector<int> &steps, int **matrix){
    vector<int> newDepth;
    int nodeNumber = steps.back(), currentDepth = steps.size();

    int currentNode = values.back()[nodeNumber];
    for (auto value : values.back()){
        if(matrix[currentNode][value] != 0){
            newDepth.push_back(value);
        }
    }

    // TODO: DO THE SORTING
    
    values.push_back(newDepth);
}
void GraphRunner::maxClique(Graph* graph){
    auto g = Graph::fromGraph(graph);


    output << "Graph max clique Randy Carraghan"<<std::endl;
    int n = graph->size;

    //prune a graph
    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (g->matrix[i][j]!= 0 && g-> matrix[j][i] == 0){
                g-> matrix[i][j] = 0;
            }
        }
    }
    g->printToStream(cout);
    vector<int> defaultValues;
    for(int i = 0; i<n;i++) defaultValues.push_back(i);
    orderVertices(defaultValues, n, g->matrix);
    

    std::cout<< "Test the ordering"<<std::endl;
    for (int i = 0; i < n; i++){
        std::cout << " Original number: "<<defaultValues[i]+1<<std::endl; 
    }

    vector<vector<int>> values;
    values.push_back(defaultValues);
    int best_clique = 1;
    vector<int> steps = {1};
    // we try to find the clique with v_i in it.
    // depth - steps.size();
    // 
    while(true){
        if((steps.size() + values.back().size() - steps.back()) <= best_clique){
            // we prune
            if(steps.size() == 1){
                // we have the max clique
                break;
            }
            steps.pop_back();
            values.pop_back();
            steps.back()++;
        }

        addNextStep(values, steps, g->matrix);
    }
}
void GraphRunner::maxSubgraph(std::vector<Graph* > graphs){
    output << "TODO: Graph max subgraph"<<std::endl;
}

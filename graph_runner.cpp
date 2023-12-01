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
void orderVertices(vector<int>& vector, int ** matrix){
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
    int nodeNumber = steps.back();

    int currentNode = values.back()[nodeNumber];
    bool didFindValue = false;

    for (auto value : values.back()){
        cout<<"Checking "<<currentNode<<" "<<value;
        if(didFindValue && matrix[currentNode][value] != 0 ){
            newDepth.push_back(value);
            cout<<" success"<<endl;
        }else{
            cout<< " fail: d"<<endl;
        }
        if(currentNode == value)didFindValue = true;
    }

    cout<<endl;
    orderVertices(newDepth, matrix);
    values.push_back(newDepth);
    
}

void maxCliqueStep(vector<vector<int>> &values, vector<int> &steps,  int **matrix, vector<int>& best_clique){
    cout<<"Current values:"<<endl;
        for(auto val1: values){
            cout<<" ";
            for(auto val2: val1){
                cout<<val2+1<<" ";
            }
            cout<<endl;
        }
        cout<<"Current steps: ";
        for(auto val1: steps){
            cout<<val1<<" ";
        }
        cout<<endl;
        // we check if expanded. If not - we have the clique
    if(steps.size() > 0 && values.back().size() == 0){
        best_clique.clear();
        for(int i = 0; i<steps.size(); i++){
            best_clique.push_back(values[i][steps[i]]);
        }
        cout<<"Found best clique with size "<<best_clique.size()<<" and members: ";
        for(auto v:best_clique){
            cout<<v+1<<" ";
        } 
        cout<<endl; 
        return;
    }
    // we check if we do the next move

    for(int val = 0; val<values.back().size(); val++){
        steps.push_back(val);
        if((values.size() + values.back().size() - steps.back() - 1) <= best_clique.size()){
            steps.pop_back();
            cout<<"Pruning..."<<endl;
            return;
        }else{
            cout<<"Did not prune: "<<values.size() <<" "<<values.back().size()<<" "<<steps.back()<<" "<<best_clique.size()<<endl; 
        }
        addNextStep(values, steps, matrix);
        
        maxCliqueStep(values, steps, matrix, best_clique);
        values.pop_back();
        steps.pop_back();
    }
    return;
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
    orderVertices(defaultValues, g->matrix);
    

    std::cout<< "Test the ordering"<<std::endl;
    for (int i = 0; i < n; i++){
        std::cout << " Original number: "<<defaultValues[i]+1<<std::endl; 
    }

    vector<vector<int>> values;
    values.push_back(defaultValues);
    vector<int> steps, best_clique;
    // we try to find the clique with v_i in it.
    // depth - steps.size();
    // 
    maxCliqueStep(values, steps, g->matrix, best_clique);
}
void GraphRunner::maxSubgraph(std::vector<Graph* > graphs){
    output << "TODO: Graph max subgraph"<<std::endl;
}

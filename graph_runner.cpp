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

    //cout<<endl;
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
    if(steps.size() > best_clique.size() && steps.size() > 0 && values.back().size() == 0){
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
vector<int> GraphRunner::maxClique(Graph* graph){
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
    return best_clique;
}

bool checkCondition(int ** m1, int ** m2, int v, int w, int i, int j){
    if(v == i) return false;
    if(w == j) return false;
    if(m1[v][i] == m2[w][j] && m1[i][v] == m2[j][w]) return true;
    else return false;
}

Graph createModularProduct(Graph* G1, Graph* G2){
    int n = G1->size;
    int m = G2->size;
    Graph graph(n*m);
    for(int v=0;v<n;v++) {
        for (int w = 0;w<m;w++){
            int k = v*m + w;
            for(int i=0;i<n;i++){
                for(int j=0;j<m;j++){
                    int l = i*m+j;
                    if(checkCondition(G1->matrix, G2->matrix, v,w,i, j)){
                        graph.matrix[k][l] = 1;
                    }
                    else graph.matrix[k][l] = 0;
                }
            }
        }
    }
    return graph;
}

vector<int> encodeSubgraph(vector<int> clique, int n, int  m)
{
    vector<int> subgraph;
    for(auto i: clique)
    {
        int value = (int)i / m;
        subgraph.push_back(value);
    }
    return subgraph;
}

vector<int> GraphRunner::maxSubgraph(std::vector<Graph* > graphs){
    Graph modularProduct = createModularProduct(graphs[0],graphs[1]);
    return encodeSubgraph(maxClique(&modularProduct),graphs[0]->size,graphs[1]->size);
}

double GraphRunner::graphMetric(std::vector<Graph* > graphs){
    int max = graphs[0]->size > graphs[1]->size ? graphs[0]->size : graphs[1]->size;
    return 1 - maxSubgraph(graphs).size() / max;
}

double GraphRunner::graphMetric(std::vector<int> subgraph, int n, int m){
    int max = n > m ? n : m;
    return 1 - subgraph.size() / max;
}

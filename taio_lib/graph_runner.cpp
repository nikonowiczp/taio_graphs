#include "graph_runner.h"
#include <vector>
#include <algorithm>
#include <set>
#include <array>
using std::vector;
using std::cout;
using std::endl;
using std::set;

#define DEBUG 1
GraphRunner::GraphRunner(std::ostream& output): output(output){
}
GraphRunner::~GraphRunner(){

}

double GraphRunner::graphSize1(Graph* graph){
    int n = graph->size;
    double k = 0, new_n = 0;
    auto working_graphs = std::make_unique<int[]>(n);
    for(int i = 0; i < n; i++){
        working_graphs[i] = 0;
    }
    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if(graph->matrix[i][j] != 0){
                k++;
                working_graphs[i] = 1;
                working_graphs[j] = 1;
            }
        }
    }
    for(int i = 0; i < n; i++){
        if(working_graphs[i] == 1)new_n ++;
    }
    if(new_n == 0) return 0;
    return new_n + (k / new_n);
}
double GraphRunner::graphSize2(Graph* graph){
    int n = graph->size;
    if(n == 0) return n;
    set<int> *sets = new std::set<int>[n]; 
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(graph->matrix[i][j]){
                sets[j].insert(i);
            }
        }
    }
    bool changed = true;
    while(changed){
        if(DEBUG){
            cout<<"Snapshot of vertices: "<<endl;
        for(int i = 0; i < n; i++){
            cout<<" To vertice "<<i<<": ";
            for(auto a: sets[i]){
                cout<<a<<" ";
            }cout<<endl;
        }
        }
        changed = false;
        // for each vertice
        for(int i = 0; i < n; i++){
            // for each vertice that we already have for i
            for(auto a: sets[i]){
                // we try to insert
                for(auto b: sets[a]){
                    auto result = sets[i].insert(b);
                    if(result.second == true){
                        changed = true;
                    }
                }
            }
        }
    }
    double count = 0;
    for(int i = 0; i < n; i++){
        count += sets[i].size();
        if(sets[i].find(i) != sets[i].end()) count--;
    }
    
    return count/(double)n;
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
        if(DEBUG) cout<<"Checking "<<currentNode<<" "<<value;
        if(didFindValue && matrix[currentNode][value] != 0 ){
            newDepth.push_back(value);
            if(DEBUG) cout<<" success"<<endl;
        }else{
            if(DEBUG) cout<< " fail: d"<<endl;
        }
        if(currentNode == value)didFindValue = true;
    }

    //if(DEBUG) cout<<endl;
    orderVertices(newDepth, matrix);
    values.push_back(newDepth);
    
}

void maxCliqueStep(vector<vector<int>> &values, vector<int> &steps,  int **matrix, vector<int>& best_clique){
    if(DEBUG) cout<<"Current values:"<<endl;
        for(auto val1: values){
            if(DEBUG) cout<<" ";
            for(auto val2: val1){
                if(DEBUG) cout<<val2+1<<" ";
            }
            if(DEBUG) cout<<endl;
        }
        if(DEBUG) cout<<"Current steps: ";
        for(auto val1: steps){
            if(DEBUG) cout<<val1<<" ";
        }
        if(DEBUG) cout<<endl;
        // we check if expanded. If not - we have the clique
    if(steps.size() > best_clique.size() && steps.size() > 0 && values.back().size() == 0){
        best_clique.clear();
        for(int i = 0; i<steps.size(); i++){
            best_clique.push_back(values[i][steps[i]]);
        }
        if(DEBUG) cout<<"Found best clique with size "<<best_clique.size()<<" and members: ";
        for(auto v:best_clique){
            if(DEBUG) cout<<v+1<<" ";
        }
        if(DEBUG) cout<<endl;
        return;
    }
    // we check if we do the next move

    for(int val = 0; val<values.back().size(); val++){
        steps.push_back(val);
        if((values.size() + values.back().size() - steps.back() - 1) <= best_clique.size()){
            steps.pop_back();
            if(DEBUG) cout<<"Pruning..."<<endl;
            return;
        }else{
            if(DEBUG) cout<<"Did not prune: "<<values.size() <<" "<<values.back().size()<<" "<<steps.back()<<" "<<best_clique.size()<<endl;
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
    int n = graph->size;

    //prune a graph
    for(int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (g->matrix[i][j]!= 0 && g-> matrix[j][i] == 0){
                g-> matrix[i][j] = 0;
            }
        }
    }
    if(DEBUG)g->printToStream(cout);
    vector<int> defaultValues;
    for(int i = 0; i<n;i++) defaultValues.push_back(i);
    orderVertices(defaultValues, g->matrix);
    

    if(DEBUG) cout<< "Test the ordering"<<endl;
    for (int i = 0; i < n; i++){
        if(DEBUG) cout << " Original number: "<<defaultValues[i]+1<<endl;
    }

    vector<vector<int>> values;
    values.push_back(defaultValues);
    vector<int> steps, best_clique;
    // we try to find the clique with v_i in it.
    // depth - steps.size();
    // 
    maxCliqueStep(values, steps, g->matrix, best_clique);
    delete g;
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

vector<int> GraphRunner::maxSubgraph(vector<Graph* > graphs){
    Graph modularProduct = createModularProduct(graphs[0],graphs[1]);
    return encodeSubgraph(maxClique(&modularProduct),graphs[0]->size,graphs[1]->size);
}
vector<int> GraphRunner::maxSubgraphApprox(vector<Graph* > graphs){
    Graph modularProduct = createModularProduct(graphs[0],graphs[1]);
    return encodeSubgraph(maxCliqueApprox1(&modularProduct),graphs[0]->size,graphs[1]->size);
}
double GraphRunner::graphMetric(vector<Graph* > graphs){
    int max = graphs[0]->size > graphs[1]->size ? graphs[0]->size : graphs[1]->size;
    return 1 - maxSubgraph(graphs).size() / max;
}

double GraphRunner::graphMetric(vector<int> subgraph, int n, int m){
    int max = n > m ? n : m;
    return 1 - subgraph.size() / max;
}








vector<int> GraphRunner::maxCliqueApprox1(Graph* graph){
    if(DEBUG) cout<<"Max clique approx"<<endl;
    Graph * g = Graph::fromGraph(graph);
    int n = g->size;
    auto degrees = std::make_unique<int[]>(n);
    vector<int> subgraph(n);
    int highest_index = 0;
    vector<int> maximum_clique;
    
    for(int i = 0; i < n; i++){
        subgraph[i] = i;
        for (int j = 0; j < n; j++){
            if (g->matrix[i][j]!= 0 && g-> matrix[j][i] == 0){
                g-> matrix[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < n; i++){
        degrees[i] = 0;
        for(int j = 0; j < n; j++){
            if(g->matrix[i][j] != 0){
                degrees[i]++;
            }
        }
        if(degrees[i] > degrees[highest_index]){
            highest_index = i;
        }
    }
    while(subgraph.size() != 0){
        if(DEBUG) cout<<"Current maximum index: "<< highest_index <<endl;
        if(DEBUG) cout<<"Current degrees: ";
        for(int i =0; i< n; i++)if(DEBUG) cout<<degrees[i]<<" ";
        if(DEBUG) cout<<endl;
        maximum_clique.push_back(highest_index);
        vector<int> tmp;
        vector<int> rejected;

        for(auto vec: subgraph){
            if(g->matrix[highest_index][vec]){
                tmp.push_back(vec);
            }else{
                rejected.push_back(vec);
            }
        }
        if(tmp.size() == 0){
            break;
        }
        highest_index = tmp.front();
        for(auto vec1: tmp){
            for(auto vec2: rejected){
                if(g->matrix[vec1][vec2] != 0){
                    degrees[vec1] --;
                }
            }
            if(degrees[vec1] > degrees[highest_index] ){
                highest_index = vec1;
            }
        }
        subgraph = tmp;
    }
    if(DEBUG) cout<<"Found approx maximum clique: ";
    for(auto vec:maximum_clique)if(DEBUG) cout<<vec<<" ";
    if(DEBUG) cout<<endl;
    delete g;
    return maximum_clique;
}


vector<int> GraphRunner::maxCliqueApproxNewBestIn(Graph* graph, int k, int *degrees, vector<int> subgraph, vector<int> maximum_clique, vector<int> maximum_found){
    cout<<"Max clique approx new best in"<<endl;
    int n = graph->size;
    int highest_index = 0;
    
    for (int i = 0; i < n; i++){
        if(degrees[i] > degrees[highest_index]){
            highest_index = i;
        }
    }
    while(subgraph.size() != 0){
        if(maximum_clique.size() + subgraph.size() <= maximum_found.size()){
            if(DEBUG) cout<<"We pruned"<<endl;
            return maximum_found;
        }
        maximum_clique.push_back(highest_index);
        vector<int> tmp;
        vector<int> rejected;

        for(auto vec: subgraph){
            if(graph->matrix[highest_index][vec]){
                tmp.push_back(vec);
            }else{
                rejected.push_back(vec);
            }
        }
        if(tmp.size() == 0){
            break;
        }
        highest_index = tmp.front();
        for(auto vec1: tmp){
            for(auto vec2: rejected){
                if(graph->matrix[vec1][vec2] != 0){
                    degrees[vec1] --;
                }
            }
            if(degrees[vec1] > degrees[highest_index] ){
                highest_index = vec1;
            }
        }
        subgraph = tmp;
    }
    return maximum_clique;
}
vector<int> GraphRunner::maxCliqueApproxStep(Graph* graph, int *degrees, int k, int max_k, vector<int> subgraph, vector<int> maximum_clique, vector<int> maximum_found) {
    int n = graph->size, highest_index = 0;
    int* degrees_order = new int(n);
    for(int i = 0; i < n; i++){
        degrees_order[i] = degrees[i];
    }
    if(max_k == k){
        cout<<"Max_k = k. Trying maxClique approx new best in"<<endl;
        auto q = maxCliqueApproxNewBestIn(graph, k, degrees_order, subgraph, maximum_clique, maximum_found);
        cout<<"Returned from max clique best in"<<endl;
        cout<<"Found clique ";
        for(auto a:q)cout<<a<<" ";
        cout<<endl;
        return q;
    }

    while(true){
        cout<<"K = "<<k<<". Another loop"<<endl;
        int* degrees_local = new int(graph->size);
        for(int j = 0; j < n; j++){
            degrees_local[j] = degrees[j];
        }
        vector<int> subgraph_local(subgraph);
        vector<int> maximum_clique_local(maximum_clique);
        for(int j = 0 ; j < n; j++){
            if(degrees_order[j] > degrees_order[highest_index]){
                highest_index = j;
            }
        }
        if(degrees_order[highest_index] < 0){
            break;
        }
        degrees_order[highest_index] = -1;
        maximum_clique_local.push_back(highest_index);

        vector<int> tmp;
        vector<int> rejected;

        for(auto vec: subgraph_local){
            if(graph->matrix[highest_index][vec]){
                tmp.push_back(vec);
            }else{
                rejected.push_back(vec);
            }
        }
        if(maximum_clique_local.size() + tmp.size() <= maximum_found.size()){
            continue;
        }
        for(auto vec1: tmp){
            for(auto vec2: rejected){
                if(graph->matrix[vec1][vec2] != 0){
                    degrees_local[vec1]--;
                }
            }
        }
        auto q = maxCliqueApproxStep(graph, degrees_local, k+1, max_k, tmp, maximum_clique_local, maximum_found);
        cout<<"Returned from maxCliqueApproxStep"<<endl;

        if(q.size()>maximum_found.size()){
            maximum_found=q;
        }
        delete degrees_local;
    }
    //delete degrees_order;
    return maximum_found;
}
vector<int> GraphRunner::maxCliqueApprox2(Graph* graph, int k){
    Graph * g = Graph::fromGraph(graph);
    int n = g->size;
    auto degrees = new int(n);
    vector<int> subgraph(n);
    vector<int> maximum_clique_found, maximum_clique;
    
    for(int i = 0; i < n; i++){
        subgraph[i] = i;
        for (int j = 0; j < n; j++){
            if (g->matrix[i][j]!= 0 && g-> matrix[j][i] == 0){
                g-> matrix[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < n; i++){
        degrees[i] = 0;
        for(int j = 0; j < n; j++){
            if(g->matrix[i][j] != 0){
                degrees[i]++;
            }
        }
    }
    maximum_clique_found = maxCliqueApproxStep(g, degrees, 1, k+1, maximum_clique, maximum_clique_found, subgraph);
    //delete g;
    cout<<"Ended max clique approx"<<endl;
    return maximum_clique_found;
}
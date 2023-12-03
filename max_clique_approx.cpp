#include "max_clique_approx.h"
#include <algorithm>

using std::cout;
using std::endl;
MaxCliqueApprox::MaxCliqueApprox(std::ostream& output): output(output){
}
MaxCliqueApprox::~MaxCliqueApprox(){

}

vector<int> MaxCliqueApprox::maxCliqueApprox(Graph* graph, int k){
    if(k != 0 && this->k != 0){
        this->k = k;
    }
    cout<<"Max clique approx"<<endl;
    Graph * g = Graph::fromGraph(graph);
    int n = g->size;
    int* degrees = new int(n);
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
        cout<<"Current maximum index: "<< highest_index <<endl;
        cout<<"Current degrees: ";
        for(int i =0; i< n; i++)cout<<degrees[i]<<" ";
        cout<<endl;
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
    cout<<"Found approx maximum clique: ";
    for(auto vec:maximum_clique)cout<<vec<<" ";
    cout<<endl;
    return maximum_clique;
}


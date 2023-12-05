#include "graph.h"
#include <sstream>

Graph::Graph(int n){
    this->size = n;
    this->matrix = new int*[n];
    for(int i = 0; i < this->size; i++){
        this->matrix [i] = new int[n];
    }
    
}
Graph::~Graph(){
    for(int i = 0; i < this->size; i++){
        delete this->matrix[i];
    }
    delete this->matrix;
}

Graph* Graph::fromStream(std::istream& input){
    Graph* result;
    int size;
    std::string line;
    if(std::getline(input, line)){
        std::istringstream lineStream(line);
        lineStream >> size;
        result = new Graph(size);
    }else{
        throw std::invalid_argument("Invalid input: No size found");
    }

    for(int i = 0; i< size; i++){
        if(!std::getline(input, line)){
            throw std::invalid_argument("Invalid input: Expected line");
        }
        std::istringstream lineStream(line);
        int number;
        for ( int j =0; j < size; j++){
            if(!(lineStream >> number)){
                throw std::invalid_argument("Invalid input: Expected number in line " +std::to_string(i));
            }

            result->matrix[i][j] = number;
        }
    }

    return result;
}

Graph* Graph::fromGraph(Graph* other){
    Graph* result = new Graph(other->size);

    for (int i = 0; i<result->size; i++){
        for(int j = 0; j< result->size; j++){
            result->matrix[i][j] = other->matrix[i][j];
        }
    }
    return result;

}


void Graph::printToStream(std::ostream& stream){
    stream << this->size << std::endl;
    for (int i = 0; i < this->size; i++){
        for (int j = 0; j < this->size; j++){
            stream << this->matrix[i][j] << ' '; 
        }
        stream << std::endl;
    }
}

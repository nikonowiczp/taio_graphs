#pragma once
#include <string>
#include <iostream>

class Graph{
    
public:
    Graph(int n);
    ~Graph();
    static Graph* fromStream(std::istream& input);
    static Graph* fromGraph(Graph* other);

    void printToStream(std::ostream& stream);
    int size = -1;
    int ** matrix;
};

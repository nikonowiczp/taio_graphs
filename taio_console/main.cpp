#include <iostream>
#include <fstream>
#include <string>
#include "graph_runner.h"
#include "graph.h"
#include <map>
#include <sstream>

using std::cout;
using std::endl;
using std::string;

Graph* getGraphFromUser(std::vector<std::pair<string, Graph*>> &opened_files, string prefix ="");
std::pair<Graph*,Graph*> getGraphsFromUser(std::vector<std::pair<string, Graph*>> &opened_files); 

int main(int argc, char* argv[]){
    char char_input;
    std::vector<std::pair<string, Graph*>> opened_files;

    GraphRunner runner(cout);
    cout<<    
    "Witaj w rozwiazaniu laboratorium z TAIO, autorzy Mateusz Cichowicz, Patryk Nikonowicz.\n"
    "To jest tryb interaktywny. Jeśli chcesz wykonać wszystkie operacje na pliku i uzyskać wynik w folderze wywołaj program z następującymi argumentami:\n\n"
    "<plik wykonywalny> nazwa_pliku_wyjściowego nazwa_pliku_z_grafem_1 [nazwa_pliku_z_grafem_2]\n\n"
    "Jeśli otrzymany będzie jeden plik nie wykonanie zostanie na nim metryka ani podgraf\n";
    
    while(true){
        cout<<
            "Opcje są następujące:\n"
            "1 Rozmiar grafu    - wierzchołki \"aktywne\"\n"
            "2 Rozmiar grafu    - średnia dostępność\n"
            "3 Maksymalna klika - algorytm dokładny\n"
            "4 Maksymalna klika - algorytm aproksymacyjny New-Best-In\n"
            "5 Maksymalna klika - algorytm aproksymacyjny SM k\n"
            "6 Maksymalny wspólny podgraf - algorytm dokładny\n"
            "7 Maksymalny wspólny podgraf - algorytm aproksymacyjny\n"
            "8 Metryka\n"
            "q Zakończenie programu\n";    
        std::cin >> char_input;
        cout<<endl;
        switch(char_input){
            case 'q':{
                cout<<"Wychodzenie z programu\n";
                return 0;
            }
            case '1':{
                auto graph = getGraphFromUser(opened_files);
                auto result = runner.graphSize1(graph);
                cout<<"Wynik: Rozmiar grafu używając metody wierzchołki \"aktywne\" to: "<<result<<endl<<endl;
                break;
            }
            case '2':{
                auto graph = getGraphFromUser(opened_files);
                auto result = runner.graphSize2(graph);
                cout<<"Wynik: Rozmiar grafu używając metody średniej dostępności to: "<<result<<endl<<endl;
                break;
            }
            case '3':{
                auto graph = getGraphFromUser(opened_files);
                auto result = runner.maxClique(graph);
                cout<<"Wynik: Maksymalna klika znaleziona używając algorytmu dokładnego: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl<<endl;
                break;
            }
            case '4':{
                auto graph = getGraphFromUser(opened_files);
                auto result = runner.maxCliqueApprox(graph);
                cout<<"Wynik: Maksymalna klika znaleziona używając algorytmu aproksymacyjnego New-Best-In: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl<<endl;
                break;
            }
            case '5':{
                auto graph = getGraphFromUser(opened_files);
                auto result = runner.maxCliqueApprox(graph, 1);
                cout<<"Wynik: Maksymalna klika znaleziona używając algorytmu aproksymacyjnego SM k dla k = 1: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                result = runner.maxCliqueApprox(graph, 2);
                cout<<"Wynik: Maksymalna klika znaleziona używając algorytmu aproksymacyjnego SM k dla k = 2: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl;
                result = runner.maxCliqueApprox(graph, 3);
                cout<<"Wynik: Maksymalna klika znaleziona używając algorytmu aproksymacyjnego SM k dla k = 3: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl<<endl;
                break;
            }
            case '6':{
                auto graphs = getGraphsFromUser(opened_files);
                auto result = runner.maxSubgraph({graphs.first, graphs.second});
                cout<<"Wynik: Maksymalny wspólny podgraf znaleziony używając algorytmu dokładnego: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl<<endl;
                break;
            }
            case '7':{
                auto graphs = getGraphsFromUser(opened_files);
                auto result = runner.maxSubgraphApprox({graphs.first, graphs.second});
                cout<<"Wynik: Maksymalny wspólny podgraf znaleziony używając algorytmu aproksymacyjnego: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl<<endl;
                break;
            }
            case '8':{
                auto graphs = getGraphsFromUser(opened_files);
                auto result = runner.graphMetric({graphs.first, graphs.second});
                cout<<"Wynik: Metryka dla grafów: "<<result<<endl<<endl;
                break;
            }
            default:{
                cout<<"Invalid input";
                break;
            }
        }
    }

    //system("pause");
    return 0;
}
bool isNumber(string& s){
    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
Graph* getGraphFromUser(std::vector<std::pair<string, Graph*>> &opened_files, string prefix){
    while(true){
        cout<<prefix<<"Proszę wpisać nazwę pliku albo wpisać numer wczytanego już pliku"<<endl;

        for(int i = 0; i < opened_files.size(); i++){
            cout<<i<<": "<<opened_files[i].first<<endl;
        }
        string input;
        std::cin >> input;
        cout<<endl;
        if(isNumber(input)){
            int number;
            std::istringstream ( input ) >> number;
            if(opened_files.size()>=(number+1)){
                return opened_files[number].second;
            }
        }

        std::ifstream inputFile(input);
        if (!inputFile.is_open()) {
            std::cerr << "Nie znaleziono pliku: " << input << endl;
            continue;
        }

        auto graph = Graph::fromStream(inputFile);
        opened_files.push_back({input, graph});
        return graph;
    }        
}
std::pair<Graph*,Graph*> getGraphsFromUser(std::vector<std::pair<string, Graph*>> &opened_files){
    Graph *graph1, *graph2;
    graph1 = getGraphFromUser(opened_files, "Graf 1: ");
    graph2 = getGraphFromUser(opened_files, "Graf 2: ");
    
    return {graph1, graph2};
}

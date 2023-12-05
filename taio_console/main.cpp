#include <iostream>
#include <fstream>
#include <string>
#include "graph_runner.h"
#include "graph.h"
#include <map>
#include <sstream>
#include <chrono>

using std::cout;
using std::endl;
using std::string;

Graph* getGraphFromUser(std::vector<std::pair<string, Graph*>> &opened_files, string prefix ="");
std::pair<Graph*,Graph*> getGraphsFromUser(std::vector<std::pair<string, Graph*>> &opened_files); 

int main(int argc, char* argv[]){
    char char_input;
    std::vector<std::pair<string, Graph*>> opened_files;
    std::chrono::steady_clock::time_point start, end; 
    std::chrono::microseconds duration;
    GraphRunner runner(cout);
    cout<<    
    "Witaj w rozwiazaniu laboratorium z TAIO, autorzy Mateusz Cichowicz, Patryk Nikonowicz.\n"
    "To jest tryb interaktywny. Jesli chcesz wykonac wszystkie operacje na pliku i uzyskac wynik w folderze wywolaj program z nastepujacymi argumentami:\n\n"
    "<plik wykonywalny> nazwa_pliku_wyjsciowego nazwa_pliku_z_grafem_1 [nazwa_pliku_z_grafem_2]\n\n"
    "Jesli otrzymany bedzie jeden plik nie wykonanie zostanie na nim metryka ani podgraf\n";
    
    while(true){
        cout<<
            "Opcje sa nastepujace:\n"
            "1 Rozmiar grafu    - wierzcholki \"aktywne\"\n"
            "2 Rozmiar grafu    - srednia dostepnosc\n"
            "3 Maksymalna klika - algorytm dokladny\n"
            "4 Maksymalna klika - algorytm aproksymacyjny New-Best-In\n"
            "5 Maksymalna klika - algorytm aproksymacyjny SM k\n"
            "6 Maksymalny wspolny podgraf - algorytm dokladny\n"
            "7 Maksymalny wspolny podgraf - algorytm aproksymacyjny\n"
            "8 Metryka\n"
            "q Zakonczenie programu\n";    
        std::cin >> char_input;
        cout<<endl;
        switch(char_input){
            case 'q':{
                cout<<"Wychodzenie z programu\n";
                return 0;
            }
            case '1':{
                auto graph = getGraphFromUser(opened_files);
                start = std::chrono::high_resolution_clock::now();
                auto result = runner.graphSize1(graph);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                cout<<"Wynik: Rozmiar grafu używajac metody wierzcholki \"aktywne\" to: "<<result<<endl<<"Wykonanie zajelo: "<<duration.count()<<" ms"<<endl<<endl;
                break;
            }
            case '2':{
                auto graph = getGraphFromUser(opened_files);
                start = std::chrono::high_resolution_clock::now();
                auto result = runner.graphSize2(graph);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                cout<<"Wynik: Rozmiar grafu używajac metody sredniej dostepnosci to: "<<result<<endl<<"Wykonanie zajelo: "<<duration.count()<<" ms"<<endl<<endl;
                break;
            }
            case '3':{
                auto graph = getGraphFromUser(opened_files);
                start = std::chrono::high_resolution_clock::now();
                auto result = runner.maxClique(graph);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                cout<<"Wynik: Maksymalna klika znaleziona używajac algorytmu dokladnego: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl<<"Wykonanie zajelo: "<<duration.count()<<" ms"<<endl<<endl;
                break;
            }
            case '4':{
                auto graph = getGraphFromUser(opened_files);
                start = std::chrono::high_resolution_clock::now();
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                auto result = runner.maxCliqueApprox1(graph);
                cout<<"Wynik: Maksymalna klika znaleziona używajac algorytmu aproksymacyjnego New-Best-In: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl<<"Wykonanie zajelo: "<<duration.count()<<" ms"<<endl<<endl;
                break;
            }
            case '5':{
                auto graph = getGraphFromUser(opened_files);
                start = std::chrono::high_resolution_clock::now();
                auto result = runner.maxCliqueApprox2(graph, 1);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                cout<<"Wynik: Maksymalna klika znaleziona używajac algorytmu aproksymacyjnego SM k dla k = 1: "<<endl;
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl<<"Wykonanie zajelo: "<<duration.count()<<" ms"<<endl<<endl;
                break;
            }
            case '6':{
                auto graphs = getGraphsFromUser(opened_files);
                start = std::chrono::high_resolution_clock::now();
                auto result = runner.maxSubgraph({graphs.first, graphs.second});
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                cout<<"Wynik: Maksymalny wspolny podgraf znaleziony używajac algorytmu dokladnego: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl<<"Wykonanie zajelo: "<<duration.count()<<" ms"<<endl<<endl;
                break;
            }
            case '7':{
                auto graphs = getGraphsFromUser(opened_files);
                start = std::chrono::high_resolution_clock::now();
                auto result = runner.maxSubgraphApprox({graphs.first, graphs.second});
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                cout<<"Wynik: Maksymalny wspolny podgraf znaleziony używajac algorytmu aproksymacyjnego: ";
                for(auto v: result){
                    cout<<v<<" ";
                }
                cout<<endl<<"Wykonanie zajelo: "<<duration.count()<<" ms"<<endl<<endl;
                break;
            }
            case '8':{
                auto graphs = getGraphsFromUser(opened_files);
                start = std::chrono::high_resolution_clock::now();
                auto result = runner.graphMetric({graphs.first, graphs.second});
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                cout<<"Wynik: Metryka dla grafow: "<<result<<endl<<"Wykonanie zajelo: "<<duration.count()<<" ms"<<endl<<endl;
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
        cout<<prefix<<"Prosze wpisac nazwe pliku albo wpisac numer wczytanego już pliku"<<endl;

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

#include <iostream>
#include <fstream>
#include <string>
#include "graph_runner.h"
#include "graph.h"
#include <map>
#include <sstream>
#include <chrono>
#include <algorithm>
using std::cout;
using std::endl;
using std::string;

Graph *getGraphFromUser(std::vector<std::pair<string, Graph *>> &opened_files, string prefix = "");
std::pair<Graph *, Graph *> getGraphsFromUser(std::vector<std::pair<string, Graph *>> &opened_files);
int run_non_interactive(int argc, char *argv[]);
void run_case_graph_size_1(GraphRunner runner, Graph *graph, std::ostream &output);
void run_case_graph_size_2(GraphRunner runner, Graph *graph, std::ostream &output);
void run_case_max_clique(GraphRunner runner, Graph *graph, std::ostream &output);
void run_case_max_clique_approx_1(GraphRunner runner, Graph *graph, std::ostream &output);
void run_case_max_clique_approx_2(GraphRunner runner, Graph *graph, int k, std::ostream &output);
void run_case_max_subgraph(GraphRunner runner, Graph *graph1, Graph *graph2, std::ostream &output);
void run_case_max_subgraph_approx(GraphRunner runner, Graph *graph1, Graph *graph2, std::ostream &output);
void run_case_metric(GraphRunner runner, Graph *graph1, Graph *graph2, std::ostream &output);

int main(int argc, char *argv[])
{
    char char_input;
    std::vector<std::pair<string, Graph *>> opened_files;
    GraphRunner runner(cout);
    if (argc != 1)
    {
        return run_non_interactive(argc, argv);
    }
    cout << "Witaj w rozwiazaniu laboratorium z TAIO, autorzy Mateusz Cichowicz, Patryk Nikonowicz.\n"
            "To jest tryb interaktywny. Jesli chcesz wykonac wszystkie operacje na pliku i uzyskac wynik w folderze wywolaj program z nastepujacymi argumentami:\n\n"
            "<plik wykonywalny> nazwa_pliku_wyjsciowego nazwa_pliku_z_grafem_1 [nazwa_pliku_z_grafem_2]\n\n"
            "Jesli otrzymany bedzie jeden plik nie wykonanie zostanie na nim metryka ani podgraf\n";

    while (true)
    {
        cout << "Opcje sa nastepujace:\n"
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
        cout << endl;
        switch (char_input)
        {
        case 'q':
        {
            cout << "Wychodzenie z programu\n";
            return 0;
        }
        case '1':
        {
            auto graph = getGraphFromUser(opened_files);
            run_case_graph_size_1(runner, graph, cout);
            break;
        }
        case '2':
        {
            auto graph = getGraphFromUser(opened_files);
            run_case_graph_size_2(runner, graph, cout);

            break;
        }
        case '3':
        {
            auto graph = getGraphFromUser(opened_files);
            run_case_max_clique(runner, graph, cout);

            break;
        }
        case '4':
        {
            auto graph = getGraphFromUser(opened_files);
            run_case_max_clique_approx_1(runner, graph, cout);
            break;
        }
        case '5':
        {
            auto graph = getGraphFromUser(opened_files);
            int k = -1;
            while (k == -1)
            {
                char input;
                cout << "Prosze wybierz k:" << endl;
                std::cin >> input;
                if (std::isdigit(input))
                {
                    k = input - '0';
                }
                cout << endl;
            }
            run_case_max_clique_approx_2(runner, graph, k, cout);
            break;
        }
        case '6':
        {
            auto graphs = getGraphsFromUser(opened_files);
            run_case_max_subgraph(runner, graphs.first, graphs.second, cout);
            break;
        }
        case '7':
        {
            auto graphs = getGraphsFromUser(opened_files);
            run_case_max_subgraph_approx(runner, graphs.first, graphs.second, cout);
            break;
        }
        case '8':
        {
            auto graphs = getGraphsFromUser(opened_files);
            run_case_metric(runner, graphs.first, graphs.second, cout);
            break;
        }
        default:
        {
            cout << "Invalid input";
            break;
        }
        }
    }

    // system("pause");
    return 0;
}
bool isNumber(string &s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}
Graph *getGraphFromUser(std::vector<std::pair<string, Graph *>> &opened_files, string prefix)
{
    while (true)
    {
        cout << prefix << "Prosze wpisac nazwe pliku albo wpisac numer wczytanego juz pliku" << endl;

        for (int i = 0; i < opened_files.size(); i++)
        {
            cout << i << ": " << opened_files[i].first << endl;
        }
        string input;
        std::cin >> input;
        cout << endl;
        if (isNumber(input))
        {
            int number;
            std::istringstream(input) >> number;
            if (opened_files.size() >= (number + 1))
            {
                return opened_files[number].second;
            }
        }

        std::ifstream inputFile(input);
        if (!inputFile.is_open())
        {
            std::cerr << "Nie znaleziono pliku: " << input << endl;
            continue;
        }

        auto graph = Graph::fromStream(inputFile);
        opened_files.push_back({input, graph});
        return graph;
    }
}
std::pair<Graph *, Graph *> getGraphsFromUser(std::vector<std::pair<string, Graph *>> &opened_files)
{
    Graph *graph1, *graph2;
    graph1 = getGraphFromUser(opened_files, "Graf 1: ");
    graph2 = getGraphFromUser(opened_files, "Graf 2: ");

    return {graph1, graph2};
}

void run_case_graph_size_1(GraphRunner runner, Graph *graph, std::ostream &output)
{
    std::chrono::steady_clock::time_point start, end;
    std::chrono::microseconds duration;
    start = std::chrono::high_resolution_clock::now();
    auto result = runner.graphSize1(graph);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    output << "Wynik: Rozmiar grafu uzywajac metody wierzcholki \"aktywne\" to: " << result << endl
           << "Wykonanie zajelo: " << duration.count() << " ms" << endl
           << endl;
}

void run_case_graph_size_2(GraphRunner runner, Graph *graph, std::ostream &output)
{
    std::chrono::steady_clock::time_point start, end;
    std::chrono::microseconds duration;
    start = std::chrono::high_resolution_clock::now();
    auto result = runner.graphSize2(graph);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    output << "Wynik: Rozmiar grafu uzywajac metody sredniej dostepnosci to: " << result << endl
           << "Wykonanie zajelo: " << duration.count() << " ms" << endl
           << endl;
}
void run_case_max_clique(GraphRunner runner, Graph *graph, std::ostream &output)
{
    std::chrono::steady_clock::time_point start, end;
    std::chrono::microseconds duration;

    start = std::chrono::high_resolution_clock::now();
    auto result = runner.maxClique(graph);
    std::sort(result.begin(), result.end()); 

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    output << "Wynik: Maksymalna klika znaleziona uzywajac algorytmu dokladnego: "<<endl;
    for (auto v : result)
    {
        output << v << " ";
    }
    output << endl
           << "Wykonanie zajelo: " << duration.count() << " ms" << endl
           << endl;
}
void run_case_max_clique_approx_1(GraphRunner runner, Graph *graph, std::ostream &output)
{
    std::chrono::steady_clock::time_point start, end;
    std::chrono::microseconds duration;
    start = std::chrono::high_resolution_clock::now();
    auto result = runner.maxCliqueApprox1(graph);
    std::sort(result.begin(), result.end()); 
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    output << "Wynik: Maksymalna klika znaleziona uzywajac algorytmu aproksymacyjnego New-Best-In: "<<endl;
    for (auto v : result)
    {
        output << v << " ";
    }
    output << endl
           << "Wykonanie zajelo: " << duration.count() << " ms" << endl
           << endl;
}
void run_case_max_clique_approx_2(GraphRunner runner, Graph *graph, int k, std::ostream &output)
{
    std::chrono::steady_clock::time_point start, end;
    std::chrono::microseconds duration;
    start = std::chrono::high_resolution_clock::now();
    auto result = runner.maxCliqueApprox2(graph, k);
    std::sort(result.begin(), result.end()); 
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    output << "Wynik: Maksymalna klika znaleziona uzywajac algorytmu aproksymacyjnego SM k dla k = "<<k<<": " << endl;
    for (auto v : result)
    {
        output << v << " ";
    }
    output << endl
           << "Wykonanie zajelo: " << duration.count() << " ms" << endl
           << endl;
}
void run_case_max_subgraph(GraphRunner runner, Graph *graph1, Graph *graph2, std::ostream &output)
{
    std::chrono::steady_clock::time_point start, end;
    std::chrono::microseconds duration;
    start = std::chrono::high_resolution_clock::now();
    auto result = runner.maxSubgraph({graph1, graph2});
    std::sort(result.begin(), result.end());
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    output << "Wynik: Maksymalny wspolny podgraf znaleziony uzywajac algorytmu dokladnego: ";
    for (auto v : result)
    {
        output << v << " ";
    }
    output << endl
           << "Wykonanie zajelo: " << duration.count() << " ms" << endl
           << endl;
}
void run_case_max_subgraph_approx(GraphRunner runner, Graph *graph1, Graph *graph2, std::ostream &output)
{
    std::chrono::steady_clock::time_point start, end;
    std::chrono::microseconds duration;
    start = std::chrono::high_resolution_clock::now();
    auto result = runner.maxSubgraphApprox({graph1, graph2});
    std::sort(result.begin(), result.end());
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    output << "Wynik: Maksymalny wspolny podgraf znaleziony uzywajac algorytmu aproksymacyjnego: ";
    for (auto v : result)
    {
        output << v << " ";
    }
    output << endl
           << "Wykonanie zajelo: " << duration.count() << " ms" << endl
           << endl;
}
void run_case_metric(GraphRunner runner, Graph *graph1, Graph *graph2, std::ostream &output)
{
    std::chrono::steady_clock::time_point start, end;
    std::chrono::microseconds duration;
    start = std::chrono::high_resolution_clock::now();
    auto result = runner.graphMetric({graph1, graph2});
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    output << "Wynik: Metryka dla grafow: " << result << endl
           << "Wykonanie zajelo: " << duration.count() << " ms" << endl
           << endl;
}
int run_non_interactive(int argc, char *argv[])
{
    string output, input1, input2;
    Graph *g1 = nullptr, *g2 = nullptr;
    std::chrono::steady_clock::time_point start, end;
    std::chrono::microseconds duration;
    GraphRunner runner(cout);

    if (argc == 4)
    {
        output = argv[1];
        input1 = argv[2];
        input2 = argv[3];
        std::ifstream inputFile1(input1), inputFile2(input2);
        if (!inputFile1.is_open())
        {
            std::cerr << "Nie znaleziono pliku: " << input1 << endl;
            return 1;
        }
        if (!inputFile2.is_open())
        {
            std::cerr << "Nie znaleziono pliku: " << input2 << endl;
            return 1;
        }
        std::ofstream outputFile(output);
        if(!outputFile.is_open()){
            std::cerr << "Nie można otworzyć pliku: " << output << endl;
            return 1;
        }
        g1 = Graph::fromStream(inputFile1);
        g2 = Graph::fromStream(inputFile2);
        outputFile<<"Graph1 "<<input1<<endl;
        run_case_graph_size_1(runner, g1, outputFile);
        run_case_graph_size_2(runner, g1, outputFile);
        run_case_max_clique(runner, g1, outputFile);
        run_case_max_clique_approx_1(runner, g1, outputFile);
        run_case_max_clique_approx_2(runner, g1, 1, outputFile);

        outputFile<<endl;
        outputFile<<"Graph2 "<<input2<<endl;
        run_case_graph_size_1(runner, g2, outputFile);
        run_case_graph_size_2(runner, g2, outputFile);
        run_case_max_clique(runner, g2, outputFile);
        run_case_max_clique_approx_1(runner, g2, outputFile);
        run_case_max_clique_approx_2(runner, g2, 1, outputFile);

        outputFile<<"Oba grafy: "<<input2<<endl;
        run_case_max_subgraph(runner, g1, g2, outputFile);
        run_case_max_subgraph_approx(runner, g1, g2, outputFile);
        run_case_metric(runner, g1, g2, outputFile);
        outputFile.close();
    }
    else if (argc == 3)
    {
        // one graph
        output = argv[1];
        input1 = argv[2];
        std::ifstream inputFile1(input1);
        if (!inputFile1.is_open())
        {
            std::cerr << "Nie znaleziono pliku: " << input1 << endl;
            return 1;
        }
        std::ofstream outputFile(output);
        if(!outputFile.is_open()){
            std::cerr << "Nie można otworzyć pliku: " << output << endl;
            return 1;
        }
        g1 = Graph::fromStream(inputFile1);
        outputFile<<"Graph1 "<<input1<<endl;
        run_case_graph_size_1(runner, g1, outputFile);
        run_case_graph_size_2(runner, g1, outputFile);
        run_case_max_clique(runner, g1, outputFile);
        run_case_max_clique_approx_1(runner, g1, outputFile);
        run_case_max_clique_approx_2(runner, g1, 1, outputFile);
        outputFile.close();
    }
    else
    {
        cout << "Uzycie: <plik wykonywalny> nazwa_pliku_wyjsciowego nazwa_pliku_z_grafem_1 [nazwa_pliku_z_grafem_2] " << endl;
        return 1;
    }
}
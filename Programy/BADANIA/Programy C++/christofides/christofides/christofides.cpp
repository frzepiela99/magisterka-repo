#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <set>
#include <queue>
#include <tuple>
#include <utility>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Struktura reprezentująca graf
struct Graph {
    int V; // Liczba wierzchołków
    std::vector<std::vector<double>> adjMatrix; // Macierz sąsiedztwa

    Graph(int V) : V(V), adjMatrix(V, std::vector<double>(V, 0)) {}

    void addEdge(int u, int v, double w) {
        adjMatrix[u][v] = w;
        adjMatrix[v][u] = w;
    }
};

// Funkcja obliczająca minimalne drzewo rozpinające za pomocą algorytmu Prima
Graph primMST(const Graph& graph, double& mstCost) {
    int V = graph.V;
    Graph mst(V);
    std::vector<double> key(V, std::numeric_limits<double>::max());
    std::vector<int> parent(V, -1);
    std::vector<bool> inMST(V, false);

    key[0] = 0.0;

    for (int count = 0; count < V - 1; ++count) {
        double minKey = std::numeric_limits<double>::max();
        int u;

        for (int v = 0; v < V; ++v) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        inMST[u] = true;

        for (int v = 0; v < V; ++v) {
            if (graph.adjMatrix[u][v] && !inMST[v] && graph.adjMatrix[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph.adjMatrix[u][v];
            }
        }
    }

    mstCost = 0.0;
    for (int i = 1; i < V; ++i) {
        mst.addEdge(parent[i], i, graph.adjMatrix[parent[i]][i]);
        mstCost += graph.adjMatrix[parent[i]][i];
    }

    return mst;
}

// Funkcja znajdująca wierzchołki o nieparzystym stopniu
std::vector<int> findOddDegreeVertices(const Graph& graph) {
    std::vector<int> oddDegreeVertices;
    for (int i = 0; i < graph.V; ++i) {
        int degree = 0;
        for (int j = 0; j < graph.V; ++j) {
            if (graph.adjMatrix[i][j] != 0) {
                ++degree;
            }
        }
        if (degree % 2 == 1) {
            oddDegreeVertices.push_back(i);
        }
    }
    return oddDegreeVertices;
}

// Funkcja do znajdowania minimalnego doskonałego skojarzenia
std::vector<std::pair<int, int>> minWeightMatching(const Graph& graph, const std::vector<int>& oddDegreeVertices) {
    std::vector<std::pair<int, int>> matching;
    int n = oddDegreeVertices.size();
    std::vector<bool> visited(n, false);

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            int u = oddDegreeVertices[i];
            double minWeight = std::numeric_limits<double>::max();
            int minVertex = -1;

            for (int j = 0; j < n; ++j) {
                if (i != j && !visited[j] && graph.adjMatrix[u][oddDegreeVertices[j]] < minWeight) {
                    minWeight = graph.adjMatrix[u][oddDegreeVertices[j]];
                    minVertex = j;
                }
            }

            visited[minVertex] = true;
            matching.push_back({ u, oddDegreeVertices[minVertex] });
        }
    }

    return matching;
}

// Funkcja realizująca algorytm Christofidesa
std::vector<int> christofidesAlgorithm(const Graph& graph, double& tourCost, double& mstCost) {
    // Krok 1: Znajdź minimalne drzewo rozpinające (MST)
    Graph mst = primMST(graph, mstCost);

    // Krok 2: Znajdź wierzchołki o nieparzystym stopniu w MST
    std::vector<int> oddDegreeVertices = findOddDegreeVertices(mst);

    // Krok 3: Znajdź minimalne doskonałe skojarzenie dla tych wierzchołków
    std::vector<std::pair<int, int>> matching = minWeightMatching(graph, oddDegreeVertices);

    // Krok 4: Dodaj krawędzie skojarzenia do MST, aby utworzyć graf eulerowski
    for (const auto& edge : matching) {
        mst.addEdge(edge.first, edge.second, graph.adjMatrix[edge.first][edge.second]);
    }

    // Krok 5: Znajdź cykl Eulera w powstałym grafie
    std::vector<int> eulerianCycle;
    std::vector<bool> visited(graph.V, false);
    std::vector<int> stack;
    stack.push_back(0);

    while (!stack.empty()) {
        int u = stack.back();
        bool hasUnvisited = false;

        for (int v = 0; v < graph.V; ++v) {
            if (mst.adjMatrix[u][v] != 0) {
                stack.push_back(v);
                mst.adjMatrix[u][v] = 0;
                mst.adjMatrix[v][u] = 0;
                hasUnvisited = true;
                break;
            }
        }

        if (!hasUnvisited) {
            eulerianCycle.push_back(u);
            stack.pop_back();
        }
    }

    // Krok 6: Stwórz cykl Hamiltona z cyklu Eulera (pomijając powtarzające się wierzchołki)
    std::vector<int> hamiltonianCycle;
    std::vector<bool> visitedHamiltonian(graph.V, false);

    for (int v : eulerianCycle) {
        if (!visitedHamiltonian[v]) {
            hamiltonianCycle.push_back(v);
            visitedHamiltonian[v] = true;
        }
    }

    // Obliczanie sumy wag krawędzi trasy
    tourCost = 0;
    for (size_t i = 0; i < hamiltonianCycle.size() - 1; ++i) {
        tourCost += graph.adjMatrix[hamiltonianCycle[i]][hamiltonianCycle[i + 1]];
    }
    // Dodanie kosztu powrotu do startowego wierzchołka
    tourCost += graph.adjMatrix[hamiltonianCycle.back()][hamiltonianCycle[0]];

    return hamiltonianCycle;
}

// Funkcja do zapisywania wyników do pliku
void saveResults(const std::vector<int>& path, double totalCost, double mstCost, double duration) {
    std::ofstream outFile("results.txt");
    outFile << "Path: ";
    for (int v : path) {
        outFile << v + 1 << " "; // Zamiana indeksów na 1-bazowe
    }
    outFile << std::endl;

    outFile << "Total cost of the tour: " << totalCost << std::endl;
    outFile << "Total cost of the MST: " << mstCost << std::endl;
    outFile << std::fixed << std::setprecision(3);
    outFile << "Execution time: " << duration << " seconds" << std::endl;
    outFile.close();

    // Zapis wyników do pliku CSV
    std::ofstream csvFile("results.csv");
    csvFile << "Vertex\n";
    for (int v : path) {
        csvFile << v + 1 << "\n"; // Zamiana indeksów na 1-bazowe
    }
    csvFile << "Total cost of the tour," << totalCost << "\n";
    csvFile << "Total cost of the MST," << mstCost << "\n";
    csvFile << std::fixed << std::setprecision(6);
    csvFile << "Execution time (seconds)," << duration << "\n";
    csvFile.close();
}

int main() {
    std::ifstream inputFile("C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\magisterka-repo\\Programy\\BADANIA\\Dane_wejsciowe\\edges1000.in");
    if (!inputFile) {
        std::cerr << "Nie można otworzyć pliku." << std::endl;
        return 1;
    }

    int V, E;
    inputFile >> V >> E;
    Graph graph(V);

    int u, v;
    double w;
    while (inputFile >> u >> v >> w) {
        graph.addEdge(u - 1, v - 1, w); // Zamiana indeksów na 0-bazowe
    }

    auto start = high_resolution_clock::now();

    double tourCost, mstCost;
    std::vector<int> tour = christofidesAlgorithm(graph, tourCost, mstCost);

    auto stop = high_resolution_clock::now();
    duration<double> duration = stop - start;

    std::cout << "Znaleziona trasa: ";
    for (int v : tour) {
        std::cout << (v + 1) << " "; // Zamiana indeksów na 1-bazowe
    }
    std::cout << (tour[0] + 1) << std::endl; // Powrót do początkowego wierzchołka

    std::cout << "Suma wag krawedzi trasy: " << tourCost << std::endl;
    std::cout << "Suma wag krawedzi w MST: " << mstCost << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Czas dzialania programu: " << duration.count() << " sekund" << std::endl;

    saveResults(tour, tourCost, mstCost, duration.count());

    return 0;
}

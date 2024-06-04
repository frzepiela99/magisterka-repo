#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
#include <chrono>
#include <unordered_set>
#include <tuple>

using namespace std;
using namespace chrono;

// Struktura krawędzi używana do priorytetowej kolejki
struct Edge {
    int u, v, weight;
    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }
};

// Algorytm Prima do znajdowania MST
vector<int> primMST(const vector<vector<int>>& graph, int& mstWeight) {
    int V = graph.size();
    vector<int> parent(V, -1); // Rodzic w MST
    vector<int> key(V, INT_MAX); // Minimalny koszt krawędzi
    vector<bool> inMST(V, false); // Czy wierzchołek jest w MST
    priority_queue<Edge, vector<Edge>, greater<Edge>> pq; // Priorytetowa kolejka

    key[0] = 0;
    pq.push({ -1, 0, 0 }); // Inicjalizacja z wierzchołkiem 0

    mstWeight = 0; // Inicjalizacja całkowitej wagi MST

    while (!pq.empty()) {
        int u = pq.top().v;
        pq.pop();

        if (inMST[u]) continue;
        inMST[u] = true;

        if (parent[u] != -1) {
            mstWeight += graph[u][parent[u]]; // Dodaj wagę krawędzi do całkowitej wagi MST
        }

        for (int v = 0; v < V; ++v) {
            if (graph[u][v] && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                pq.push({ u, v, key[v] });
                parent[v] = u;
            }
        }
    }

    return parent;
}

// Przechodzenie pre-order po MST
void preorderTraversal(const vector<vector<int>>& mst, int u, vector<bool>& visited, vector<int>& path) {
    visited[u] = true;
    path.push_back(u);
    for (int v : mst[u]) {
        if (!visited[v]) {
            preorderTraversal(mst, v, visited, path);
        }
    }
}

// Generowanie przybliżonego rozwiązania TSP
vector<int> approxTSP(const vector<vector<int>>& graph, int& mstWeight) {
    vector<int> parent = primMST(graph, mstWeight);
    int V = graph.size();
    vector<vector<int>> mst(V);

    for (int i = 1; i < V; ++i) {
        mst[parent[i]].push_back(i);
        mst[i].push_back(parent[i]);
    }

    vector<bool> visited(V, false);
    vector<int> path;
    preorderTraversal(mst, 0, visited, path);
    path.push_back(0); // Zamknięcie cyklu

    return path;
}

// Obliczanie całkowitej długości trasy i zwracanie długości krawędzi
pair<int, vector<int>> calculatePathLength(const vector<vector<int>>& graph, const vector<int>& path) {
    int totalLength = 0;
    vector<int> edgeLengths;
    for (int i = 0; i < path.size() - 1; ++i) {
        int length = graph[path[i]][path[i + 1]];
        totalLength += length;
        edgeLengths.push_back(length);
    }
    return { totalLength, edgeLengths };
}

// Weryfikacja poprawności ścieżki
bool verifyPath(const vector<int>& path, int numVertices) {
    vector<bool> visitedVertices(numVertices, false);
    for (int v : path) {
        visitedVertices[v] = true;
    }
    for (bool visited : visitedVertices) {
        if (!visited) return false;
    }
    return true;
}

// Zapis wyników do pliku
void saveResults(const vector<int>& path, int totalLength, const vector<int>& edgeLengths, int mstWeight, long long duration, int pathLength) {
    ofstream outFile("results.txt");
    outFile << "Path: ";
    for (int v : path) {
        outFile << v + 1 << " "; // adjust back to 1-indexed
    }
    outFile << endl;

    outFile << "Edge lengths: ";
    for (int length : edgeLengths) {
        outFile << length << " ";
    }
    outFile << endl;

    outFile << "Total length of the approximate TSP path: " << totalLength << endl;
    outFile << "Total weight of the MST: " << mstWeight << endl;
    outFile << "Execution time: " << duration << " microseconds" << endl;
    outFile << "Path length (number of edges): " << pathLength << endl;
    outFile.close();
}

int main() {
    ifstream inputFile("C:\\Users\\mcmys\\source\\repos\\ConsoleApplication6\\ConsoleApplication6\\edges1000.txt");
    if (!inputFile) {
        cerr << "Unable to open file";
        exit(1);
    }

    int V, E;
    inputFile >> V >> E;

    vector<vector<int>> graph(V, vector<int>(V, 0));
    int totalWeight = 0;

    int u, v, w;
    while (inputFile >> u >> v >> w) {
        graph[u - 1][v - 1] = w;
        graph[v - 1][u - 1] = w; // Assuming the graph is undirected
        totalWeight += w;
    }
    inputFile.close();

    auto start = high_resolution_clock::now();

    int mstWeight;
    vector<int> path = approxTSP(graph, mstWeight);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    int totalLength;
    vector<int> edgeLengths;
    tie(totalLength, edgeLengths) = calculatePathLength(graph, path);
    bool isPathValid = verifyPath(path, V);

    cout << "Approximate TSP path: ";
    for (int v : path) {
        cout << v + 1 << " "; // adjust back to 1-indexed
    }
    cout << "\n" << endl;

    cout << "Edge lengths: ";
    for (int length : edgeLengths) {
        cout << length << " ";
    }
    cout << "\n" << endl;

    cout << "Total length of the approximate TSP path: " << totalLength << endl;
    cout << "\n" << endl;
    cout << "Total weight of the MST: " << mstWeight << endl;
    cout << "\n" << endl;
    cout << "Execution time: " << duration.count() << " microseconds" << endl;
    cout << "\n" << endl;
    cout << "Path validity: " << (isPathValid ? "Valid" : "Invalid") << endl;

    int pathLength = path.size() - 1; // Odejmujemy 1, aby uzyskać liczbę krawędzi
    if (pathLength == V) {
        pathLength -= 1; // Korekta dla przypadku, gdy ścieżka zawiera dodatkowy powrót do początku
    }
    cout << "Path length (number of edges): " << pathLength << endl;

    saveResults(path, totalLength, edgeLengths, mstWeight, duration.count(), pathLength);

    return 0;
}

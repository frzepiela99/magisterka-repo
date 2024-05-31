#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;

const int INF = 999999;

class TSPSolver {
private:
    vector<vector<int>> matrix;
    vector<int> bestPath;
    int n;
    int bestCost = INF;

    struct Node {
        vector<vector<int>> reducedMatrix;
        vector<int> path;
        int cost;
        int vertex;
        int level;
    };

    struct compare {
        bool operator()(const Node& lhs, const Node& rhs) const {
            return lhs.cost > rhs.cost;
        }
    };

    void reduceMatrix(vector<vector<int>>& matrix, int& costReduction) {
        costReduction = 0;
        for (int i = 0; i < n; i++) {
            int rowMin = *min_element(matrix[i].begin(), matrix[i].end());
            if (rowMin != INF && rowMin > 0) {
                for (int j = 0; j < n; j++) {
                    if (matrix[i][j] != INF) {
                        matrix[i][j] -= rowMin;
                    }
                }
                costReduction += rowMin;
            }
        }
        for (int j = 0; j < n; j++) {
            int colMin = INF;
            for (int i = 0; i < n; i++) {
                if (matrix[i][j] < colMin) {
                    colMin = matrix[i][j];
                }
            }
            if (colMin != INF && colMin > 0) {
                for (int i = 0; i < n; i++) {
                    if (matrix[i][j] != INF) {
                        matrix[i][j] -= colMin;
                    }
                }
                costReduction += colMin;
            }
        }
    }

    int calculateCost(vector<vector<int>>& matrix) {
        int costReduction = 0;
        reduceMatrix(matrix, costReduction);
        return costReduction;
    }

    bool isPromising(int currentCost, int edgeCost, int reductionCost) {
        return (currentCost + edgeCost + reductionCost < bestCost);
    }

    bool createsCycle(const vector<int>& path, int nextVertex) {
        unordered_set<int> visited(path.begin(), path.end());
        return visited.find(nextVertex) != visited.end() || (path.size() == n && nextVertex != 0);
    }

    void printMatrix(const vector<vector<int>>& matrix) {
        for (const auto& row : matrix) {
            for (int val : row) {
                if (val == INF) {
                    cout << "INF ";
                }
                else {
                    cout << val << " ";
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    void debugState(const Node& node) {
        cout << "Level: " << node.level << ", Vertex: " << node.vertex << ", Cost: " << node.cost << endl;
        cout << "Path: ";
        for (int v : node.path) {
            cout << v << " ";
        }
        cout << endl;
        printMatrix(node.reducedMatrix);
    }

    void solve() {
        priority_queue<Node, vector<Node>, compare> pq;

        Node root;
        root.reducedMatrix = matrix;
        root.path.push_back(0);
        root.cost = calculateCost(root.reducedMatrix);
        root.vertex = 0;
        root.level = 0;

        pq.push(root);

        while (!pq.empty()) {
            Node minNode = pq.top();
            pq.pop();

            debugState(minNode);

            int i = minNode.vertex;

            if (minNode.level == n - 1) {
                minNode.path.push_back(0);
                int finalCost = minNode.cost + (minNode.reducedMatrix[i][0] != INF ? minNode.reducedMatrix[i][0] : 0);
                if (finalCost < bestCost && finalCost >= 0) {
                    bestCost = finalCost;
                    bestPath = minNode.path;
                }
                continue;
            }

            for (int j = 0; j < n; j++) {
                if (minNode.reducedMatrix[i][j] != INF && !createsCycle(minNode.path, j)) {
                    Node child;
                    child.path = minNode.path;
                    child.path.push_back(j);

                    child.reducedMatrix = minNode.reducedMatrix;
                    for (int k = 0; k < n; k++) {
                        child.reducedMatrix[i][k] = INF;
                        child.reducedMatrix[k][j] = INF;
                    }
                    child.reducedMatrix[j][0] = INF;

                    int edgeCost = minNode.reducedMatrix[i][j];
                    int reductionCost = calculateCost(child.reducedMatrix);
                    child.cost = minNode.cost + edgeCost + reductionCost;
                    child.vertex = j;
                    child.level = minNode.level + 1;

                    if (isPromising(child.cost, edgeCost, reductionCost)) {
                        pq.push(child);
                    }
                }
            }
        }
    }

public:
    TSPSolver(const vector<vector<int>>& inputMatrix) : matrix(inputMatrix) {
        n = matrix.size();
    }

    int getBestCost() {
        return bestCost;
    }

    const vector<int>& getBestPath() const {
        return bestPath;
    }

    void run() {
        solve();
    }
};

vector<vector<int>> readMatrixFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Nie mozna otworzyc pliku!" << endl;
        exit(1);
    }

    int n;
    file >> n;
    vector<vector<int>> matrix(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> matrix[i][j];
            if (matrix[i][j] == 999999) {
                matrix[i][j] = INF;
            }
        }
    }

    file.close();
    return matrix;
}

vector<vector<int>> readMatrixFromInput() {
    int n;
    cout << "Podaj rozmiar macierzy: ";
    cin >> n;
    vector<vector<int>> matrix(n, vector<int>(n));

    cout << "Podaj elementy macierzy w jednym wierszu (uzyj 999999 dla INF, oddzielone spacja):" << endl;
    cin.ignore();  // Ignorowanie znaku nowej linii pozostałego w buforze
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        for (int j = 0; j < n; j++) {
            ss >> matrix[i][j];
            if (matrix[i][j] == 999999) {
                matrix[i][j] = INF;
            }
        }
    }

    return matrix;
}

int main() {
    vector<vector<int>> matrix;
    char choice;
    cout << "Czy chcesz wczytac macierz z pliku (p) czy wprowadzic recznie (r)? ";
    cin >> choice;

    if (choice == 'p') {
        string filename;
        cout << "Podaj nazwe pliku: ";
        cin >> filename;
        matrix = readMatrixFromFile(filename);
    }
    else {
        matrix = readMatrixFromInput();
    }

    TSPSolver solver(matrix);

    auto start = chrono::high_resolution_clock::now();
    solver.run();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "Minimum cost to complete the tour: " << solver.getBestCost() << endl;
    cout << "Path: ";
    for (int node : solver.getBestPath()) {
        cout << node << " ";
    }
    cout << endl;

    cout << "Time taken: " << elapsed.count() << " seconds" << endl;

    return 0;
}

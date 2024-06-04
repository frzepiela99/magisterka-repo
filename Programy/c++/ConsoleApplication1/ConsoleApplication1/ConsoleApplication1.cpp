#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

const int MAX_CITIES = 1000;
const int MAX_ANTS = 50;
const int MAX_ITERATIONS = 1000;
const double ALPHA = 1.0; // Wpływ feromonów
const double BETA = 5.0; // Wpływ odległości
const double EVAPORATION = 0.5;
const double Q = 100.0;

struct Edge {
    int from, to;
    double cost;
};

struct Ant {
    vector<int> tour;
    double tour_length;
};

vector<vector<double>> pheromones;
vector<vector<double>> distances;
vector<Edge> edges;
vector<Ant> ants;
int num_cities, num_edges;

void read_file(const string& filename) {
    ifstream file(filename);
    file >> num_cities >> num_edges;
    distances.resize(num_cities, vector<double>(num_cities, numeric_limits<double>::max()));
    pheromones.resize(num_cities, vector<double>(num_cities, 1.0)); // Inicjalizacja feromonów

    int from, to;
    double cost;
    for (int i = 0; i < num_edges; ++i) {
        file >> from >> to >> cost;
        edges.push_back({ from - 1, to - 1, cost });
        distances[from - 1][to - 1] = cost;
        distances[to - 1][from - 1] = cost;
    }
    file.close();
}

double distance(int city1, int city2) {
    return distances[city1][city2];
}

void initialize_ants() {
    ants.resize(MAX_ANTS);
    for (auto& ant : ants) {
        ant.tour.resize(num_cities);
        ant.tour_length = 0.0;
    }
}

void construct_solutions() {
    for (auto& ant : ants) {
        vector<bool> visited(num_cities, false);
        int start_city = rand() % num_cities;
        ant.tour[0] = start_city;
        visited[start_city] = true;

        for (int i = 1; i < num_cities; ++i) {
            int current_city = ant.tour[i - 1];
            double sum_prob = 0.0;
            vector<double> probs(num_cities, 0.0);

            for (int j = 0; j < num_cities; ++j) {
                if (!visited[j] && distances[current_city][j] != numeric_limits<double>::max()) {
                    probs[j] = pow(pheromones[current_city][j], ALPHA) * pow(1.0 / distance(current_city, j), BETA);
                    sum_prob += probs[j];
                }
            }

            double r = (double)rand() / RAND_MAX * sum_prob;
            double total = 0.0;
            for (int j = 0; j < num_cities; ++j) {
                if (!visited[j]) {
                    total += probs[j];
                    if (total >= r) {
                        ant.tour[i] = j;
                        visited[j] = true;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < num_cities - 1; ++i) {
            ant.tour_length += distance(ant.tour[i], ant.tour[i + 1]);
        }
        ant.tour_length += distance(ant.tour[num_cities - 1], ant.tour[0]);
    }
}

void update_pheromones() {
    for (int i = 0; i < num_cities; ++i) {
        for (int j = 0; j < num_cities; ++j) {
            pheromones[i][j] *= (1.0 - EVAPORATION);
        }
    }

    for (const auto& ant : ants) {
        double contribution = Q / ant.tour_length;
        for (int i = 0; i < num_cities - 1; ++i) {
            int from = ant.tour[i];
            int to = ant.tour[i + 1];
            pheromones[from][to] += contribution;
            pheromones[to][from] += contribution;
        }
        pheromones[ant.tour[num_cities - 1]][ant.tour[0]] += contribution;
        pheromones[ant.tour[0]][ant.tour[num_cities - 1]] += contribution;
    }
}

int main() {
    srand(time(0));
    read_file("C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\magisterka-repo\\Programy\\c++\\generator-uniwersalny\\generator-uniwersalny\\edges10.txt");
    initialize_ants();

    for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
        construct_solutions();
        update_pheromones();

        double best_length = numeric_limits<double>::max();
        for (const auto& ant : ants) {
            if (ant.tour_length < best_length) {
                best_length = ant.tour_length;
            }
        }
        cout << "Iteration " << iteration + 1 << ": Best tour length = " << best_length << endl;
    }

    return 0;
}

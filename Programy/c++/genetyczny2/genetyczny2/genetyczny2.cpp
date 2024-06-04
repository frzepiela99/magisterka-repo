#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <chrono>
#include <numeric>
#include <cmath>

using namespace std;
using namespace chrono;

const int POPULATION_SIZE = 100;
const int GENERATIONS = 500;
const double MUTATION_RATE = 0.1;
int CITY_COUNT;

struct Edge {
    int city1, city2, distance;
};

vector<Edge> edges;
unordered_map<int, unordered_map<int, int>> distanceMatrix;

void loadEdges(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        exit(1);
    }

    int edgeCount;
    file >> CITY_COUNT >> edgeCount;

    edges.resize(edgeCount);
    for (int i = 0; i < edgeCount; ++i) {
        int city1, city2, distance;
        file >> city1 >> city2 >> distance;
        edges[i] = { city1, city2, distance };
        distanceMatrix[city1][city2] = distance;
        distanceMatrix[city2][city1] = distance;  // Because the graph is undirected
    }
}

double routeDistance(const vector<int>& route) {
    double totalDistance = 0;
    for (int i = 0; i < route.size() - 1; ++i) {
        totalDistance += distanceMatrix[route[i]][route[i + 1]];
    }
    totalDistance += distanceMatrix[route.back()][route.front()];
    return totalDistance;
}

double averageDistance(const vector<double>& distances) {
    return accumulate(distances.begin(), distances.end(), 0.0) / distances.size();
}

double standardDeviation(const vector<double>& distances, double mean) {
    double sum = 0;
    for (double distance : distances) {
        sum += (distance - mean) * (distance - mean);
    }
    return sqrt(sum / distances.size());
}

vector<int> generateIndividual(int n) {
    vector<int> individual(n);
    for (int i = 0; i < n; ++i) {
        individual[i] = i + 1;
    }
    random_shuffle(individual.begin(), individual.end());
    return individual;
}

vector<vector<int>> generatePopulation(int populationSize, int cityCount) {
    vector<vector<int>> population(populationSize);
    for (int i = 0; i < populationSize; ++i) {
        population[i] = generateIndividual(cityCount);
    }
    return population;
}

vector<int> crossover(const vector<int>& parent1, const vector<int>& parent2) {
    int start = rand() % parent1.size();
    int end = start + rand() % (parent1.size() - start);

    vector<int> child(parent1.size(), -1);
    for (int i = start; i < end; ++i) {
        child[i] = parent1[i];
    }

    int current = 0;
    for (int i = 0; i < parent2.size(); ++i) {
        if (find(child.begin(), child.end(), parent2[i]) == child.end()) {
            while (child[current] != -1) {
                current++;
            }
            child[current] = parent2[i];
        }
    }

    return child;
}

void mutate(vector<int>& individual) {
    if ((double)rand() / RAND_MAX < MUTATION_RATE) {
        int i = rand() % individual.size();
        int j = rand() % individual.size();
        swap(individual[i], individual[j]);
    }
}

vector<int> tournamentSelection(const vector<vector<int>>& population, const vector<double>& fitness) {
    int tournamentSize = 5;
    int best = rand() % population.size();
    for (int i = 0; i < tournamentSize - 1; ++i) {
        int contender = rand() % population.size();
        if (fitness[contender] < fitness[best]) {
            best = contender;
        }
    }
    return population[best];
}

int main() {
    srand(time(0));

    // Wczytanie danych z pliku
    loadEdges("C:\\Users\\mcmys\\source\\repos\\ConsoleApplication6\\ConsoleApplication6\\edges1000.txt");

    vector<vector<int>> population = generatePopulation(POPULATION_SIZE, CITY_COUNT);
    vector<double> fitness(POPULATION_SIZE);

    ofstream logFile("genetic_algorithm_log.txt");
    if (!logFile.is_open()) {
        cerr << "Could not open the log file!" << endl;
        exit(1);
    }

    auto startTime = high_resolution_clock::now();

    for (int generation = 0; generation < GENERATIONS; ++generation) {
        auto genStartTime = high_resolution_clock::now();

        for (int i = 0; i < POPULATION_SIZE; ++i) {
            fitness[i] = routeDistance(population[i]);
        }

        vector<vector<int>> newPopulation;

        int mutationCount = 0;
        int crossoverCount = 0;

        for (int i = 0; i < POPULATION_SIZE / 2; ++i) {
            vector<int> parent1 = tournamentSelection(population, fitness);
            vector<int> parent2 = tournamentSelection(population, fitness);

            vector<int> child1 = crossover(parent1, parent2);
            vector<int> child2 = crossover(parent2, parent1);
            crossoverCount += 2;

            mutate(child1);
            mutate(child2);
            mutationCount += 2;

            newPopulation.push_back(child1);
            newPopulation.push_back(child2);
        }

        for (int i = 0; i < newPopulation.size(); ++i) {
            fitness[i] = routeDistance(newPopulation[i]);
        }

        sort(newPopulation.begin(), newPopulation.end(), [&](const vector<int>& a, const vector<int>& b) {
            return routeDistance(a) < routeDistance(b);
            });

        population = vector<vector<int>>(newPopulation.begin(), newPopulation.begin() + POPULATION_SIZE);

        auto genEndTime = high_resolution_clock::now();
        duration<double> genTime = genEndTime - genStartTime;

        double bestFitness = routeDistance(population[0]);
        double worstFitness = routeDistance(population.back());
        double avgFitness = averageDistance(fitness);
        double stdDevFitness = standardDeviation(fitness, avgFitness);

        if (generation % 10 == 0 || generation == GENERATIONS - 1) {
            cout << "Generation " << generation << ": Best = " << bestFitness << " (Time: " << genTime.count() << "s)" << endl;
            logFile << "Generation " << generation << ": Best = " << bestFitness << " (Time: " << genTime.count() << "s)" << endl;
            logFile << "Worst = " << worstFitness << ", Average = " << avgFitness << ", StdDev = " << stdDevFitness << endl;
            logFile << "Mutations = " << mutationCount << ", Crossovers = " << crossoverCount << endl;
        }
    }

    auto endTime = high_resolution_clock::now();
    duration<double> totalTime = endTime - startTime;

    // Znalezienie najlepszego rozwiązania
    int bestIndex = 0;
    cout << "Best route: ";
    logFile << "Best route: ";
    for (int city : population[bestIndex]) {
        cout << city << " ";
        logFile << city << " ";
    }
    cout << "\nBest distance: " << routeDistance(population[bestIndex]) << endl;
    logFile << "\nBest distance: " << routeDistance(population[bestIndex]) << endl;
    cout << "Total execution time: " << totalTime.count() << "s" << endl;
    logFile << "Total execution time: " << totalTime.count() << "s" << endl;

    logFile.close();

    return 0;
}

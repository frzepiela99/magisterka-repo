#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <limits>
#include <fstream>
#include <sstream>
#include <numeric>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int POPULATION_SIZE = 100;
const int GENERATIONS = 1000;
const double MUTATION_RATE = 0.01;

// Struktura reprezentująca krawędź
struct Edge {
    int node1;
    int node2;
    double distance;
};

// Struktura reprezentująca chromosom (ścieżkę)
struct Chromosome {
    vector<int> path;
    double fitness;
};

// Funkcja oceny chromosomu (tu: długość ścieżki)
double evaluate(const Chromosome& chromosome, const vector<vector<double>>& distances) {
    double totalDistance = 0;
    for (size_t i = 0; i < chromosome.path.size() - 1; ++i) {
        totalDistance += distances[chromosome.path[i]][chromosome.path[i + 1]];
    }
    totalDistance += distances[chromosome.path.back()][chromosome.path.front()]; // powrót do startu
    return totalDistance;
}

// Funkcja inicjalizująca populację
void initializePopulation(vector<Chromosome>& population, int numberOfNodes) {
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        Chromosome chromosome;
        chromosome.path.resize(numberOfNodes);
        for (int j = 0; j < numberOfNodes; ++j) {
            chromosome.path[j] = j;
        }
        random_shuffle(chromosome.path.begin(), chromosome.path.end());
        population.push_back(chromosome);
    }
}

// Funkcja selekcji chromosomu metodą ruletki
Chromosome rouletteWheelSelection(const vector<Chromosome>& population) {
    double totalFitness = 0;
    for (const auto& chrom : population) {
        totalFitness += 1 / chrom.fitness;
    }

    double randValue = (double)rand() / RAND_MAX * totalFitness;
    double partialSum = 0;

    for (const auto& chrom : population) {
        partialSum += 1 / chrom.fitness;
        if (partialSum >= randValue) {
            return chrom;
        }
    }

    return population.back();
}

// Funkcja krzyżowania (crossover)
pair<Chromosome, Chromosome> crossover(const Chromosome& parent1, const Chromosome& parent2) {
    int size = parent1.path.size();
    int crossoverPoint = rand() % size;

    Chromosome offspring1, offspring2;
    offspring1.path = vector<int>(size, -1);
    offspring2.path = vector<int>(size, -1);

    for (int i = 0; i < crossoverPoint; ++i) {
        offspring1.path[i] = parent1.path[i];
        offspring2.path[i] = parent2.path[i];
    }

    int currentIndex1 = crossoverPoint, currentIndex2 = crossoverPoint;
    for (int i = 0; i < size; ++i) {
        if (find(offspring1.path.begin(), offspring1.path.end(), parent2.path[i]) == offspring1.path.end()) {
            offspring1.path[currentIndex1++] = parent2.path[i];
        }
        if (find(offspring2.path.begin(), offspring2.path.end(), parent1.path[i]) == offspring2.path.end()) {
            offspring2.path[currentIndex2++] = parent1.path[i];
        }
    }

    return { offspring1, offspring2 };
}

// Funkcja mutacji
void mutate(Chromosome& chromosome) {
    if ((double)rand() / RAND_MAX < MUTATION_RATE) {
        int index1 = rand() % chromosome.path.size();
        int index2 = rand() % chromosome.path.size();
        swap(chromosome.path[index1], chromosome.path[index2]);
    }
}

int main() {
    srand(time(0));

    ifstream file("C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\magisterka-repo\\Programy\\testy\\edges1000.txt");
    if (!file) {
        cerr << "Nie można otworzyć pliku!" << endl;
        return 1;
    }

    int numberOfNodes, numberOfEdges;
    file >> numberOfNodes >> numberOfEdges;

    vector<Edge> edges(numberOfEdges);
    for (int i = 0; i < numberOfEdges; ++i) {
        file >> edges[i].node1 >> edges[i].node2 >> edges[i].distance;
    }

    vector<vector<double>> distances(numberOfNodes, vector<double>(numberOfNodes, numeric_limits<double>::max()));
    for (const auto& edge : edges) {
        distances[edge.node1 - 1][edge.node2 - 1] = edge.distance;
        distances[edge.node2 - 1][edge.node1 - 1] = edge.distance;
    }

    vector<Chromosome> population;
    initializePopulation(population, numberOfNodes);

    for (auto& chrom : population) {
        chrom.fitness = evaluate(chrom, distances);
    }

    auto start = high_resolution_clock::now();

    for (int generation = 0; generation < GENERATIONS; ++generation) {
        vector<Chromosome> newPopulation;

        for (int i = 0; i < POPULATION_SIZE / 2; ++i) {
            Chromosome parent1 = rouletteWheelSelection(population);
            Chromosome parent2 = rouletteWheelSelection(population);

            pair<Chromosome, Chromosome> offspring = crossover(parent1, parent2);
            Chromosome& offspring1 = offspring.first;
            Chromosome& offspring2 = offspring.second;

            mutate(offspring1);
            mutate(offspring2);

            offspring1.fitness = evaluate(offspring1, distances);
            offspring2.fitness = evaluate(offspring2, distances);

            newPopulation.push_back(offspring1);
            newPopulation.push_back(offspring2);

            cout << "Krzyżowanie rodziców: [" << parent1.path[0] << ", " << parent2.path[0] << "] -> potomkowie: ["
                << offspring1.path[0] << ", " << offspring2.path[0] << "]" << endl;
        }

        population = newPopulation;

        if (generation % 100 == 0) {
            Chromosome bestInGeneration = *min_element(population.begin(), population.end(), [](const Chromosome& a, const Chromosome& b) {
                return a.fitness < b.fitness;
                });
            cout << "Pokolenie " << generation << " najlepszy chromosom: długość ścieżki = " << bestInGeneration.fitness << endl;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    Chromosome best = *min_element(population.begin(), population.end(), [](const Chromosome& a, const Chromosome& b) {
        return a.fitness < b.fitness;
        });

    cout << "Najlepsza ścieżka: ";
    for (int node : best.path) {
        cout << node + 1 << " ";
    }
    cout << "\nDługość ścieżki: " << best.fitness << endl;
    cout << "Czas wykonania: " << duration.count() << " ms" << endl;

    return 0;
}

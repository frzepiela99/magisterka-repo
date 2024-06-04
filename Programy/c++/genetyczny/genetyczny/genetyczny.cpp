#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <iomanip> // Dodane dla formatowania wyjścia

using namespace std;

// Definicje i stałe
const int NUM_CITIES = 10;
const int POPULATION_SIZE = 10;
const int NUM_GENERATIONS = 100;
const int MUTATION_RATE = 1; // 1% szansa na mutację

// Struktura reprezentująca osobnika
struct Individual {
    vector<int> path;
    double fitness;

    Individual(vector<int> p) : path(p), fitness(0) {}
};

// Funkcja do obliczania dystansu (przykładowa funkcja kosztu)
double calculateDistance(const vector<int>& path) {
    double distance = 0;
    for (size_t i = 1; i < path.size(); i++) {
        distance += abs(path[i] - path[i - 1]); // Przykładowy koszt
    }
    distance += abs(path.front() - path.back()); // Zamknięcie cyklu
    return distance;
}

// Funkcja do oceny osobnika
void evaluateFitness(Individual& individual) {
    individual.fitness = 1.0 / calculateDistance(individual.path); // Zmiana sposobu oceny fitness
}

// Funkcja do generowania losowej ścieżki
vector<int> generateRandomPath() {
    vector<int> path(NUM_CITIES);
    for (int i = 0; i < NUM_CITIES; ++i) {
        path[i] = i;
    }
    random_shuffle(path.begin(), path.end());
    return path;
}

// Funkcja do krzyżowania dwóch osobników
Individual crossover(const Individual& parent1, const Individual& parent2) {
    vector<int> childPath(NUM_CITIES, -1);
    int start = rand() % NUM_CITIES;
    int end = start + rand() % (NUM_CITIES - start);

    for (int i = start; i < end; ++i) {
        childPath[i] = parent1.path[i];
    }

    int index = 0;
    for (int i = 0; i < NUM_CITIES; ++i) {
        if (find(childPath.begin(), childPath.end(), parent2.path[i]) == childPath.end()) {
            while (childPath[index] != -1) {
                ++index;
            }
            childPath[index] = parent2.path[i];
        }
    }
    return Individual(childPath);
}

// Funkcja do mutacji osobnika
void mutate(Individual& individual) {
    if (rand() % 100 < MUTATION_RATE) {
        int index1 = rand() % NUM_CITIES;
        int index2 = rand() % NUM_CITIES;
        swap(individual.path[index1], individual.path[index2]);
    }
}

// Funkcja do inicjalizacji populacji
vector<Individual> initializePopulation() {
    vector<Individual> population;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        vector<int> path = generateRandomPath();
        population.push_back(Individual(path));
    }
    return population;
}

// Funkcja do wypisywania ścieżki
void printPath(const vector<int>& path) {
    for (int city : path) {
        cout << city << " ";
    }
    cout << endl;
}

// Funkcja główna algorytmu genetycznego
Individual geneticAlgorithm() {
    srand(time(nullptr));

    vector<Individual> population = initializePopulation();
    for (auto& individual : population) {
        evaluateFitness(individual);
    }

    cout << "Początkowa populacja:" << endl;
    for (const auto& individual : population) {
        printPath(individual.path);
        cout << "Fitness: " << individual.fitness << endl;
    }

    for (int generation = 0; generation < NUM_GENERATIONS; ++generation) {
        vector<Individual> newPopulation;

        // Selekcja rodziców i tworzenie nowej populacji
        for (int i = 0; i < POPULATION_SIZE / 2; ++i) {
            int parent1Index = rand() % POPULATION_SIZE;
            int parent2Index = rand() % POPULATION_SIZE;
            Individual child = crossover(population[parent1Index], population[parent2Index]);
            mutate(child);
            evaluateFitness(child);
            newPopulation.push_back(child);
        }

        // Połączenie starej i nowej populacji
        population.insert(population.end(), newPopulation.begin(), newPopulation.end());

        // Sortowanie populacji według fitness
        sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitness > b.fitness; // Fitness jest teraz odwrotnością dystansu
            });

        // Usunięcie najgorszych osobników
        population.resize(POPULATION_SIZE);

        cout << "Pokolenie " << generation + 1 << ":" << endl;
        for (const auto& individual : population) {
            printPath(individual.path);
            cout << "Fitness: " << individual.fitness << endl;
        }
    }

    // Zwrócenie najlepszego osobnika
    return *max_element(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
        return a.fitness < b.fitness;
        });
}

// Funkcja główna programu
int main() {
    Individual best = geneticAlgorithm();
    cout << "Najlepsza ścieżka: ";
    printPath(best.path);
    cout << "Najlepsza wartość fitness: " << best.fitness << endl;
    return 0;
}

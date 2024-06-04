#include <fstream>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void generateEdgesFile(int N) {
    int M = N * (N - 1) / 2; // liczba krawędzi dla grafu pełnego

    // Rozpoczęcie pomiaru czasu
    auto start = chrono::high_resolution_clock::now();

    // Użycie std::stringstream do stworzenia nazwy pliku
    stringstream ss;
    ss << "edges" << N << ".txt";
    string fileName = ss.str();

    ofstream dane(fileName);
    dane << N << ' ' << M << endl;
    srand(0);
    for (int i = 1; i <= N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            dane << i << ' ' << j << ' ' << rand() % 999 + 1 + (rand() % 999 * 0.001) << endl;
        }
    }
    dane.close();

    // Zakończenie pomiaru czasu
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Wyświetlenie czasu wykonania na konsoli
    cout << "Plik " << fileName << " wygenerowany. Czas wykonania: " << elapsed.count() << " sekund" << endl;
}

int main() {
    // Rozpoczęcie pomiaru całkowitego czasu
    auto startTotal = chrono::high_resolution_clock::now();

    int numFiles;
    cout << "Podaj liczbę plików do wygenerowania: ";
    cin >> numFiles;

    int N;
    cout << "Podaj liczbę wierzchołków dla każdego pliku: ";
    cin >> N;

    for (int i = 1; i <= numFiles; ++i) {
        generateEdgesFile(N);
    }

    // Zakończenie pomiaru całkowitego czasu
    auto endTotal = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedTotal = endTotal - startTotal;

    // Wyświetlenie całkowitego czasu wykonania na konsoli
    cout << "Całkowity czas wykonania programu: " << elapsedTotal.count() << " sekund" << endl;

    return 0;
}

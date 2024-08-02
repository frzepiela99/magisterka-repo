#include <fstream>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

void generateEdgesFile(int N) {
    int M = N * (N - 1) / 2;

    auto start = chrono::high_resolution_clock::now();

    stringstream ss;
    ss << "edges" << N << ".in";
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

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "Plik " << fileName << " wygenerowany. Czas wykonania: " << elapsed.count() << " sekund" << endl;
}

int main() {
    auto startTotal = chrono::high_resolution_clock::now();

    int numFiles;
    cout << "Podaj liczbe plikow do wygenerowania: ";
    cin >> numFiles;

    int N;
    cout << "Podaj liczbe wierzcholkow dla kazdego pliku: ";
    cin >> N;

    for (int i = 1; i <= numFiles; ++i) {
        generateEdgesFile(N);
    }

    auto endTotal = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedTotal = endTotal - startTotal;

    cout << "Calkowity czas wykonania programu: " << elapsedTotal.count() << " sekund" << endl;

    return 0;
}

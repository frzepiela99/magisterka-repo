#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

const int INF = 999999; // Używamy 999999 do oznaczania nieskończoności

void generateRandomMatrix(int size, int minValue, int maxValue, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Nie mozna otworzyc pliku!" << endl;
        exit(1);
    }

    file << size << endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j) {
                file << INF; // Reprezentacja INF
            }
            else {
                int weight = minValue + rand() % (maxValue - minValue + 1);
                file << weight;
            }
            if (j < size - 1) {
                file << " ";
            }
        }
        file << endl;
    }

    file.close();
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    int size;
    int minValue, maxValue;
    string filename;

    cout << "Podaj rozmiar macierzy: ";
    cin >> size;
    cout << "Podaj minimalna wartosc wagi krawedzi: ";
    cin >> minValue;
    cout << "Podaj maksymalna wartosc wagi krawedzi: ";
    cin >> maxValue;
    cout << "Podaj nazwe pliku, do ktorego zapisac macierz: ";
    cin >> filename;

    generateRandomMatrix(size, minValue, maxValue, filename);

    cout << "Macierz została wygenerowana i zapisana do pliku " << filename << endl;

    return 0;
}

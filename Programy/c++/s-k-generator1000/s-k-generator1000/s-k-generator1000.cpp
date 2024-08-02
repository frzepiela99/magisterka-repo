#include <fstream>
#include <cstdlib>
#include <chrono>
#include <iostream>
using namespace std;

int N = 1000, M = 499500;

int main()
{
    // Start measuring time
    auto start = chrono::high_resolution_clock::now();

    ofstream dane("edges1000.in");
    dane << N << ' ' << M << endl;
    srand(0);
    for (int i = 1; i <= N; ++i)
        for (int j = i + 1; j <= N; ++j)
            dane << i << ' ' << j << ' ' <<
            rand() % 999 + 1 + (rand() % 999 * 0.001) << endl;
    dane.close();

    // Stop measuring time
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Output the time to the console
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;

    return 0;
}

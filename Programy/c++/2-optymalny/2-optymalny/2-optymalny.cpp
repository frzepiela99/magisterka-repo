#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>  // Dodano do użycia std::setprecision

using namespace std;
using namespace std::chrono;

int N, M;
int* Ptr;
int** W;
int* Route;
int Ahead, I, I1, I2, Index, J, J1, J2, Last, Limit, Max, Max1, Next, S1, S2, T1, T2;
int Tweight = 0;
int TotalWeight = 0;

void Initialization()
{
    int X, Y, Temp;
    ifstream date("C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\magisterka-repo\\Programy\\testy\\edges100.in");
    date >> N >> M;
    Ptr = new int[N + 1];
    Route = new int[N + 1];
    W = new int* [N + 1];
    for (int i = 0; i <= N; ++i) W[i] = new int[N + 1];
    for (int i = 1; i <= M; ++i)
    {
        date >> X >> Y >> Temp;
        W[X][Y] = Temp;
        W[Y][X] = Temp;
        TotalWeight += Temp;
    }
    for (int i = 1; i <= N; ++i) Route[i] = i, W[i][i] = 0;
    for (int i = 1; i < N; ++i) Tweight += W[i][i + 1];
    Tweight += W[N][1];
}

void Calculations()
{
    for (I = 1; I < N; ++I) Ptr[Route[I]] = Route[I + 1];
    Ptr[Route[N]] = Route[1];
    do
    {
        Max = 0, I1 = 1;
        for (int I = 1; I <= N - 2; ++I)
        {
            if (I == 1) Limit = N - 1; else Limit = N;
            I2 = Ptr[I1], J1 = Ptr[I2];
            for (J = I + 2; J <= Limit; ++J)
            {
                J2 = Ptr[J1], Max1 = W[I1][I2] + W[J1][J2] - (W[I1][J1] + W[I2][J2]);
                if (Max1 > Max) S1 = I1, S2 = I2, T1 = J1, T2 = J2, Max = Max1;
                J1 = J2;
            }
            I1 = I2;
        }
        if (Max > 0)
        {
            Ptr[S1] = T1, Next = S2, Last = T2;
            do Ahead = Ptr[Next], Ptr[Next] = Last, Last = Next, Next = Ahead; while (Next != T2);
            Tweight -= Max;
        }
    } while (Max != 0);
    Index = 1;
    for (I = 1; I <= N; ++I) Route[I] = Index, Index = Ptr[Index];
}

int main()
{
    auto start = high_resolution_clock::now();

    Initialization();

    cout << "Displaying a portion of the weight matrix for verification:" << endl;
    for (int k = 1; k <= min(10, N); ++k)
    {
        for (int l = 1; l <= min(10, N); ++l)
        {
            cout << W[k][l] << '\t';
        }
        cout << '\n';
    }

    Calculations();

    cout << "Total weight of the route: " << Tweight << endl;
    cout << "Route: ";
    for (int i = 1; i <= N; ++i) cout << Route[i] << "  ";
    cout << endl;

    cout << "Sum of all edge weights in the graph: " << TotalWeight << endl;

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    double seconds = duration / 1e6;
    cout << "Execution time: " << fixed << setprecision(3) << seconds << " s" << endl;

    ofstream outputFile("output.csv");
    outputFile << "Total weight of the route," << Tweight << "\n";
    outputFile << "Route,";
    for (int i = 1; i <= N; ++i) outputFile << Route[i] << (i < N ? "," : "\n");
    outputFile << "Sum of all edge weights in the graph," << TotalWeight << "\n";
    outputFile << "Execution time," << fixed << setprecision(3) << seconds << " s\n";
    outputFile.close();

    cin.get();
    return 0;
}

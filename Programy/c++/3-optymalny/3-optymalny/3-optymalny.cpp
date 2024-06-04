#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int N, M;
int* Ptr;
int** W;
int* Route;
int Ahead, I, J, K, Index, Limit, Max;
int Tweight = 0;
int TotalWeight = 0; // Zmienna do przechowywania sumy wag wszystkich krawędzi
enum SwapType { Asym, Sym };
struct SwapData
{
    int X1, X2, Y1, Y2, Z1, Z2, Gain;
    SwapType Choice;
} BestSwap, Swap;

void SwapCheck(SwapData& Swap)
{
    int DelWeight, Max;
    Swap.Gain = 0;
    DelWeight = W[Swap.X1][Swap.X2] + W[Swap.Y1][Swap.Y2] + W[Swap.Z1][Swap.Z2];
    Max = DelWeight - (W[Swap.Y1][Swap.X1] + W[Swap.Z1][Swap.X2] + W[Swap.Z2][Swap.Y2]);
    if (Max > Swap.Gain) Swap.Gain = Max, Swap.Choice = Asym;
    Max = DelWeight - (W[Swap.X1][Swap.Y2] + W[Swap.Z1][Swap.X2] + W[Swap.Y1][Swap.Z2]);
    if (Max > Swap.Gain) Swap.Gain = Max, Swap.Choice = Sym;
}

void Reverse(int start, int finish)
{
    int ahead, last, next;
    if (start != finish) last = start, next = Ptr[last];
    do ahead = Ptr[next], Ptr[next] = last, last = next, next = ahead; while (last != finish);
}

void Initialization()
{
    int X, Y, Temp;
    ifstream data("C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\THREEOPT\\edges300.in");
    data >> N >> M;
    Ptr = new int[N + 1];
    Route = new int[N + 1];
    W = new int* [N + 1];
    for (int i = 0; i <= N; ++i) W[i] = new int[N + 1];
    for (int i = 1; i <= M; ++i)
    {
        data >> X >> Y >> Temp;
        W[X][Y] = Temp;
        W[Y][X] = Temp;
        TotalWeight += Temp; // Dodaj wagę krawędzi do całkowitej sumy wag
    }
    for (int i = 1; i <= N; ++i) Route[i] = i, W[i][i] = 0;
    for (int i = 1; i < N; ++i) Tweight += W[i][i + 1];
    Tweight += W[N][1];
    data.close();
}

void Calculations()
{
    for (I = 1; I < N; ++I) Ptr[Route[I]] = Route[I + 1];
    Ptr[Route[N]] = Route[1];
    do
    {
        BestSwap.Gain = 0, Swap.X1 = 1;
        for (int I = 1; I <= N; ++I)
        {
            Swap.X2 = Ptr[Swap.X1], Swap.Y1 = Swap.X2;
            for (J = 2; J <= N - 3; ++J)
            {
                Swap.Y2 = Ptr[Swap.Y1], Swap.Z1 = Ptr[Swap.Y2];
                for (K = J + 2; K <= N - 1; ++K)
                {
                    Swap.Z2 = Ptr[Swap.Z1];
                    SwapCheck(Swap);
                    if (Swap.Gain > BestSwap.Gain) BestSwap = Swap;
                    Swap.Z1 = Swap.Z2;
                }
                Swap.Y1 = Swap.Y2;
            }
            Swap.X1 = Swap.X2;
        }
        if (BestSwap.Gain > 0)
        {
            if (BestSwap.Choice == Asym)
            {
                Reverse(BestSwap.Z2, BestSwap.X1);
                Ptr[BestSwap.Y1] = BestSwap.X1, Ptr[BestSwap.Z2] = BestSwap.Y2;
            }
            else Ptr[BestSwap.X1] = BestSwap.Y2, Ptr[BestSwap.Y1] = BestSwap.Z2;
            Ptr[BestSwap.Z1] = BestSwap.X2;
            Tweight -= BestSwap.Gain;
        }
    } while (BestSwap.Gain);
    Index = 1;
    for (int I = 1; I <= N; ++I) Route[I] = Index, Index = Ptr[Index];
}

int main()
{
    auto start = high_resolution_clock::now(); // Rozpocznij pomiar czasu

    Initialization();
    Calculations();
    cout << "Total weight of the route: " << Tweight << endl;
    cout << "Route: ";
    for (int i = 1; i <= N; ++i) cout << Route[i] << "  ";
    cout << endl;

    int EdgeSum = 0;
    for (int i = 1; i < N; ++i)
        EdgeSum += W[Route[i]][Route[i + 1]];
    EdgeSum += W[Route[N]][Route[1]]; // Dodaj ostatnią krawędź do sumy

    cout << "Sum of all edge lengths in the route: " << EdgeSum << endl;
    cout << "Sum of all edge weights in the graph: " << TotalWeight << endl;

    auto end = high_resolution_clock::now(); // Zakończ pomiar czasu
    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << "Execution time: " << duration << " ms" << endl;

    cin.get();
    return 0;
}

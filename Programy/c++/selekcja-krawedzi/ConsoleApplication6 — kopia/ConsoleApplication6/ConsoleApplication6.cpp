#include <iostream>
#include <fstream>
#include <set>
#include <cmath>
#include <chrono> // Dodane do mierzenia czasu

using namespace std;
using namespace std::chrono;

int N, M;    // liczba wierzchołków i krawędzi
double W = 0; // suma wag krawędzi w MST
double L = 0; // suma długości krawędzi w MST

struct edge // krawędź
{
    unsigned int v1, v2; // numery wierzchołków
    double weight;       // waga
} t;

class comp_edge
{
public:
    bool operator()(const edge& e1, const edge& e2) const
    {
        return e1.weight < e2.weight;
    }
};

typedef multiset<edge, comp_edge> Edge; // drzewo krawędzi
Edge edges;
Edge mst_edges; // Edges in the MST
Edge::iterator pos;

struct vertex // wierzchołek
{
    unsigned int row, v1, v2; // stopień, sąsiad1, sąsiad2
};

vertex* vs;

void date()
{
    ifstream dane("C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\magisterka-repo\\Programy\\testy\\edges1000.txt");
    dane >> M >> N;
    for (int i = 0; i < N; ++i)
    {
        dane >> t.v1 >> t.v2 >> t.weight;
        edges.insert(t);
    }
    vs = new vertex[M + 1];
    vertex temp;
    temp.row = 0, temp.v1 = 0, temp.v2 = 0;
    for (int i = 0; i <= M; ++i)
        vs[i] = temp;
    cout << M << ' ' << N << endl;
    dane.close();
}

bool nocycle(edge e)
{
    unsigned int v1 = e.v1, v2 = e.v2;
    v1 = vs[v1].v1 + vs[v1].v2;
    while (v1 != v2 && vs[v1].row == 2)
    {
        if (vs[v1].v1)
            v1 = vs[v1].v1;
        else
            v1 = vs[v1].v2;
    }
    return v1 != v2;
}

void build()
{
    pos = edges.begin();
    for (int i = 1; i <= M; ++i)
    {
        t = *pos;
        if (vs[t.v1].row == 2 || vs[t.v2].row == 2)
        {
            ++pos;
            --i;
            continue;
        }
        if (vs[t.v1].row + vs[t.v2].row <= 1)
        {
            if (vs[t.v1].v1)
                vs[t.v1].v1 = t.v1;
            else
                vs[t.v1].v2 = t.v1;
            if (vs[t.v2].v1)
                vs[t.v2].v1 = t.v2;
            else
                vs[t.v1].v2 = t.v2;
            ++vs[t.v1].row, ++vs[t.v2].row, ++pos;
            W += t.weight;
            L += abs(static_cast<int>(t.v1) - static_cast<int>(t.v2));
            mst_edges.insert(t); // Add edge to MST
            continue;
        }
        if (vs[t.v1].row + vs[t.v2].row == 2)
        {
            if (nocycle(t) || i == M)
            {
                cout << '\n'
                    << t.weight << ' ' << t.v1 << ' ' << t.v2 << " II " << i;
                ++vs[t.v1].row, ++vs[t.v2].row, ++pos;
                W += t.weight;
                L += abs(static_cast<int>(t.v1) - static_cast<int>(t.v2));
                mst_edges.insert(t); // Add edge to MST
            }
            else
            {
                ++pos;
                --i;
            }
            continue;
        }
    }
}

void generate_dot_file(const string& filename, const Edge& mst_edges)
{
    ofstream dotfile(filename);
    if (!dotfile)
    {
        cerr << "Could not open file " << filename << endl;
        exit(1);
    }
    dotfile << "graph G {\n";
    for (const auto& e : mst_edges)
    {
        dotfile << "    " << e.v1 << " -- " << e.v2 << " [label=\"" << e.weight << "\"];\n";
    }
    dotfile << "}\n";
    dotfile.close();
}

bool is_dot_available()
{
    int result = system("dot -V >nul 2>&1");
    return result == 0;
}

int main()
{
    // Rozpoczęcie mierzenia czasu
    auto start = high_resolution_clock::now();

    date();
    build();

    // Zakończenie mierzenia czasu
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << '\n'
        << "Suma wag krawedzi w MST: " << W << endl;
    cout << "Suma dlugosci krawedzi w MST: " << L << endl;
    cout << "Czas dzialania programu: " << duration.count() << " ms" << endl;

    // Generowanie pliku DOT
    string dot_filename = "graph.dot";
    generate_dot_file(dot_filename, mst_edges);

    // Sprawdzenie, czy narzędzie dot jest dostępne
    if (is_dot_available())
    {
        // Pełna ścieżka do narzędzia dot (zmień to na właściwą ścieżkę, jeśli Graphviz jest zainstalowany, ale nie w PATH)
        string dot_path = "\"C:\\Program Files\\Graphviz\\bin\\dot.exe\"";
        // Wywołanie narzędzia Graphviz do generowania obrazu
        string command = dot_path + " -Tpng graph.dot -o graph.png";
        system(command.c_str());
        cout << "Graph image has been generated as graph.png" << endl;
    }
    else
    {
        cerr << "'dot' is not recognized as an internal or external command, operable program or batch file." << endl;
    }

    cin.get();
    return 0;
}

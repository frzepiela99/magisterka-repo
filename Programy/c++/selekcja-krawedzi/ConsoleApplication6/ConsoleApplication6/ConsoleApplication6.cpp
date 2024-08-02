#include <iostream>
#include <fstream>
#include <set>
#include <cmath>
#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;

int N, M;    // Number of vertices and edges
double W = 0; // Sum of edge weights in MST

struct edge {
    unsigned int v1, v2;
    double weight;
} t;

class comp_edge {
public:
    bool operator()(const edge& e1, const edge& e2) const {
        return e1.weight < e2.weight;
    }
};

typedef multiset<edge, comp_edge> Edge;
Edge edges;
Edge::iterator pos;

struct vertex {
    unsigned int row, v1, v2;
};

vertex* vs;

void date() {
    ifstream dane("C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\magisterka-repo\\Programy\\testy\\edges100.in");
    dane >> M >> N;
    for (int i = 0; i < N; ++i) {
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

bool nocycle(edge e) {
    unsigned int v1 = e.v1, v2 = e.v2;
    v1 = vs[v1].v1 + vs[v1].v2;
    while (v1 != v2 && vs[v1].row == 2) {
        if (vs[v1].v1)
            v1 = vs[v1].v1;
        else
            v1 = vs[v1].v2;
    }
    return v1 != v2;
}

void build() {
    pos = edges.begin();
    for (int i = 1; i <= M; ++i) {
        t = *pos;
        if (vs[t.v1].row == 2 || vs[t.v2].row == 2) {
            ++pos;
            --i;
            continue;
        }
        if (vs[t.v1].row + vs[t.v2].row <= 1) {
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
            continue;
        }
        if (vs[t.v1].row + vs[t.v2].row == 2) {
            if (nocycle(t) || i == M) {
                cout << '\n' << t.weight << ' ' << t.v1 << ' ' << t.v2 << " II " << i;
                ++vs[t.v1].row, ++vs[t.v2].row, ++pos;
                W += t.weight;
            }
            else {
                ++pos;
                --i;
            }
            continue;
        }
    }
}

void save_to_csv(double weight_sum, double duration, int vertices) {
    ofstream csv_file;
    csv_file.open("result.csv", ios::out | ios::app);
    if (csv_file.is_open()) {
        if (csv_file.tellp() == 0) {
            csv_file << "Weight Sum,Execution Time (s),Vertices" << endl;
        }
        csv_file << weight_sum << "," << duration << "," << vertices << endl;
        csv_file.close();
    }
    else {
        cout << "Unable to open file to write CSV data." << endl;
    }
}

int main() {
    auto start = high_resolution_clock::now();

    date();
    build();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    double duration_seconds = duration.count() / 1000.0;

    cout << '\n' << "Sum of edge weights in MST: " << W << endl;
    cout << "Execution time: " << duration_seconds << " s" << endl;

    save_to_csv(W, duration_seconds, M);

    cin.get();
    return 0;
}



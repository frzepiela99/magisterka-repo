#!/usr/bin/env python
# coding: utf-8

# In[3]:


import time
from typing import List, Tuple
import csv

class Edge:
    def __init__(self, v1: int, v2: int, weight: float):
        self.v1 = v1
        self.v2 = v2
        self.weight = weight

    def __lt__(self, other):
        return self.weight < other.weight

class Vertex:
    def __init__(self):
        self.row = 0
        self.v1 = 0
        self.v2 = 0

def read_data(filename: str) -> Tuple[int, int, List[Edge], float]:
    edges = []
    total_weight = 0.0
    with open(filename, 'r') as file:
        M, N = map(int, file.readline().split())
        for _ in range(N):
            v1, v2, weight = map(float, file.readline().split())
            edges.append(Edge(int(v1), int(v2), weight))
            total_weight += weight  # Add the edge weight to the total weight
    return M, N, edges, total_weight

def nocycle(vs: List[Vertex], e: Edge) -> bool:
    v1, v2 = e.v1, e.v2
    v1 = vs[v1].v1 + vs[v1].v2
    while v1 != v2 and vs[v1].row == 2:
        if vs[v1].v1:
            v1 = vs[v1].v1
        else:
            v1 = vs[v1].v2
    return v1 != v2

def build(M: int, edges: List[Edge]) -> float:
    vs = [Vertex() for _ in range(M + 1)]
    edges.sort()
    W = 0.0

    i = 1
    for t in edges:
        if vs[t.v1].row == 2 or vs[t.v2].row == 2:
            continue
        if vs[t.v1].row + vs[t.v2].row <= 1:
            if vs[t.v1].v1:
                vs[t.v1].v1 = t.v1
            else:
                vs[t.v1].v2 = t.v1
            if vs[t.v2].v1:
                vs[t.v2].v1 = t.v2
            else:
                vs[t.v2].v2 = t.v2
            vs[t.v1].row += 1
            vs[t.v2].row += 1
            W += t.weight
            i += 1
            if i > M:
                break
        elif vs[t.v1].row + vs[t.v2].row == 2:
            if nocycle(vs, t) or i == M:
                print(f'\n{t.weight} {t.v1} {t.v2} II {i}')
                vs[t.v1].row += 1
                vs[t.v2].row += 1
                W += t.weight
                i += 1
                if i > M:
                    break

    return W

def save_to_csv(weight_sum: float, duration: float, vertices: int, total_weight: float):
    with open('result.csv', mode='a', newline='') as file:
        writer = csv.writer(file)
        file_is_empty = file.tell() == 0
        if file_is_empty:
            writer.writerow(["Weight Sum", "Execution Time (s)", "Vertices", "Total Weight"])
        writer.writerow([f'{weight_sum:.2f}', f'{duration:.6f}', vertices, f'{total_weight:.2f}'])

def main():
    start = time.time()

    M, N, edges, total_weight = read_data('C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\magisterka-repo\\Programy\\testy\\edges100.in')
    W = build(M, edges)

    end = time.time()
    duration = end - start

    print(f'\nSuma wag krawędzi w MST: {W:.2f}')
    print(f'Total weight of the path: {total_weight:.2f}')
    print(f'Czas działania programu: {duration:.6f} s')

    save_to_csv(W, duration, M, total_weight)

if __name__ == "__main__":
    main()


# In[ ]:





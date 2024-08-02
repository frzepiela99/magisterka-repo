#!/usr/bin/env python
# coding: utf-8

# In[2]:


import math
import sys
from heapq import heappop, heappush
from itertools import combinations
import time

class Graph:
    def __init__(self, V):
        self.V = V
        self.adjMatrix = [[0] * V for _ in range(V)]

    def add_edge(self, u, v, w):
        self.adjMatrix[u][v] = w
        self.adjMatrix[v][u] = w

def prim_mst(graph):
    V = graph.V
    mst = Graph(V)
    key = [float('inf')] * V
    parent = [-1] * V
    in_mst = [False] * V

    key[0] = 0.0

    for _ in range(V - 1):
        min_key = float('inf')
        u = -1

        for v in range(V):
            if not in_mst[v] and key[v] < min_key:
                min_key = key[v]
                u = v

        in_mst[u] = True

        for v in range(V):
            if graph.adjMatrix[u][v] and not in_mst[v] and graph.adjMatrix[u][v] < key[v]:
                parent[v] = u
                key[v] = graph.adjMatrix[u][v]

    mst_cost = 0.0
    for i in range(1, V):
        mst.add_edge(parent[i], i, graph.adjMatrix[parent[i]][i])
        mst_cost += graph.adjMatrix[parent[i]][i]

    return mst, mst_cost

def find_odd_degree_vertices(graph):
    odd_degree_vertices = []
    for i in range(graph.V):
        degree = sum(1 for j in range(graph.V) if graph.adjMatrix[i][j] != 0)
        if degree % 2 == 1:
            odd_degree_vertices.append(i)
    return odd_degree_vertices

def min_weight_matching(graph, odd_degree_vertices):
    matching = []
    n = len(odd_degree_vertices)
    visited = [False] * n

    for i in range(n):
        if not visited[i]:
            u = odd_degree_vertices[i]
            min_weight = float('inf')
            min_vertex = -1

            for j in range(n):
                if i != j and not visited[j] and graph.adjMatrix[u][odd_degree_vertices[j]] < min_weight:
                    min_weight = graph.adjMatrix[u][odd_degree_vertices[j]]
                    min_vertex = j

            visited[min_vertex] = True
            matching.append((u, odd_degree_vertices[min_vertex]))

    return matching

def christofides_algorithm(graph):
    mst, mst_cost = prim_mst(graph)
    odd_degree_vertices = find_odd_degree_vertices(mst)
    matching = min_weight_matching(graph, odd_degree_vertices)

    for u, v in matching:
        mst.add_edge(u, v, graph.adjMatrix[u][v])

    def find_eulerian_cycle(graph):
        stack = [0]
        eulerian_cycle = []
        adj_matrix_copy = [row[:] for row in graph.adjMatrix]

        while stack:
            u = stack[-1]
            has_unvisited = False

            for v in range(graph.V):
                if adj_matrix_copy[u][v] != 0:
                    stack.append(v)
                    adj_matrix_copy[u][v] = 0
                    adj_matrix_copy[v][u] = 0
                    has_unvisited = True
                    break

            if not has_unvisited:
                eulerian_cycle.append(u)
                stack.pop()

        return eulerian_cycle

    eulerian_cycle = find_eulerian_cycle(mst)
    hamiltonian_cycle = []
    visited = [False] * graph.V

    for v in eulerian_cycle:
        if not visited[v]:
            hamiltonian_cycle.append(v)
            visited[v] = True

    tour_cost = 0
    for i in range(len(hamiltonian_cycle) - 1):
        tour_cost += graph.adjMatrix[hamiltonian_cycle[i]][hamiltonian_cycle[i + 1]]
    tour_cost += graph.adjMatrix[hamiltonian_cycle[-1]][hamiltonian_cycle[0]]

    return hamiltonian_cycle, tour_cost, mst_cost

def save_results(path, total_cost, mst_cost, duration):
    with open("results.txt", "w") as out_file:
        out_file.write("Path: ")
        out_file.write(" ".join(str(v + 1) for v in path))
        out_file.write("\n")
        out_file.write(f"Total cost of the tour: {total_cost}\n")
        out_file.write(f"Total cost of the MST: {mst_cost}\n")
        out_file.write(f"Execution time: {duration:.3f} seconds\n")

    with open("results.csv", "w") as csv_file:
        csv_file.write("Vertex\n")
        for v in path:
            csv_file.write(f"{v + 1}\n")
        csv_file.write(f"Total cost of the tour,{total_cost}\n")
        csv_file.write(f"Total cost of the MST,{mst_cost}\n")
        csv_file.write(f"Execution time (seconds),{duration:.3f}\n")

def main():
    with open("C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\magisterka-repo\\Programy\\BADANIA\\Dane_wejsciowe\\edges1000.in") as input_file:
        V, E = map(int, input_file.readline().split())
        graph = Graph(V)

        for line in input_file:
            u, v, w = map(float, line.split())
            graph.add_edge(int(u) - 1, int(v) - 1, w)

    start = time.time()

    tour, tour_cost, mst_cost = christofides_algorithm(graph)

    duration = time.time() - start

    print("Znaleziona trasa:", " ".join(str(v + 1) for v in tour), tour[0] + 1)
    print("Suma wag krawedzi trasy:", tour_cost)
    print("Suma wag krawedzi w MST:", mst_cost)
    print("Czas dzialania programu:", f"{duration:.9f} sekund")

    save_results(tour, tour_cost, mst_cost, duration)

if __name__ == "__main__":
    main()


# In[ ]:





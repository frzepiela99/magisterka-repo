#!/usr/bin/env python
# coding: utf-8

# In[3]:


import heapq
import sys
import time

INF = sys.maxsize

class TSPSolver:
    def __init__(self, input_matrix):
        self.matrix = input_matrix
        self.best_path = []
        self.n = len(self.matrix)
        self.best_cost = INF

    class Node:
        def __init__(self, reduced_matrix, path, cost, vertex, level):
            self.reduced_matrix = reduced_matrix
            self.path = path
            self.cost = cost
            self.vertex = vertex
            self.level = level

        def __lt__(self, other):
            return self.cost < other.cost

    # Redukcja macierzy kosztów
    def reduce_matrix(self, matrix):
        cost_reduction = 0

        # Redukcja wierszy
        for i in range(self.n):
            row_min = min(matrix[i])
            if row_min != INF and row_min > 0:
                for j in range(self.n):
                    if matrix[i][j] != INF:
                        matrix[i][j] -= row_min
                cost_reduction += row_min

        # Redukcja kolumn
        for j in range(self.n):
            col_min = INF
            for i in range(self.n):
                if matrix[i][j] < col_min:
                    col_min = matrix[i][j]
            if col_min != INF and col_min > 0:
                for i in range(self.n):
                    if matrix[i][j] != INF:
                        matrix[i][j] -= col_min
                cost_reduction += col_min

        return cost_reduction

    # Obliczanie kosztu redukcji macierzy
    def calculate_cost(self, matrix):
        return self.reduce_matrix(matrix)

    # Sprawdzanie, czy warto rozwijać gałąź
    def is_promising(self, current_cost, edge_cost, reduction_cost):
        return current_cost + edge_cost + reduction_cost < self.best_cost

    # Unikanie tworzenia cykli i podcykli
    def creates_cycle(self, path, next_vertex):
        visited = set(path)
        return next_vertex in visited or (len(path) == self.n and next_vertex != 0)

    # Drukowanie macierzy dla debugowania
    def print_matrix(self, matrix):
        for row in matrix:
            print(" ".join("INF" if x == INF else str(x) for x in row))
        print()

    # Drukowanie stanu dla debugowania
    def debug_state(self, node):
        print(f"Level: {node.level}, Vertex: {node.vertex}, Cost: {node.cost}")
        print("Path:", " ".join(map(str, node.path)))
        self.print_matrix(node.reduced_matrix)

    # Główna funkcja rozwiązująca problem
    def solve(self):
        pq = []

        root_matrix = [row[:] for row in self.matrix]
        root_cost = self.calculate_cost(root_matrix)
        root = self.Node(root_matrix, [0], root_cost, 0, 0)
        heapq.heappush(pq, root)

        while pq:
            min_node = heapq.heappop(pq)

            self.debug_state(min_node)  # Debugowanie stanu

            i = min_node.vertex

            # Sprawdzanie, czy osiągnięto ostatni poziom
            if min_node.level == self.n - 1:
                min_node.path.append(0)
                final_cost = min_node.cost + (min_node.reduced_matrix[i][0] if min_node.reduced_matrix[i][0] != INF else 0)
                if final_cost < self.best_cost and final_cost >= 0:
                    self.best_cost = final_cost
                    self.best_path = min_node.path
                continue

            # Rozwijanie dzieci
            for j in range(self.n):
                if min_node.reduced_matrix[i][j] != INF and not self.creates_cycle(min_node.path, j):
                    child_path = min_node.path + [j]

                    # Unikanie kopiowania macierzy
                    child_matrix = [row[:] for row in min_node.reduced_matrix]
                    for k in range(self.n):
                        child_matrix[i][k] = INF
                        child_matrix[k][j] = INF
                    child_matrix[j][0] = INF

                    edge_cost = min_node.reduced_matrix[i][j]
                    reduction_cost = self.calculate_cost(child_matrix)
                    child_cost = min_node.cost + edge_cost + reduction_cost

                    if self.is_promising(min_node.cost, edge_cost, reduction_cost):
                        child = self.Node(child_matrix, child_path, child_cost, j, min_node.level + 1)
                        heapq.heappush(pq, child)

    # Uruchamianie algorytmu
    def run(self):
        self.solve()

    # Zwracanie najlepszego kosztu
    def get_best_cost(self):
        return self.best_cost

    # Zwracanie najlepszej ścieżki
    def get_best_path(self):
        return self.best_path


def main():
    matrix = [
        [INF, 10, 15, 20],
        [10, INF, 35, 25],
        [15, 35, INF, 30],
        [20, 25, 30, INF]
    ]

    solver = TSPSolver(matrix)

    start_time = time.time()
    solver.run()
    end_time = time.time()

    elapsed_time = end_time - start_time

    print("Minimum cost to complete the tour:", solver.get_best_cost())
    print("Path:", " -> ".join(map(str, solver.get_best_path())))
    print(f"Elapsed time: {elapsed_time:.8f} seconds")


if __name__ == "__main__":
    main()


# In[4]:


import heapq
import sys
import time

INF = sys.maxsize

class TSPSolver:
    def __init__(self, input_matrix):
        self.matrix = input_matrix
        self.best_path = []
        self.n = len(self.matrix)
        self.best_cost = INF

    class Node:
        def __init__(self, reduced_matrix, path, cost, vertex, level):
            self.reduced_matrix = reduced_matrix
            self.path = path
            self.cost = cost
            self.vertex = vertex
            self.level = level

        def __lt__(self, other):
            return self.cost < other.cost

    # Redukcja macierzy kosztów
    def reduce_matrix(self, matrix):
        cost_reduction = 0

        # Redukcja wierszy
        for i in range(self.n):
            row_min = min(matrix[i])
            if row_min != INF and row_min > 0:
                for j in range(self.n):
                    if matrix[i][j] != INF:
                        matrix[i][j] -= row_min
                cost_reduction += row_min

        # Redukcja kolumn
        for j in range(self.n):
            col_min = INF
            for i in range(self.n):
                if matrix[i][j] < col_min:
                    col_min = matrix[i][j]
            if col_min != INF and col_min > 0:
                for i in range(self.n):
                    if matrix[i][j] != INF:
                        matrix[i][j] -= col_min
                cost_reduction += col_min

        return cost_reduction

    # Obliczanie kosztu redukcji macierzy
    def calculate_cost(self, matrix):
        return self.reduce_matrix(matrix)

    # Sprawdzanie, czy warto rozwijać gałąź
    def is_promising(self, current_cost, edge_cost, reduction_cost):
        return current_cost + edge_cost + reduction_cost < self.best_cost

    # Unikanie tworzenia cykli i podcykli
    def creates_cycle(self, path, next_vertex):
        visited = set(path)
        return next_vertex in visited or (len(path) == self.n and next_vertex != 0)

    # Drukowanie macierzy dla debugowania
    def print_matrix(self, matrix):
        for row in matrix:
            print(" ".join("INF" if x == INF else str(x) for x in row))
        print()

    # Drukowanie stanu dla debugowania
    def debug_state(self, node):
        print(f"Level: {node.level}, Vertex: {node.vertex}, Cost: {node.cost}")
        print("Path:", " ".join(map(str, node.path)))
        self.print_matrix(node.reduced_matrix)

    # Główna funkcja rozwiązująca problem
    def solve(self):
        pq = []

        root_matrix = [row[:] for row in self.matrix]
        root_cost = self.calculate_cost(root_matrix)
        root = self.Node(root_matrix, [0], root_cost, 0, 0)
        heapq.heappush(pq, root)

        while pq:
            min_node = heapq.heappop(pq)

            self.debug_state(min_node)  # Debugowanie stanu

            i = min_node.vertex

            # Sprawdzanie, czy osiągnięto ostatni poziom
            if min_node.level == self.n - 1:
                min_node.path.append(0)
                final_cost = min_node.cost + (min_node.reduced_matrix[i][0] if min_node.reduced_matrix[i][0] != INF else 0)
                if final_cost < self.best_cost and final_cost >= 0:
                    self.best_cost = final_cost
                    self.best_path = min_node.path
                continue

            # Rozwijanie dzieci
            for j in range(self.n):
                if min_node.reduced_matrix[i][j] != INF and not self.creates_cycle(min_node.path, j):
                    child_path = min_node.path + [j]

                    # Unikanie kopiowania macierzy
                    child_matrix = [row[:] for row in min_node.reduced_matrix]
                    for k in range(self.n):
                        child_matrix[i][k] = INF
                        child_matrix[k][j] = INF
                    child_matrix[j][0] = INF

                    edge_cost = min_node.reduced_matrix[i][j]
                    reduction_cost = self.calculate_cost(child_matrix)
                    child_cost = min_node.cost + edge_cost + reduction_cost

                    if self.is_promising(min_node.cost, edge_cost, reduction_cost):
                        child = self.Node(child_matrix, child_path, child_cost, j, min_node.level + 1)
                        heapq.heappush(pq, child)

    # Uruchamianie algorytmu
    def run(self):
        self.solve()

    # Zwracanie najlepszego kosztu
    def get_best_cost(self):
        return self.best_cost

    # Zwracanie najlepszej ścieżki
    def get_best_path(self):
        return self.best_path


def main():
    matrix = [
        [INF, 10, 15, 20],
        [10, INF, 35, 25],
        [15, 35, INF, 30],
        [20, 25, 30, INF]
    ]

    solver = TSPSolver(matrix)

    start_time = time.time()
    solver.run()
    end_time = time.time()

    elapsed_time = end_time - start_time

    print("Minimum cost to complete the tour:", solver.get_best_cost())
    print("Path:", " -> ".join(map(str, solver.get_best_path())))
    print(f"Elapsed time: {elapsed_time:.8f} seconds")


if __name__ == "__main__":
    main()


# In[1]:


import heapq
import sys
import time
import os

INF = sys.maxsize

class TSPSolver:
    def __init__(self, input_matrix):
        self.matrix = input_matrix
        self.best_path = []
        self.n = len(self.matrix)
        self.best_cost = INF

    class Node:
        def __init__(self, reduced_matrix, path, cost, vertex, level):
            self.reduced_matrix = reduced_matrix
            self.path = path
            self.cost = cost
            self.vertex = vertex
            self.level = level

        def __lt__(self, other):
            return self.cost < other.cost

    # Redukcja macierzy kosztów
    def reduce_matrix(self, matrix):
        cost_reduction = 0

        # Redukcja wierszy
        for i in range(self.n):
            row_min = min(matrix[i])
            if row_min != INF and row_min > 0:
                for j in range(self.n):
                    if matrix[i][j] != INF:
                        matrix[i][j] -= row_min
                cost_reduction += row_min

        # Redukcja kolumn
        for j in range(self.n):
            col_min = INF
            for i in range(self.n):
                if matrix[i][j] < col_min:
                    col_min = matrix[i][j]
            if col_min != INF and col_min > 0:
                for i in range(self.n):
                    if matrix[i][j] != INF:
                        matrix[i][j] -= col_min
                cost_reduction += col_min

        return cost_reduction

    # Obliczanie kosztu redukcji macierzy
    def calculate_cost(self, matrix):
        return self.reduce_matrix(matrix)

    # Sprawdzanie, czy warto rozwijać gałąź
    def is_promising(self, current_cost, edge_cost, reduction_cost):
        return current_cost + edge_cost + reduction_cost < self.best_cost

    # Unikanie tworzenia cykli i podcykli
    def creates_cycle(self, path, next_vertex):
        visited = set(path)
        return next_vertex in visited or (len(path) == self.n and next_vertex != 0)

    # Drukowanie macierzy dla debugowania
    def print_matrix(self, matrix):
        for row in matrix:
            print(" ".join("INF" if x == INF else str(x) for x in row))
        print()

    # Drukowanie stanu dla debugowania
    def debug_state(self, node):
        print(f"Level: {node.level}, Vertex: {node.vertex}, Cost: {node.cost}")
        print("Path:", " ".join(map(str, node.path)))
        self.print_matrix(node.reduced_matrix)

    # Główna funkcja rozwiązująca problem
    def solve(self):
        pq = []

        root_matrix = [row[:] for row in self.matrix]
        root_cost = self.calculate_cost(root_matrix)
        root = self.Node(root_matrix, [0], root_cost, 0, 0)
        heapq.heappush(pq, root)

        while pq:
            min_node = heapq.heappop(pq)

            self.debug_state(min_node)  # Debugowanie stanu

            i = min_node.vertex

            # Sprawdzanie, czy osiągnięto ostatni poziom
            if min_node.level == self.n - 1:
                min_node.path.append(0)
                final_cost = min_node.cost + (min_node.reduced_matrix[i][0] if min_node.reduced_matrix[i][0] != INF else 0)
                if final_cost < self.best_cost and final_cost >= 0:
                    self.best_cost = final_cost
                    self.best_path = min_node.path
                continue

            # Rozwijanie dzieci
            for j in range(self.n):
                if min_node.reduced_matrix[i][j] != INF and not self.creates_cycle(min_node.path, j):
                    child_path = min_node.path + [j]

                    # Unikanie kopiowania macierzy
                    child_matrix = [row[:] for row in min_node.reduced_matrix]
                    for k in range(self.n):
                        child_matrix[i][k] = INF
                        child_matrix[k][j] = INF
                    child_matrix[j][0] = INF

                    edge_cost = min_node.reduced_matrix[i][j]
                    reduction_cost = self.calculate_cost(child_matrix)
                    child_cost = min_node.cost + edge_cost + reduction_cost

                    if self.is_promising(min_node.cost, edge_cost, reduction_cost):
                        child = self.Node(child_matrix, child_path, child_cost, j, min_node.level + 1)
                        heapq.heappush(pq, child)

    # Uruchamianie algorytmu
    def run(self):
        self.solve()

    # Zwracanie najlepszego kosztu
    def get_best_cost(self):
        return self.best_cost

    # Zwracanie najlepszej ścieżki
    def get_best_path(self):
        return self.best_path


def read_matrix_from_file(filename):
    try:
        with open(filename, 'r') as file:
            n = int(file.readline().strip())
            matrix = []
            for _ in range(n):
                line = file.readline().strip()
                row = [int(x) if x != '999999' else INF for x in line.split()]
                matrix.append(row)
        return matrix
    except IOError:
        print("Nie można otworzyć pliku!")
        sys.exit(1)


def read_matrix_from_input():
    n = int(input("Podaj rozmiar macierzy: "))
    matrix = []
    print("Podaj elementy macierzy w jednym wierszu (użyj 999999 dla INF, oddzielone spacją):")
    for i in range(n):
        line = input().strip()
        row = [int(x) if x != '999999' else INF for x in line.split()]
        matrix.append(row)
    return matrix


def main():
    choice = input("Czy chcesz wczytać macierz z pliku (p) czy wprowadzić ręcznie (r)? ").strip().lower()

    if choice == 'p':
        filename = input("Podaj nazwę pliku: ").strip()
        matrix = read_matrix_from_file(filename)
    else:
        matrix = read_matrix_from_input()

    solver = TSPSolver(matrix)

    start_time = time.time()
    solver.run()
    end_time = time.time()

    elapsed_time = end_time - start_time

    print("Minimum cost to complete the tour:", solver.get_best_cost())
    print("Path:", " -> ".join(map(str, solver.get_best_path())))
    print(f"Elapsed time: {elapsed_time:.8f} seconds")


if __name__ == "__main__":
    main()


# In[ ]:





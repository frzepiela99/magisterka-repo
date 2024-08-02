#!/usr/bin/env python
# coding: utf-8

# In[1]:


import time
import numpy as np
import csv

class SwapType:
    Asym, Sym = range(2)

class SwapData:
    def __init__(self):
        self.X1 = self.X2 = self.Y1 = self.Y2 = self.Z1 = self.Z2 = self.Gain = 0
        self.Choice = SwapType.Asym

def SwapCheck(Swap, W):
    DelWeight = W[Swap.X1][Swap.X2] + W[Swap.Y1][Swap.Y2] + W[Swap.Z1][Swap.Z2]
    Max = DelWeight - (W[Swap.Y1][Swap.X1] + W[Swap.Z1][Swap.X2] + W[Swap.Z2][Swap.Y2])
    if Max > Swap.Gain:
        Swap.Gain = Max
        Swap.Choice = SwapType.Asym
    Max = DelWeight - (W[Swap.X1][Swap.Y2] + W[Swap.Z1][Swap.X2] + W[Swap.Y1][Swap.Z2])
    if Max > Swap.Gain:
        Swap.Gain = Max
        Swap.Choice = SwapType.Sym

def Reverse(Ptr, start, finish):
    if start != finish:
        last = start
        next = Ptr[last]
        while last != finish:
            ahead = Ptr[next]
            Ptr[next] = last
            last = next
            next = ahead

def Initialization():
    global N, M, Ptr, W, Route, Tweight, TotalWeight

    with open('C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\magisterka-repo\\Programy\\testy\\edges100.in') as data:
        N, M = map(int, data.readline().split())
        Ptr = [0] * (N + 1)
        Route = [0] * (N + 1)
        W = np.zeros((N + 1, N + 1), dtype=int)
        Tweight = 0
        TotalWeight = 0

        for _ in range(M):
            X, Y, Temp = map(int, data.readline().split())
            W[X][Y] = Temp
            W[Y][X] = Temp
            TotalWeight += Temp

        for i in range(1, N + 1):
            Route[i] = i
            W[i][i] = 0

        for i in range(1, N):
            Tweight += W[i][i + 1]
        Tweight += W[N][1]

def Calculations():
    global Ptr, Route, Tweight

    for I in range(1, N):
        Ptr[Route[I]] = Route[I + 1]
    Ptr[Route[N]] = Route[1]

    while True:
        BestSwap = SwapData()
        Swap = SwapData()
        Swap.X1 = 1

        for I in range(1, N + 1):
            Swap.X2 = Ptr[Swap.X1]
            Swap.Y1 = Swap.X2

            for J in range(2, N - 1):
                Swap.Y2 = Ptr[Swap.Y1]
                Swap.Z1 = Ptr[Swap.Y2]

                for K in range(J + 2, N + 1):
                    Swap.Z2 = Ptr[Swap.Z1]
                    SwapCheck(Swap, W)

                    if Swap.Gain > BestSwap.Gain:
                        BestSwap = SwapData()
                        BestSwap.X1, BestSwap.X2 = Swap.X1, Swap.X2
                        BestSwap.Y1, BestSwap.Y2 = Swap.Y1, Swap.Y2
                        BestSwap.Z1, BestSwap.Z2 = Swap.Z1, Swap.Z2
                        BestSwap.Gain, BestSwap.Choice = Swap.Gain, Swap.Choice
                    Swap.Z1 = Swap.Z2

                Swap.Y1 = Swap.Y2

            Swap.X1 = Swap.X2

        if BestSwap.Gain > 0:
            if BestSwap.Choice == SwapType.Asym:
                Reverse(Ptr, BestSwap.Z2, BestSwap.X1)
                Ptr[BestSwap.Y1] = BestSwap.X1
                Ptr[BestSwap.Z2] = BestSwap.Y2
            else:
                Ptr[BestSwap.X1] = BestSwap.Y2
                Ptr[BestSwap.Y1] = BestSwap.Z2
            Ptr[BestSwap.Z1] = BestSwap.X2
            Tweight -= BestSwap.Gain
        else:
            break

    Index = 1
    for I in range(1, N + 1):
        Route[I] = Index
        Index = Ptr[Index]

def SaveResultsToCSV(filename, N, Route, Tweight, TotalWeight):
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Node', 'NextNode'])
        for i in range(1, N):
            writer.writerow([Route[i], Route[i + 1]])
        writer.writerow([Route[N], Route[1]])  # Last edge
        writer.writerow([])
        writer.writerow(['Total weight of the route', Tweight])
        writer.writerow(['Sum of all edge weights in the graph', TotalWeight])

if __name__ == "__main__":
    start = time.time()

    Initialization()
    Calculations()
    print(f"Total weight of the route: {Tweight}")
    print("Route: ", end="")
    for i in range(1, N + 1):
        print(Route[i], end="  ")
    print()

    print(f"Sum of all edge weights in the graph: {TotalWeight}")

    end = time.time()
    duration = end - start
    print(f"Execution time: {duration:.3f} seconds")

    SaveResultsToCSV('results.csv', N, Route, Tweight, TotalWeight)


# In[ ]:





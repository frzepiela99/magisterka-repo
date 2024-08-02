#!/usr/bin/env python
# coding: utf-8

# In[4]:


import time
import numpy as np

N = 0
M = 0
Ptr = []
W = []
Route = []
Ahead = I = I1 = I2 = Index = J = J1 = J2 = Last = Limit = Max = Max1 = Next = S1 = S2 = T1 = T2 = 0
Tweight = 0
TotalWeight = 0

def initialization():
    global N, M, Ptr, Route, W, Tweight, TotalWeight
    with open("C:\\Users\\mcmys\\OneDrive\\Pulpit\\magisterka repo\\magisterka-repo\\Programy\\testy\\edges100.in", "r") as file:
        N, M = map(int, file.readline().split())
        Ptr = [0] * (N + 1)
        Route = [0] * (N + 1)
        W = np.zeros((N + 1, N + 1), dtype=int)

        for _ in range(M):
            X, Y, Temp = map(int, file.readline().split())
            W[X][Y] = Temp
            W[Y][X] = Temp
            TotalWeight += Temp

    for i in range(1, N + 1):
        Route[i] = i
        W[i][i] = 0
    for i in range(1, N):
        Tweight += W[i][i + 1]
    Tweight += W[N][1]

def calculations():
    global Ptr, Route, Tweight, Max, I1, S1, S2, T1, T2, Max1
    for I in range(1, N):
        Ptr[Route[I]] = Route[I + 1]
    Ptr[Route[N]] = Route[1]
    while True:
        Max = 0
        I1 = 1
        for I in range(1, N - 1):
            Limit = N if I != 1 else N - 1
            I2 = Ptr[I1]
            J1 = Ptr[I2]
            for J in range(I + 2, Limit + 1):
                J2 = Ptr[J1]
                Max1 = W[I1][I2] + W[J1][J2] - (W[I1][J1] + W[I2][J2])
                if Max1 > Max:
                    S1, S2, T1, T2 = I1, I2, J1, J2
                    Max = Max1
                J1 = J2
            I1 = I2
        if Max <= 0:
            break
        Ptr[S1] = T1
        Next = S2
        Last = T2
        while Next != T2:
            Ahead = Ptr[Next]
            Ptr[Next] = Last
            Last = Next
            Next = Ahead
        Tweight -= Max
    
    Index = 1
    for I in range(1, N + 1):
        Route[I] = Index
        Index = Ptr[Index]

def main():
    start = time.time()

    initialization()
    print("Displaying a portion of the weight matrix for verification:")
    for k in range(1, min(10, N + 1)):
        print('\t'.join(map(str, W[k][1:min(10, N + 1)])))
    calculations()
    print("Total weight of the route:", Tweight)
    print("Route:", ' '.join(map(str, Route[1:])))
    
    EdgeSum = 0
    for i in range(1, N):
        EdgeSum += W[Route[i]][Route[i + 1]]
    EdgeSum += W[Route[N]][Route[1]]

    print("Sum of all edge lengths in the route:", EdgeSum)
    print("Sum of all edge weights in the graph:", TotalWeight)

    end = time.time()
    duration = end - start
    print("Execution time:", duration, "seconds")

if __name__ == "__main__":
    main()


# In[ ]:





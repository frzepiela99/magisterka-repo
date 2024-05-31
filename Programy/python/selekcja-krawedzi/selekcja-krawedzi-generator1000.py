#!/usr/bin/env python
# coding: utf-8

# In[1]:


import random

N = 1000
M = 499500

def main():
    with open("edges1000.in", "w") as dane:
        dane.write(f"{N} {M}\n")
        random.seed(1)
        for i in range(1, N + 1):
            for j in range(i + 1, N + 1):
                weight = random.randint(1, 999)
                dane.write(f"{i} {j} {weight}\n")

if __name__ == "__main__":
    main()


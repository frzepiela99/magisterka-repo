#!/usr/bin/env python
# coding: utf-8

# In[1]:


import random
import time

INF = 999999  # Używamy 999999 do oznaczania nieskończoności

def generate_random_matrix(size, min_value, max_value, filename):
    try:
        with open(filename, 'w') as file:
            file.write(f"{size}\n")
            for i in range(size):
                for j in range(size):
                    if i == j:
                        file.write(f"{INF}")
                    else:
                        weight = random.randint(min_value, max_value)
                        file.write(f"{weight}")
                    if j < size - 1:
                        file.write(" ")
                file.write("\n")
    except IOError:
        print("Nie można otworzyć pliku!")
        exit(1)

def main():
    random.seed(time.time())

    size = int(input("Podaj rozmiar macierzy: "))
    min_value = int(input("Podaj minimalną wartość wagi krawędzi: "))
    max_value = int(input("Podaj maksymalną wartość wagi krawędzi: "))
    filename = input("Podaj nazwę pliku, do którego zapisać macierz: ")

    generate_random_matrix(size, min_value, max_value, filename)

    print(f"Macierz została wygenerowana i zapisana do pliku {filename}")

if __name__ == "__main__":
    main()


# In[ ]:





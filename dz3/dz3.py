import time
import sys
import random
import matplotlib.pyplot as plt
import numpy as np

def gnomsort(arr):
    i = 1
    while i < len(arr):
        if i > 0 and arr[i - 1] > arr[i]:
            arr[i-1], arr[i] = arr[i], arr[i - 1]
            i = i -1
        else:
            i = i + 1


def porazrydsort(arr):
    if not arr:
        return
    maxx = max(arr)
    i = 1
    while maxx // i > 0:
        sortporazryd(arr, i)
        i = i *10

def sortporazryd(arr, razr):
    k = [[] for _ in range(10)]
    for x in arr:
        ik = (x//razr) % 10
        k[ik].append(x)
    arr[:] = obiedink(k)

def obiedink(k):
    o = []
    for i in k:
        o.extend(i)
    return o



def fastsort(arr, d, h):
    if d < h:
        opori  = razdel(arr, d, h)
        fastsort(arr, d, opori - 1)
        fastsort(arr, opori + 1, h)

def razdel(arr, d, h):
    opor = arr[h]
    i = d - 1
    for j in range(d, h):
        if arr[j] <= opor:
            i = i + 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i + 1], arr[h] = arr[h], arr[i + 1]
    return i + 1

def p(arr):
    return sys.getsizeof(arr)

sizes = [1000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000]

t_gnom = []
t_porazr = []
t_fast = []

for i in sizes:    
    massiv1 = [random.randint(1, 1000) for _ in range(i)]
    
    arr1 = massiv1.copy()
    start = time.perf_counter()
    gnomsort(arr1)
    exec_time = time.perf_counter() - start
    t_gnom.append(exec_time)

    arr2 = massiv1.copy()
    start = time.perf_counter()
    porazrydsort(arr2)
    exec_time = time.perf_counter() - start
    t_porazr.append(exec_time)

    arr3 = massiv1.copy()
    start = time.perf_counter()
    fastsort(arr3, 0, len(arr3) - 1)
    exec_time = time.perf_counter() - start
    t_fast.append(exec_time)
    
    
    
plt.figure()
plt.plot(sizes, t_gnom, marker='o', label = 'Гномья сортировка', color='blue')
plt.plot(sizes, t_porazr, marker='o', label = 'Поразрядная сортировка', color='red')
plt.plot(sizes, t_fast, marker='o', label = 'Быстрая сортировка', color='green')
plt.xlabel("Количсетво элементов в массиве")
plt.ylabel("Время выполнения (секунды)")
plt.title("")
plt.grid(True)
plt.show()
import time
import sys
import random
import matplotlib.pyplot as plt
import numpy as np

def f(x):
    a = random.choice(x)
    n = [] 
    for i in x:
        if i != a:
            n.append(i)
    return n

def p(arr):
    return sys.getsizeof(arr)

sizes = [10, 100, 1000, 10000, 50000, 100000, 250000, 500000, 1000000, 10000000]

used_times = []
pamyt = []


for i in sizes:    
    massiv1 = list(range(i))
    
    start = time.perf_counter()
    massiv2 = f(massiv1)
    exec_time = time.perf_counter() - start
    used_times.append(exec_time)
    
    mem_size = p(massiv2)
    pamyt.append(mem_size)
    

# Время выполнения
plt.figure()
plt.plot(sizes, used_times, marker='o', color='blue')
plt.xlabel("Размер массива")
plt.ylabel("Время выполнения (секунды)")
plt.title("График 1: Время работы")
plt.grid(True)
plt.xscale('log')
plt.yscale('log')
plt.show()



# Использование памяти
plt.figure()
plt.plot(sizes, pamyt, marker='o', color='red')
plt.xlabel("Размер массива")
plt.ylabel("Использование памяти (байты)")
plt.title("График 2: Занимаемая память")
plt.grid(True)
plt.xscale('log')
plt.yscale('log')
plt.show()







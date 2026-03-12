import matplotlib.pyplot as plt
import numpy as np

def f1(x):
    return (3 - x)
def f2(x):
    return (-3 + x)
def f3(x):
    return (-3 - x)
def f4(x):
    return (3 + x)


x1 = []
y1 = []

x2 = []
y2 = []

x3 = []
y3 = []

x4 = []
y4 = []

for i in range(-4, 5):
    if -4 <= f1(i) <= 4:
        y1.append(f1(i))
        x1.append(i)
    if -4 <= f2(i) <= 4:
        y2.append(f2(i))
        x2.append(i)
    if -4 <= f3(i) <= 4:
        y3.append(f3(i))
        x3.append(i)
    if -4 <= f4(i) <= 4:
        y4.append(f4(i))
        x4.append(i)
    

plt.figure()
plt.plot(x1, y1, color='blue')
plt.plot(x2, y2, color='red')
plt.plot(x3, y3, color='green')
plt.plot(x4, y4, color='yellow')
plt.xlabel("x")
plt.ylabel("y")
plt.title("Вариант 22")
plt.grid(True)
plt.show()


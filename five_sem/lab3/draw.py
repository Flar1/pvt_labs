import numpy as np
import matplotlib.pyplot as plt

# Загрузка данных ускорения
data = np.loadtxt('prog2.dat', skiprows=1)
processes = data[:, 0]    # Количество процессов
speedup = data[:, 1]      # Ускорение

# Построение графика
plt.figure(figsize=(10, 6))
plt.plot(processes, speedup, '-o', label="Вычисленное ускорение", color='green')
plt.plot(processes, processes, '--', label="Линейное ускорение", color='blue')
plt.xlabel("Количество процессов")
plt.ylabel("Ускорение")
plt.legend()
plt.grid()
plt.title("Ускорение от cpu")
plt.savefig("speedup_chart.png", dpi=300)
plt.show()

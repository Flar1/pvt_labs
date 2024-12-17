import numpy as np
import matplotlib.pyplot as plt

# Загрузка данных ускорения для первого графика
data1 = np.loadtxt('prog1.dat', skiprows=1)
processes1 = data1[:, 0].astype(int)  # Количество процессов
speedup1 = data1[:, 1]  # Ускорение

# Загрузка данных ускорения для второго графика (например, prog2.dat)
data2 = np.loadtxt('prog2.dat', skiprows=1)
processes2 = data2[:, 0].astype(int)  # Количество процессов
speedup2 = data2[:, 1]  # Ускорение

# Построение графика
plt.figure(figsize=(10, 6))

# Первый график
plt.plot(processes1, speedup1, '-o', label="n = 3000", color='green')

# Второй график
plt.plot(processes2, speedup2, '-s', label="n = 5000", color='red')  # Пример с другим цветом и маркером

# Линейное ускорение
plt.plot(processes1, processes1, '--', label="Линейное ускорение", color='blue')

# Оформление
plt.xlabel("Количество процессов")
plt.ylabel("Ускорение")
plt.xticks(processes1)  # Устанавливаем тики по количеству процессов первого графика
plt.legend()
plt.grid()
plt.savefig("speedup_chart.png", dpi=300)
plt.show()

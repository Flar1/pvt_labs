import pandas
import matplotlib.pyplot as plt

data = pandas.read_csv("./15.txt") #../build/Release/output.txt
data1 = pandas.read_csv("./20.txt")
data2 = pandas.read_csv("./25.txt")

plt.plot(data['p'], data['t'], '-o', label='n=15000')
plt.plot(data2['p'], data2['t'], '-o', label='n=20000')
plt.plot(data1['p'], data1['t'], '-o', label='n=25000')
plt.plot(range(2,9),range(2,9),'-',c="blue",linewidth=0.5,label="Linear speedup")
plt.xlabel('Количество потоков, p')
plt.ylabel('Ускорение, S')
# plt.ylim(bottom=0, top=0.000005)
plt.legend()
plt.grid(True)


plt.savefig("./graphic/image.png", dpi=300)
from matplotlib import pyplot as plt
import numbers
import numpy as np

groundTruth = np.arange(0,410,10)
print(groundTruth)
Scaling1 = [5,13,22,30,38,47,56,67,77,88,98,107,116,127,137,147,157,167,176,189,255]
Scaling2 = [5,13,21,29,37,47,56,67,78,89,97,108,115,127,138,149,158,167,179,188,198,207
            ,218,228,239,249,259,268,278,288,298,307,319,328,338,344,356,369,490,510,510]

fig, ax = plt.subplots()
# ax.plot(groundTruth, groundTruth)
ax.plot(groundTruth[0:len(Scaling1)], Scaling1, linewidth=4)
ax.plot(groundTruth, Scaling2)
ax.legend(['Scaling = 1', 'Scaling = 2'])
ax.set_xlabel('Ground Truth (mm)')
ax.set_ylabel('Distanced as perceived by sensor (mm)')
fig.savefig('efficacy.pdf')
plt.show()

import sys
import numpy as np
from matplotlib import pyplot as plt
import matplotlib.colors as colors


a = np.loadtxt(sys.argv[1])
b = np.loadtxt(sys.argv[2])

print("MSE: ", np.mean(np.power(a-b,2)))

plt.subplot(211)
plt.title(sys.argv[1])
plt.xlabel("Timestep")
plt.ylabel("Frequency(Hz)")
plt.imshow(a[:600,:], interpolation='nearest', aspect='auto', label=sys.argv[1], origin='lower', norm=colors.PowerNorm(gamma=0.5))

plt.subplot(212)
plt.title(sys.argv[2])
plt.xlabel("Timestep")
plt.ylabel("Frequency(Hz)")
plt.imshow(b[:600,:], interpolation='nearest', aspect='auto', label=sys.argv[2], origin='lower', norm=colors.PowerNorm(gamma=0.5))
plt.tight_layout()
plt.show()


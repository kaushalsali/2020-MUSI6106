import sys
import librosa
import numpy as np
from matplotlib import pyplot as plt

x, fs1 = librosa.load(sys.argv[1], sr=None, mono=False)
print(x.shape)
y, fs2 = librosa.load(sys.argv[2], sr=None, mono=False)

error = x-y

mse = np.mean(np.power(error, 2), 1)
print("MSE: ", mse)

# for each in error:
#     plt.plot(np.arange(each.size), each)
# plt.grid()
# plt.show()

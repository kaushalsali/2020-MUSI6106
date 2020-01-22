import sys
import librosa
import numpy as np
from matplotlib import pyplot as plt
import argparse

parser = argparse.ArgumentParser(description='Generate Datasets')
parser.add_argument('input_file', type=str)
parser.add_argument('output_file', type=str)
parser.add_argument('--plot', action="store_true", default=False)
args = parser.parse_args()

x, fs1 = librosa.load(args.input_file, sr=None, mono=False)
y, fs2 = librosa.load(args.output_file, sr=None, mono=False)

error = x-y

mse = np.mean(np.power(error, 2), 1)
print("MSE per channel: ", mse)

if args.plot:
    fig = plt.figure()

    ax1 = fig.add_subplot(211)    
    ax1.plot(np.arange(x[0].size), x[0], label=args.input_file + ' (L)')
    ax1.plot(np.arange(y[0].size), y[0], label=args.output_file + ' (L)')
    ax1.grid()
    ax1.legend()

    ax2 = fig.add_subplot(212)    
    ax2.plot(np.arange(x[1].size), x[1], label=args.input_file + ' (R)')
    ax2.plot(np.arange(y[1].size), y[1], label=args.output_file + ' (R)')
    ax2.grid()
    ax2.legend()
    
    plt.show()

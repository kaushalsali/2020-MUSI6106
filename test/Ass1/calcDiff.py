import sys
import librosa
import numpy as np
from matplotlib import pyplot as plt
import argparse

parser = argparse.ArgumentParser(description='Generate Datasets')
parser.add_argument('input_file', type=str)
parser.add_argument('output_file', type=str)
parser.add_argument('--plot-audio', action="store_true", default=False)
parser.add_argument('--plot-error', action="store_true", default=False)
args = parser.parse_args()

x, fs1 = librosa.load(args.input_file, sr=None, mono=False)
y, fs2 = librosa.load(args.output_file, sr=None, mono=False)

channels = len(x.shape)
if channels == 1:
    x = x.reshape(1, -1)
    y = y.reshape(1, -1)


error = x-y
mse = np.mean(np.power(error, 2), len(error.shape)-1)
print("MSE per channel: ", mse)



if args.plot_audio:
    fig = plt.figure()

    ax1 = fig.add_subplot(channels, 1, 1)    
    ax1.plot(np.arange(x[0].size), x[0], label=args.input_file + ' (L / Mono)')
    ax1.plot(np.arange(y[0].size), y[0], label=args.output_file + ' (L / Mono)')
    ax1.grid()
    ax1.legend()

    if channels == 2:
        ax2 = fig.add_subplot(channels, 1, 2)    
        ax2.plot(np.arange(x[1].size), x[1], label=args.input_file + ' (R)')
        ax2.plot(np.arange(y[1].size), y[1], label=args.output_file + ' (R)')
        ax2.grid()
        ax2.legend()
    
    plt.show()

    
if args.plot_error:
    fig = plt.figure()
    plt.title('Absolute Error')
    
    ax1 = fig.add_subplot(channels, 1, 1)    
    ax1.plot(np.arange(error[0].size), error[0], label='Channel (L / Mono)')
    ax1.grid()
    ax1.legend()

    if channels == 2:
        ax2 = fig.add_subplot(channels, 1, 2)    
        ax2.plot(np.arange(error[1].size), error[1], label='Channel (R)')
        ax2.grid()
        ax2.legend()
    
    plt.show()
    

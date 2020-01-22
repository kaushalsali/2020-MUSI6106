import librosa
import numpy as np
from tqdm import tqdm
import argparse

parser = argparse.ArgumentParser(description='Generate Datasets')
parser.add_argument('input_file', type=str)
parser.add_argument('filter_type', type=str)
parser.add_argument('--delay-sec', type=float, default=0.01)
parser.add_argument('--g', type=float, default=0.5)
args = parser.parse_args()


'''
Input shape = (num_samples, num_channels)
'''
def combFIR(x, delay_length=10, g=0.5):
    print("Filtering with FIR comb")

    num_frames = x.shape[0]
    num_channels = x.shape[1]

    y = np.zeros(x.shape)
 
    delay_line = np.zeros((delay_length, num_channels));

    for n in tqdm(range(num_frames)):
        y[n] = x[n] + g * delay_line[-1]
        delay_line = np.roll(delay_line, 1, 0)
        delay_line[0] = x[n]

    return y

    
def combIIR(x, delay_length=10, g=0.5):
    print("Filtering with IIR comb")

    num_frames = x.shape[0]
    num_channels = x.shape[1]

    y = np.zeros(x.shape)
 
    delay_line = np.zeros((delay_length, num_channels));

    for n in tqdm(range(num_frames)):
        y[n] = x[n] + g * delay_line[-1]
        delay_line = np.roll(delay_line, 1, 0)
        delay_line[0] = y[n]

    return y


# ----------------------------------------------

x, fs = librosa.load(args.input_file, sr=None, mono=False)
x = x.T

delay = int(args.delay_sec * fs)
print("delay samples: ", delay)

if args.filter_type == 'fir':
    output_file = args.input_file[:-4] + '_FIR.wav'
    y = combFIR(x, delay, args.g)

elif args.filter_type == 'iir':
    output_file = args.input_file[:-4] + '_IIR.wav'
    y = combIIR(x, delay, args.g)

librosa.output.write_wav(output_file, y, fs)
print("File saved as " + output_file)

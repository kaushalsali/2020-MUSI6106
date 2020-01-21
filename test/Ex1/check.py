import librosa
import numpy as np
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('wav_file', type=str)
parser.add_argument('txt_file', type=str)
args = parser.parse_args()

a = np.zeros((2,132300))
with open(args.txt_file, 'r') as f:
    for i, each in enumerate(f):
        a[0][i], a[1][i] = each.split('\t')
        
b, fs = librosa.load(args.wav_file, sr=44100, mono=False)

print('Number of samples whose values dont match: ', np.sum(a-b != 0))

# Exercise 3: Spectrogram

### How to run
`/bin/debug/MUSI6106Exec [input_file_path] [block_size] [hop_size]`
- `input_file_path` : path of a `.wav` file
- `block_size`      : int
- `hop_size`        : int

This will output a text file `[input_file_path]_stft.txt` Note: The output shape is `(num_hops, block_size/2+1)`


### Test for MajorTom16.wav:
##### MSE: 
1.042760290775741e-05
##### Spectrogram comparison plot:
https://github.com/kaushalsali/2020-MUSI6106/blob/ex3/test/ex3/Spectrograms.png
##### Run test using: 
`python compareSpectrograms.py MajorTom16_stft_transposed.txt MajorTomSpectrogram.txt`

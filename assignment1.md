# Assignment 1

### How to run
`/bin/debug/MUSI6106Exec [input_file_path] [filter_type] [delay-in-sec]`
- `input_file_path` : path of a `.wav` file
- `filter_type`     : `fir` or `iir`
- `delay-in-sec`    : float (optional)

### Real World Tests
##### Absolute Error plots for real world audio can be found at:
https://github.com/kaushalsali/2020-MUSI6106/blob/ass1/test/Ass1/plots

##### MSE for cello-double.wav:
- FIR: 1.1640032e-10  
- IIR: 3.0795544e-10  

##### MSE for speech-male.wav:
- FIR: 1.1640032e-10   
- IIR: 2.5988967e-09  

### Notes:
- Python test scripts for comb filter and error calculation are in `/test/Ass1`.
- Audio used for testing is in `/test/Ass1/audio`
- To run the cpp tests, place the `/test/Ass1/test_audio` folder in the same directory as the executable (which is `/bin/debug/`) and then run the executable with no arguments.

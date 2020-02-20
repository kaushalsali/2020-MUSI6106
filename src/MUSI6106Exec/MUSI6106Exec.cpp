
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Fft.h"
#include "Vector.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void convertToMono(float* pMonoAudioBuffer, float** ppInputAudioBuffer, int iNumChannels, int iLength);


/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioData = nullptr;
    float                   **ppfAudioWriteIdx = nullptr;

    CFft::complex_t         *pfSpectralData = nullptr;
    float                   *pfMag = nullptr;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    CFft                    *pFft = nullptr;

    int                     fftBlockSize;
    int                     fftHopSize;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    sInputFilePath = argv[1];
    sOutputFilePath = sInputFilePath.substr(0, sInputFilePath.size() - 4).append("_stft.wav");
    fftBlockSize = atoi(argv[2]);
    fftHopSize = atoi(argv[3]);
    
    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::FileIoType_t::kFileRead, &stFileSpec);
    phAudioFile->getFileSpec(stFileSpec);

    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    
    hOutputFile.open(sOutputFilePath, std::fstream::out);
    hOutputFile.precision(16);
 
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float* [stFileSpec.iNumChannels];
    ppfAudioWriteIdx = new float* [stFileSpec.iNumChannels];

    for (int i=0; i<stFileSpec.iNumChannels; i++) {
        ppfAudioData[i] = new float[fftBlockSize] (); // Init to zeros
        ppfAudioWriteIdx[i] = ppfAudioData[i] + fftBlockSize - fftHopSize;  // Pointer to write data read from input file to end of buffer
    }
    pfSpectralData = new float[fftBlockSize] (); // Init to zeros
    pfMag = new float [int(fftBlockSize / 2 + 1)];
    
    //////////////////////////////////////////////////////////////////////////////
    // create FFT class instance and ring buffer instances
    CFft::createInstance(pFft);
    pFft->initInstance(fftBlockSize);

    //////////////////////////////////////////////////////////////////////////////
    // Process and calculate FFT
    long long hopSize = fftHopSize;
    long long fftMagSize = int(fftBlockSize/2 + 1);

    while(!phAudioFile->isEof()) {
        // Read new block (of hop size) from input file into end of AudioData buffer
        phAudioFile->readData(ppfAudioWriteIdx, hopSize);

        // Convert to mono and save in same buffer at [0]
        convertToMono(ppfAudioWriteIdx[0], ppfAudioWriteIdx, stFileSpec.iNumChannels, fftHopSize);

        // Get Magnitude spectrum
        pFft->doFft(pfSpectralData, ppfAudioData[0]);
        pFft->getMagnitude(pfMag, pfSpectralData);

        // Move window overlap portion to start of buffer.
        CVectorFloat::moveInMem(ppfAudioData[0], fftHopSize, 0, fftBlockSize - fftHopSize);

        // Write magnitude spectrum of current time-step to output file.
        for (long long i = 0; i < fftMagSize; i++) {
            hOutputFile << pfMag[i] << "\t";
        }
        hOutputFile << std::endl;

    }


    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    CFft::destroyInstance(pFft);
    for (int i=0; i<stFileSpec.iNumChannels; i++)
        free(ppfAudioData[i]);
    free(ppfAudioData);
    phAudioFile->closeFile();
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();

    // all done
    return 0;

}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

void convertToMono(float* pMonoAudioBuffer, float** ppInputAudioBuffer, int iNumChannels, int iLength) {
    CVectorFloat::copy(pMonoAudioBuffer, ppInputAudioBuffer[0], iLength);
    for (int i=1; i<iNumChannels; i++) {
        CVectorFloat::add_I(pMonoAudioBuffer, ppInputAudioBuffer[i], iLength);
    }
    CVectorFloat::mulC_I(pMonoAudioBuffer, 1/(float)iNumChannels, iLength);
}
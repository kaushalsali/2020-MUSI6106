
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Vibrato.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    bool argsMissing = false;

    std::string sInputFilePath;                 //!< file paths
    std::string sOutputFilePath;
    float **ppfInputAudioData = 0;
    float **ppfOutputAudioData = 0;
    CAudioFileIf *phInputAudioFile = 0;
    CAudioFileIf *phOutputAudioFile = 0;
    CAudioFileIf::FileSpec_t stFileSpec;

    static const int kBlockSize = 1024;

    float delayTimeInSec;// = (float)1/441;
    float widthInSec;// = (float)1/441;
    float LFOFreqInHz;// = 1;

    Vibrato *pVibrato;

    clock_t time = 0;
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc == 1) {

    }
    if (argc > 1) {
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath.substr(0, sInputFilePath.size() - 4).append("_cpp.wav");
    }
    else {
        std::cout << "Missing: arg1: Input audio path" << std::endl;
        argsMissing = true;
    }
    if (argc > 2) {
        delayTimeInSec = atof(argv[2]);
    }
    else {
        std::cout << "Missing: arg2: Delay Time" << std::endl;
        argsMissing = true;
    }
    if (argc > 3) {
        widthInSec = atof(argv[3]);
        if (delayTimeInSec < 0) {
            std::cout << "arg3: Width time cannot be negative" << std::endl;
            return -1;
        }
    }
    else {
        std::cout << "Missing: arg3: Width Time" << std::endl;
        argsMissing = true;
    }
    if (argc > 4) {
        LFOFreqInHz = atof(argv[4]);
        if (delayTimeInSec < 0) {
            std::cout << "arg4: LFO Frequency cannot be negative" << std::endl;
            return -1;
        }
    }
    else {
        std::cout << "Missing: arg4: LFO Frequency" << std::endl;
        argsMissing = true;
    }

    if (argsMissing) // Stop if any argument is missing
        return -1;

    std::cout << "Delay: " << delayTimeInSec << std::endl;
    std::cout << "Width: " << widthInSec << std::endl;
    std::cout << "LFO Freq: " << LFOFreqInHz << std::endl;




    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phInputAudioFile);
    phInputAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phInputAudioFile->isOpen()) {
        cout << "Input wav file open error!";
        return -1;
    }
    phInputAudioFile->getFileSpec(stFileSpec);

    //////////////////////////////////////////////////////////////////////////////
    // open the output text file

    CAudioFileIf::create(phOutputAudioFile);
    phOutputAudioFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    if (!phOutputAudioFile->isOpen()) {
        cout << "Output wav file open error!";
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfInputAudioData = new float *[stFileSpec.iNumChannels];
    ppfOutputAudioData = new float *[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        ppfInputAudioData[i] = new float[kBlockSize];
        ppfOutputAudioData[i] = new float[kBlockSize];
    }

    //////////////////////////////////////////////////////////////////////////////
    // create Vibrato
    pVibrato = new Vibrato(delayTimeInSec, widthInSec, LFOFreqInHz, (int)stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);

    // //////////////////////////////////////////////////////////////////////////
    // Read, apply vibrato and write audio

    time = clock();
    long long blockSize = kBlockSize;
    while (!phInputAudioFile->isEof()) {
        phInputAudioFile->readData(ppfInputAudioData, blockSize);
        pVibrato->process(ppfInputAudioData, ppfOutputAudioData, (int)blockSize);
        phOutputAudioFile->writeData(ppfOutputAudioData, blockSize);
    }
    cout << "Reading and writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up

    delete pVibrato;
    phInputAudioFile->closeFile();
    phOutputAudioFile->closeFile();
    CAudioFileIf::destroy(phInputAudioFile);
    CAudioFileIf::destroy(phOutputAudioFile);
    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        delete[] ppfInputAudioData[i];
        delete[] ppfOutputAudioData[i];
    }
    delete[] ppfInputAudioData;
    delete[] ppfOutputAudioData;
    ppfInputAudioData = nullptr;
    ppfOutputAudioData = nullptr;

    return 0;
}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}


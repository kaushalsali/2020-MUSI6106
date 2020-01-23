#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"
#include "CombTests.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[]) {
    std::string sInputFilePath;                 //!< file paths
    std::string sOutputFilePath;
    float delayTimeInSec;

    static const int kBlockSize = 1024;

    clock_t time = 0;

    float **ppfInputAudioData = 0;
    float **ppfOutputAudioData = 0;

    CAudioFileIf *phInputAudioFile = 0;
    CAudioFileIf *phOutputAudioFile = 0;

    CAudioFileIf::FileSpec_t stFileSpec;

    CCombFilterIf *pCombFilter = 0;
    CCombFilterIf::CombFilterType_t filterType;
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments

    if (argc == 1) {
        std::cout << "------------------------------" << std::endl;
        std::cout << "\tTEST MODE" << std::endl;
        std::cout << "------------------------------" << std::endl << std::endl;
        test1();
        test2();
        test3();
        test4();
        test5();
        return 0;
    }
    if (argc > 1) {
        sInputFilePath = argv[1];
    }
    else {
        std::cout << "Missing: arg1: Input audio path, arg2: Filter type ('fir' or 'iir')" << std::endl;
        return -1;
    }
    if (argc > 2) {
        std::string arg = argv[2];
        if (arg == "fir")
            filterType = CCombFilterIf::kCombFIR;
        else if (arg == "iir")
            filterType = CCombFilterIf::kCombIIR;
        else {
            std::cout << "Missing: arg2: Filter type ('fir' or 'iir')" << std::endl;
            return -1;
        }
        sOutputFilePath = sInputFilePath.substr(0, sInputFilePath.size() - 4).append("_" + arg + "_cpp.wav");
    }
    if (argc > 3)
        delayTimeInSec = atof(argv[3]);
    else
        delayTimeInSec = 0.01;

    std::cout << "Filter Type: " << argv[2] << std::endl;
    std::cout << "Delay: " << delayTimeInSec << std::endl;

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
    // create Comb Filter

    CCombFilterIf::create(pCombFilter);
    pCombFilter->init(filterType, delayTimeInSec, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);

    //////////////////////////////////////////////////////////////////////////////
    // Read, filter and write audio

    time = clock();
    long long blockSize = kBlockSize;
    while (!phInputAudioFile->isEof()) {
        phInputAudioFile->readData(ppfInputAudioData, blockSize);
        pCombFilter->process(ppfInputAudioData, ppfOutputAudioData, (int)blockSize);
        phOutputAudioFile->writeData(ppfOutputAudioData, blockSize);
    }
    cout << "Reading and writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up

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
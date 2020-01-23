//
// Created by Kaushal Sali on 22/01/20.
//

#include <iostream>
#include <ctime>
#include <sstream>

#include "MUSI6106Config.h"

#include "CombTests.h"
#include "AudioFileIf.h"
#include "CombFilterIf.h"


// --------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------TESTS----------------------------------------------------------

int test1() {

    std::cout << "---------------" << std::endl;
    std::cout << "Running Test 1:" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::string sInputFilePath = "test_audio/sine100.wav"; // 441 samples = 1 cycle (fs = 44100)
    std::string sOutputFilePath = "test_audio/test1_sine100_out.wav";
    float delayTimeInSec = 0.01; // Delay line length = 441 (fs = 44100)
    float gain = -1;

    static const int kBlockSize = 1024;

    clock_t time = 0;

    float **ppfInputAudioData = 0;
    float **ppfOutputAudioData = 0;

    CAudioFileIf *phInputAudioFile = 0;
    CAudioFileIf *phOutputAudioFile = 0;

    CAudioFileIf::FileSpec_t stFileSpec;

    CCombFilterIf *pCombFilter = 0;
    CCombFilterIf::CombFilterType_t filterType = CCombFilterIf::kCombFIR;

    // Open the input and output text file
    CAudioFileIf::create(phInputAudioFile);
    phInputAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phInputAudioFile->isOpen()) {
        std::cout << "Input wav file open error!";
        return -1;
    }
    phInputAudioFile->getFileSpec(stFileSpec);

    CAudioFileIf::create(phOutputAudioFile);
    phOutputAudioFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    if (!phOutputAudioFile->isOpen()) {
        std::cout << "Output wav file open error!";
        return -1;
    }

    // Allocate memory for audio i/o buffers
    ppfInputAudioData = new float *[stFileSpec.iNumChannels];
    ppfOutputAudioData = new float *[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        ppfInputAudioData[i] = new float[kBlockSize];
        ppfOutputAudioData[i] = new float[kBlockSize];
    }

    // Create Comb Filter
    CCombFilterIf::create(pCombFilter);
    pCombFilter->init(filterType, delayTimeInSec, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
    pCombFilter->setParam(CCombFilterIf::kParamGain, gain);

    // Read, filter and write audio
    time = clock();
    long long blockSize = kBlockSize;
    while (!phInputAudioFile->isEof()) {
        phInputAudioFile->readData(ppfInputAudioData, blockSize);
        pCombFilter->process(ppfInputAudioData, ppfOutputAudioData, (int)blockSize);
        phOutputAudioFile->writeData(ppfOutputAudioData, blockSize);
    }
    std::cout << "Reading and writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << std::endl;
    std::cout << "Output: " << sOutputFilePath << std::endl;

    // Clean-up
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

    std::cout << std::endl << "Test 1 completed" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;
    return 0;
}

// --------------------------------------------------------------------------------------------------------------------

int test2() {

    std::cout << "---------------" << std::endl;
    std::cout << "Running Test 2:" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::string sInputFilePath = "test_audio/sine100.wav"; // 441 samples = 1 cycle (fs = 44100)
    std::string sOutputFilePath; // = "test_audio/sine100_test2_out.wav";
    float delayTimeInSec = 0.01; // Delay line length = 441 (fs = 44100)
    float gains[] = {-1, -0.7, -0.2, 0.4, 0.9};

    static const int kBlockSize = 1024;

    clock_t time = 0;

    float **ppfInputAudioData = 0;
    float **ppfOutputAudioData = 0;

    CAudioFileIf *phInputAudioFile = 0;
    CAudioFileIf *phOutputAudioFile = 0;

    CAudioFileIf::FileSpec_t stFileSpec;

    CCombFilterIf *pCombFilter = 0;
    CCombFilterIf::CombFilterType_t filterType = CCombFilterIf::kCombFIR;


    // Open the input and create output text file
    CAudioFileIf::create(phInputAudioFile);
    phInputAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phInputAudioFile->isOpen()) {
        std::cout << "Input wav file open error!";
        return -1;
    }
    phInputAudioFile->getFileSpec(stFileSpec);
    CAudioFileIf::create(phOutputAudioFile);

    // Allocate memory for audio i/o buffers
    ppfInputAudioData = new float *[stFileSpec.iNumChannels];
    ppfOutputAudioData = new float *[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        ppfInputAudioData[i] = new float[kBlockSize];
        ppfOutputAudioData[i] = new float[kBlockSize];
    }

    for (float gain : gains) {
        std::cout << "For gain = " << gain << std::endl;

        // open output file
        std::ostringstream gainAsString;
        gainAsString << gain;
        std::string s(gainAsString.str());
        sOutputFilePath = "test_audio/test2_sine100_out_g" + s + ".wav";
        phOutputAudioFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
        if (!phOutputAudioFile->isOpen()) {
            std::cout << "Output wav file open error!";
            return -1;
        }
        phInputAudioFile->setPosition((long long) 0);

        // Create Comb Filter
        CCombFilterIf::create(pCombFilter);
        pCombFilter->init(filterType, delayTimeInSec, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
        pCombFilter->setParam(CCombFilterIf::kParamGain, gain);

        // Read, filter and write audio
        time = clock();
        long long blockSize = kBlockSize;
        while (!phInputAudioFile->isEof()) {
            phInputAudioFile->readData(ppfInputAudioData, blockSize);
            pCombFilter->process(ppfInputAudioData, ppfOutputAudioData, (int)blockSize);
            phOutputAudioFile->writeData(ppfOutputAudioData, blockSize);
        }
        std::cout << "Reading and writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << std::endl;
        std::cout << "Output: " << sOutputFilePath << std::endl;

        // Clean up
        CCombFilterIf::destroy(pCombFilter);
        phOutputAudioFile->reset();
    }

    phInputAudioFile->reset();
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

    std::cout << std::endl << "Test 2 completed" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;
    return 0;
}

// --------------------------------------------------------------------------------------------------------------------

int test3() {

    std::cout << "---------------" << std::endl;
    std::cout << "Running Test 3:" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::string sInputFilePath = "test_audio/sine100.wav"; // 441 samples = 1 cycle (fs = 44100)
    std::string sOutputFilePath;
    float delayTimeInSec = 0.01; // Delay line length = 441 (fs = 44100)
    float gain = 0.5;

    // static const int kBlockSize = 1024;
    long long int blockSizes[] = {1024, 512, 256};

    clock_t time = 0;

    float **ppfInputAudioData = 0;
    float **ppfOutputAudioData = 0;

    CAudioFileIf *phInputAudioFile = 0;
    CAudioFileIf *phOutputAudioFile = 0;

    CAudioFileIf::FileSpec_t stFileSpec;

    CCombFilterIf *pCombFilter = 0;
    CCombFilterIf::CombFilterType_t filterType = CCombFilterIf::kCombFIR;


    // Open the input and create output text file
    CAudioFileIf::create(phInputAudioFile);
    phInputAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phInputAudioFile->isOpen()) {
        std::cout << "Input wav file open error!";
        return -1;
    }
    phInputAudioFile->getFileSpec(stFileSpec);
    CAudioFileIf::create(phOutputAudioFile);


    for (long long int blockSize : blockSizes) {

        std::cout << "For blockSize = " << blockSize << std::endl;

        // open output file
        std::ostringstream gainAsString;
        gainAsString << blockSize;
        std::string s(gainAsString.str());
        sOutputFilePath = "test_audio/test3_sine100_out_b" + s + ".wav";
        phOutputAudioFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
        if (!phOutputAudioFile->isOpen()) {
            std::cout << "Output wav file open error!";
            return -1;
        }
        phInputAudioFile->setPosition((long long) 0);

        // Allocate memory for audio i/o buffers
        ppfInputAudioData = new float *[stFileSpec.iNumChannels];
        ppfOutputAudioData = new float *[stFileSpec.iNumChannels];
        for (int i = 0; i < stFileSpec.iNumChannels; i++) {
            ppfInputAudioData[i] = new float[blockSize];
            ppfOutputAudioData[i] = new float[blockSize];
        }

        // Create Comb Filter
        CCombFilterIf::create(pCombFilter);
        pCombFilter->init(filterType, delayTimeInSec, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
        pCombFilter->setParam(CCombFilterIf::kParamGain, gain);

        // Read, filter and write audio
        time = clock();

        while (!phInputAudioFile->isEof()) {

            phInputAudioFile->readData(ppfInputAudioData, blockSize);
            pCombFilter->process(ppfInputAudioData, ppfOutputAudioData, (int)blockSize);
            phOutputAudioFile->writeData(ppfOutputAudioData, blockSize);
        }
        std::cout << "Reading and writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << std::endl;
        std::cout << "Output: " << sOutputFilePath << std::endl;

        // Clean up
        CCombFilterIf::destroy(pCombFilter);

        for (int i = 0; i < stFileSpec.iNumChannels; i++) {
            delete[] ppfInputAudioData[i];
            delete[] ppfOutputAudioData[i];
        }
        delete[] ppfInputAudioData;
        delete[] ppfOutputAudioData;
        ppfInputAudioData = nullptr;
        ppfOutputAudioData = nullptr;

        phOutputAudioFile->reset();
    }

    phInputAudioFile->reset();
    CAudioFileIf::destroy(phInputAudioFile);
    CAudioFileIf::destroy(phOutputAudioFile);

    std::cout << std::endl << "Test 3 completed" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;
    return 0;
}

// --------------------------------------------------------------------------------------------------------------------

int test4() {

    std::cout << "---------------" << std::endl;
    std::cout << "Running Test 4:" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::string sInputFilePath = "test_audio/empty.wav"; // 441 samples = 1 cycle (fs = 44100)
    std::string sOutputFilePath = "test_audio/test4_empty_out.wav";
    float delayTimeInSec = 0.01; // Delay line length = 441 (fs = 44100)
    float gain = 0.5;

    static const int kBlockSize = 1024;

    clock_t time = 0;

    float **ppfInputAudioData = 0;
    float **ppfOutputAudioData = 0;

    CAudioFileIf *phInputAudioFile = 0;
    CAudioFileIf *phOutputAudioFile = 0;

    CAudioFileIf::FileSpec_t stFileSpec;

    CCombFilterIf *pCombFilter = 0;
    CCombFilterIf::CombFilterType_t filterType = CCombFilterIf::kCombFIR;

    // Open the input and output text file
    CAudioFileIf::create(phInputAudioFile);
    phInputAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phInputAudioFile->isOpen()) {
        std::cout << "Input wav file open error!";
        return -1;
    }
    phInputAudioFile->getFileSpec(stFileSpec);

    CAudioFileIf::create(phOutputAudioFile);
    phOutputAudioFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    if (!phOutputAudioFile->isOpen()) {
        std::cout << "Output wav file open error!";
        return -1;
    }

    // Allocate memory for audio i/o buffers
    ppfInputAudioData = new float *[stFileSpec.iNumChannels];
    ppfOutputAudioData = new float *[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        ppfInputAudioData[i] = new float[kBlockSize];
        ppfOutputAudioData[i] = new float[kBlockSize];
    }

    // Create Comb Filter
    CCombFilterIf::create(pCombFilter);
    pCombFilter->init(filterType, delayTimeInSec, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
    pCombFilter->setParam(CCombFilterIf::kParamGain, gain);

    // Read, filter and write audio
    time = clock();
    long long blockSize = kBlockSize;
    while (!phInputAudioFile->isEof()) {
        phInputAudioFile->readData(ppfInputAudioData, blockSize);
        pCombFilter->process(ppfInputAudioData, ppfOutputAudioData, (int)blockSize);
        phOutputAudioFile->writeData(ppfOutputAudioData, blockSize);
    }
    std::cout << "Reading and writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << std::endl;
    std::cout << "Output: " << sOutputFilePath << std::endl;

    // Clean-up
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

    std::cout << std::endl << "Test 4 completed" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;
    return 0;
}

// --------------------------------------------------------------------------------------------------------------------

int test5() {

    return 0;
}
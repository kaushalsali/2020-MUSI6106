
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();



/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    
    sInputFilePath = argv[1];
    sOutputFilePath = argv[2];
    
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

    ppfAudioData = (float **) malloc(stFileSpec.iNumChannels * sizeof(float *));
    for (int i=0; i<stFileSpec.iNumChannels; i++) {
        ppfAudioData[i] = (float *) malloc(kBlockSize * sizeof(float));
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)
        
    long long blockSize = kBlockSize;
    while(!phAudioFile->isEof()) {
        phAudioFile->readData(ppfAudioData, blockSize);
        for (long long i=0; i<blockSize; i++) {
            for (int j=0; j<stFileSpec.iNumChannels; j++)
                hOutputFile << ppfAudioData[j][i] << "\t";
            hOutputFile << std::endl;
        }
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)

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


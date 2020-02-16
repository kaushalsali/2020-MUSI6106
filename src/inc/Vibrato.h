//
// Created by Kaushal Sali on 15/02/20.
//

#ifndef MUSI6106_VIBRATOIF_H
#define MUSI6106_VIBRATOIF_H

#endif //MUSI6106_VIBRATOIF_H

#include "ErrorDef.h"
#include "RingBuffer.h"
#include "LFO.h"

class VibratoIf
{
public:
    VibratoIf(int delayInSec, int modulationWidthInSec, int sampleRateInHz);
    ~VibratoIf();
    float getParam();
    Error_t setParam();
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);


private:
    VibratoIf();
    CRingBuffer<float>* ringBuffer;
    //LFO* lfo;
    int delayInSec;
    int widthInSec;
    int sampleRate;
};


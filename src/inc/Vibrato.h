//
// Created by Kaushal Sali on 15/02/20.
//

#ifndef MUSI6106_VIBRATOIF_H
#define MUSI6106_VIBRATOIF_H

#endif //MUSI6106_VIBRATOIF_H

#include "ErrorDef.h"
#include "RingBuffer.h"
#include "WaveTableOscillator.h"


class Vibrato
{
public:
    Vibrato(int delayInSec, int modulationWidthInSec, int sampleRateInHz,  int numChannels);
    ~Vibrato();
    float getParam();
    Error_t setParam();
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int numFrames);


private:
    Vibrato();
    CRingBuffer<float>** m_ppBuffer;
    WaveTableOscillator* m_pLFO;
    int m_delayInSec;
    int m_widthInSec;
    int m_sampleRate;
    int m_numChannels;
};


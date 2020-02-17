//
// Created by Kaushal Sali on 15/02/20.
//

#ifndef MUSI6106_VIBRATOIF_H
#define MUSI6106_VIBRATOIF_H

#endif //MUSI6106_VIBRATOIF_H

#include "ErrorDef.h"
#include "RingBuffer.h"
#include "LFO.h"


class Vibrato
{
public:

    enum VibratoParams {
        delayInSec,
        widthInSec,
        LFOFreqInHz
    };

    Vibrato(float delayInSec, float modulationWidthInSec, float LFOFreqInHz, int sampleRateInHz, int numChannels);
    ~Vibrato();
    float getParam();
    Error_t setParam();
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int numFrames);


private:
    Vibrato();
    CRingBuffer<float>** m_ppBuffer;
    LFO* m_pLFO; //TODO: Give option to user to pass object
    float m_LFOFreqInHz;
    float m_delayInSec;
    int m_delayInSamples;
    float m_widthInSec;
    int m_widthInSamples;
    int m_sampleRate;
    int m_numChannels;
};


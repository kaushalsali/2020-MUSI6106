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

    Vibrato(float maxDelayInSec, float modulationWidthInSec, float LFOFreqInHz, float sampleRateInHz, int numChannels);
    ~Vibrato();
    float getParam(VibratoParams param);
    Error_t setParam(VibratoParams param, float value);
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int numFrames);


private:
    Vibrato();
    Error_t setLFOFreq(float freqInHz);

    /*
    * Updates delay length and calls updateBufferLength()
    */
    Error_t setDelay(float delayInSec);

    /*
    * Updates width and calls updateBufferLength()
    */
    Error_t setWidth(float widthInSec);

    /*
    * Updates the buffer length acc to changes in width and delay.
    * Resets and initializes buffer with zeros. Read ptr is set to delay length.
    */
    Error_t updateBufferLength();

    CRingBuffer<float>** m_ppBuffer;
    LFO* m_pLFO;
    float m_LFOFreqInHz;
    float m_maxDelayInSec;
    float m_delayInSec;
    int m_delayInSamples;
    float m_widthInSec;
    int m_widthInSamples;
    float m_sampleRate;
    int m_numChannels;
};


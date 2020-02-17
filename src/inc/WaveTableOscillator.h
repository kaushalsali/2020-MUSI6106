//
// Created by Kaushal Sali on 16/02/20.
//

#ifndef MUSI6106_WAVETABLEOSCILLATOR_H
#define MUSI6106_WAVETABLEOSCILLATOR_H

#include "ErrorDef.h"
#include "RingBuffer.h"
#include "Synthesis.h"


template <class T>
class WaveTableOscillator {
public:
    enum Waveform
    {
        Sine,
        Rectangle,
        Saw,
    };

    explicit WaveTableOscillator (int waveTableSize);
    Error_t setWavefromType(Waveform waveformType);
    Error_t setFrequency(float frequencyInHz);
    Error_t setSampleRate(int sampleRate);
    Error_t updateWaveTable();
    Error_t init(Waveform waveformType, float frequencyInHz, int sampleRateInHz);
    T getNextSample();

private:
    int m_waveTableSize;
    CRingBuffer<T>* m_pWaveTable;
    Waveform m_waveformType;
    int m_frequency;
    int m_sampleRate;
    float m_currentSample = 0.0f;
    float m_sampleDelta = 0.0f;
};



#endif //MUSI6106_WAVETABLEOSCILLATOR_H

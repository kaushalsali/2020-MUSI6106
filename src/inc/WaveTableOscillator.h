//
// Created by Kaushal Sali on 16/02/20.
//

#ifndef MUSI6106_WAVETABLEOSCILLATOR_H
#define MUSI6106_WAVETABLEOSCILLATOR_H

#include "ErrorDef.h"
#include "RingBuffer.h"
#include "Synthesis.h"



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
    WaveTableOscillator::Waveform getWavefromType() const;

    /*
     * Sets the frequency at which the WaveTableOscillator will generate a waveform.
     * Internally updates sampleDelta (the factor by which the wave table updates it's current index in the waveform).
     */
    Error_t setFrequency(float frequencyInHz);
    float getFrequency() const;

    Error_t setSampleRate(int sampleRate);
    int getSampleRate() const;

    float getCurrentSampleIndex() const;
    float getSampleDelta() const;
    Error_t updateWaveTable();
    Error_t init(Waveform waveformType, float frequencyInHz, int sampleRateInHz);
    float getNextSample();

private:
    int m_waveTableSize;
    CRingBuffer<float>* m_pWaveTable;
    Waveform m_waveformType;
    float m_frequency;
    int m_sampleRate;
    float m_currentSampleIndex;
    float m_sampleDelta;
};



#endif //MUSI6106_WAVETABLEOSCILLATOR_H

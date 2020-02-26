//
// Created by Kaushal Sali on 15/02/20.
//

#include "LFO.h"
#include "Synthesis.h"

#include<iostream>


LFO::LFO(int waveTableSize) :
        m_waveTableSize(waveTableSize),
        m_pWaveTable(new CRingBuffer<float> (waveTableSize)),
        m_frequency(0.0f),
        m_sampleRate(0),
        m_currentSampleIndex(0.0f),
        m_sampleDelta(0.0f)
{
}


float LFO::getNextSample()
{
    auto sample = m_pWaveTable->get(m_currentSampleIndex);
    m_currentSampleIndex = fmod((m_currentSampleIndex + m_sampleDelta), m_waveTableSize);
    return sample;
}


Error_t LFO::setWavefromType(Waveform waveformType)
{
    m_waveformType = waveformType;
    return kNoError;
}


LFO::Waveform LFO::getWavefromType() const
{
    return m_waveformType;
}


Error_t LFO::setFrequency(float frequencyInHz)
{
    m_frequency = frequencyInHz;
    m_sampleDelta = (float)m_waveTableSize * m_frequency / (float)m_sampleRate;
    //std::cout << "m_sampleDelta = " << m_sampleDelta << std::endl; //TODO: Delete
    return kNoError;
}


float LFO::getFrequency() const
{
    return m_frequency;
}


Error_t LFO::setSampleRate(int sampleRateInHz)
{
    m_sampleRate = sampleRateInHz;
    return kNoError;
}


int LFO::getSampleRate() const
{
    return m_sampleRate;
}


float LFO::getCurrentSampleIndex() const
{
    return m_currentSampleIndex;
}


float LFO::getSampleDelta() const
{
    return m_sampleDelta;
}


Error_t LFO::updateWaveTable()
{
    // TODO: Is it better to update sampleDelta here??
    // TODO: Mechanism to check what has been updated and accordingl
    // Generate waveform and fill WaveTable
    auto* waveform = new float[m_waveTableSize];

    // For sampling the waveform such that wavetable size equals one period.
    // Note: This can also be done with arbitrary values. As long as 'Fs/F = wavetable_size' holds,
    // you'll get one period in wavetable_size samples from sin func.
    //TODO: Use arbitrary const values : what if  m_waveTableSize or m_frequency are zero???;
    auto waveformSampleRate = (float)m_waveTableSize * m_frequency;

    switch (m_waveformType) {
        case Waveform::Sine:
            CSynthesis::generateSine(waveform, m_frequency, waveformSampleRate, m_waveTableSize);
            break;
        case Waveform::Rectangle:
            CSynthesis::generateRect(waveform, m_frequency, waveformSampleRate, m_waveTableSize);
            break;
        case Waveform::Saw:
            CSynthesis::generateSaw(waveform, m_frequency, waveformSampleRate, m_waveTableSize);
            break;
    }
    m_pWaveTable->put(waveform, m_waveTableSize);
    return kNoError;
}


Error_t LFO::init(LFO::Waveform waveformType, float frequencyInHz, int sampleRateInHz)
{
    setWavefromType(waveformType);
    setSampleRate(sampleRateInHz);
    setFrequency(frequencyInHz);
    updateWaveTable();
}


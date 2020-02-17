//
// Created by Kaushal Sali on 15/02/20.
//

#include "WaveTableOscillator.h"
#include "Synthesis.h"


WaveTableOscillator::WaveTableOscillator(int waveTableSize) :
        m_waveTableSize(waveTableSize),
        m_pWaveTable(new CRingBuffer<float> (waveTableSize)),
        m_frequency(0.0f),
        m_sampleRate(0),
        m_currentSampleIndex(0.0f),
        m_sampleDelta(0.0f)
{
}


float WaveTableOscillator::getNextSample()
{
    auto sample = m_pWaveTable->get(m_currentSampleIndex);
    m_currentSampleIndex = fmod((m_currentSampleIndex + m_sampleDelta), m_waveTableSize);
    return sample;
}


Error_t WaveTableOscillator::setWavefromType(Waveform waveformType)
{
    m_waveformType = waveformType;
    return kNoError;
}


WaveTableOscillator::Waveform WaveTableOscillator::getWavefromType() const
{
    return m_waveformType;
}


Error_t WaveTableOscillator::setFrequency(float frequencyInHz)
{
    m_frequency = frequencyInHz;
    auto cyclesPerSample = m_frequency / (float)m_sampleRate;
    m_sampleDelta = (float)m_waveTableSize * cyclesPerSample;
    return kNoError;
}


float WaveTableOscillator::getFrequency() const
{
    return m_frequency;
}


Error_t WaveTableOscillator::setSampleRate(int sampleRateInHz)
{
    m_sampleRate = sampleRateInHz;
    return kNoError;
}


int WaveTableOscillator::getSampleRate() const
{
    return m_sampleRate;
}


float WaveTableOscillator::getCurrentSampleIndex() const
{
    return m_currentSampleIndex;
}


float WaveTableOscillator::getSampleDelta() const
{
    return m_sampleDelta;
}


Error_t WaveTableOscillator::updateWaveTable()
{
    // Generate waveform and fill WaveTable
    auto* waveform = new float[m_waveTableSize];

    // For sampling the waveform such that wavetable size equals one period.
    // Note: This can also be done with arbitrary values. As long as 'Fs/F = wavetable_size' holds,
    // you'll get one period in wavetable_size samples from sin func.
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


Error_t WaveTableOscillator::init(WaveTableOscillator::Waveform waveformType, float frequencyInHz, int sampleRateInHz)
{
    setWavefromType(waveformType);
    setFrequency(frequencyInHz);
    setSampleRate(sampleRateInHz);
    updateWaveTable();
}


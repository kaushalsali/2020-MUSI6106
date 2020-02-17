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
        m_currentSample(0.0f),
        m_sampleDelta(0.0f)
{
}


float WaveTableOscillator::getNextSample() {
    auto sample = m_pWaveTable->get(m_currentSample);
    m_currentSample = fmod((m_currentSample + m_sampleDelta), m_waveTableSize);
    return sample;
}


Error_t WaveTableOscillator::setWavefromType(Waveform waveformType) {
    m_waveformType = waveformType;
    return kNoError;
}


Error_t WaveTableOscillator::setFrequency(float frequencyInHz) {
    m_frequency = frequencyInHz;
    return kNoError;
}


Error_t WaveTableOscillator::setSampleRate(int sampleRateInHz) {
    m_sampleRate = sampleRateInHz;
    return kNoError;
}


Error_t WaveTableOscillator::updateWaveTable() {
    auto cyclesPerSample = m_frequency / m_sampleRate;
    m_sampleDelta = m_waveTableSize * cyclesPerSample; // sample resolution //TODO: Check

    // Generate waveform and fill WaveTable
    auto* waveform = new float[m_waveTableSize];
    auto waveformSampleRate = m_waveTableSize * m_frequency; //for sampling the waveform such that wavetable size equals one period.
    switch (m_waveformType) { //TODO: Check this
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


Error_t WaveTableOscillator::init(WaveTableOscillator::Waveform waveformType, float frequencyInHz, int sampleRateInHz) {
    setWavefromType(waveformType);
    setFrequency(frequencyInHz);
    setSampleRate(sampleRateInHz);
    updateWaveTable();
}

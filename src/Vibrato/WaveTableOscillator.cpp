//
// Created by Kaushal Sali on 15/02/20.
//

#include "WaveTableOscillator.h"
#include "Synthesis.h"

template<class T>
WaveTableOscillator<T>::WaveTableOscillator(int waveTableSize) :
        m_waveTableSize(waveTableSize),
        m_pWaveTable(new CRingBuffer<T> (waveTableSize))
{
}

template<class T>
T WaveTableOscillator<T>::getNextSample() {
    auto sample = m_pWaveTable->get(m_currentSample);
    m_currentSample += m_sampleDelta % m_waveTableSize;
    return sample;
}

template<class T>
Error_t WaveTableOscillator<T>::setWavefromType(Waveform waveformType) {
    m_waveformType = waveformType;
    return kNoError;
}

template<class T>
Error_t WaveTableOscillator<T>::setFrequency(float frequencyInHz) {
    m_frequency = frequencyInHz;
    return kNoError;
}

template<class T>
Error_t WaveTableOscillator<T>::setSampleRate(int sampleRateInHz) {
    m_sampleRate = sampleRateInHz;
    return kNoError;
}

template<class T>
Error_t WaveTableOscillator<T>::updateWaveTable() {
    auto cyclesPerSample = m_frequency / m_sampleRate;
    m_sampleDelta = m_waveTableSize * cyclesPerSample; // sample resolution //TODO: Check

    // Generate waveform and fill WaveTable
    auto* waveform = new float[m_waveTableSize];
    auto waveformSampleRate = m_waveTableSize * m_frequency; //for sampling the waveform such that wavetable size equal one period.
    switch (m_waveformType) {
        case Waveform::Sine:
            CSynthesis::generateSine(waveform, waveformSampleRate, m_waveTableSize);
            break;
        case Waveform::Rectangle:
            CSynthesis::generateRect(waveform, waveformSampleRate, m_waveTableSize);
            break;
        case Waveform::Saw:
            CSynthesis::generateSaw(waveform, waveformSampleRate, m_waveTableSize);
            break;
    }
    m_pWaveTable->put(waveform, m_waveTableSize);
    return kNoError;
}

template<class T>
Error_t WaveTableOscillator<T>::init(WaveTableOscillator::Waveform waveformType, float frequencyInHz, int sampleRateInHz) {
    setWavefrom(waveformType);
    setFrequency(frequencyInHz);
    setSampleRate(sampleRateInHz);
    updateWaveTable();
}

#include "Vibrato.h"
#include "RingBuffer.h"
#include "LFO.h"

#include<iostream>

Vibrato::Vibrato(float maxDelayInSec, float modulationWidthInSec, float LFOFreqInHz, float sampleRateInHz, int numChannels) :
        m_maxDelayInSec(maxDelayInSec),
        m_delayInSec(maxDelayInSec),
        m_widthInSec(modulationWidthInSec),
        m_LFOFreqInHz(LFOFreqInHz),
        m_sampleRate(sampleRateInHz),
        m_numChannels(numChannels)
{
    // Create and initialize delay line with zeros

    m_delayInSamples = (int) std::round(m_maxDelayInSec * m_sampleRate);
    m_widthInSamples = (int) std::round(m_widthInSec * m_sampleRate);

    auto bufferLength = m_delayInSamples + (int) std::ceil(m_widthInSamples / 2.0f);
    auto* zeros = new float[bufferLength]();

    m_ppBuffer = new CRingBuffer<float>* [numChannels];
    for (int i=0; i<numChannels; i++) {
        m_ppBuffer[i] = new CRingBuffer<float>(bufferLength);
        m_ppBuffer[i]->put(zeros, bufferLength);
        m_ppBuffer[i]->setReadIdx(std::ceil(m_widthInSamples / 2.0f));
    }
    delete[] zeros;
    zeros = nullptr;

    // Create LFO
    m_pLFO = new LFO(4096);
    m_pLFO->init(LFO::Waveform::Sine, LFOFreqInHz, sampleRateInHz);
}


Vibrato::~Vibrato() {
    delete m_pLFO;
    for (int i=0; i<m_numChannels; i++)
        delete m_ppBuffer[i];
    delete[] m_ppBuffer;
}


Error_t Vibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, int numFrames) {
    for (int i = 0; i < numFrames; i++)
    {
        auto indexOffset = m_pLFO->getNextSample() * (float) m_widthInSamples;
        for(int c = 0; c < m_numChannels; c++)
        {
            ppfOutputBuffer[c][i] = m_ppBuffer[c]->get(indexOffset);
            m_ppBuffer[c]->getPostInc(); // Increment read by 1
            m_ppBuffer[c]->putPostInc(ppfInputBuffer[c][i]); // Fill buffer and Increment write by 1
        }
    }
    return kNoError;
}

float Vibrato::getParam(Vibrato::VibratoParams param) {
    switch(param) {
        case delayInSec:
            return m_delayInSec;
        case widthInSec:
            return m_widthInSec;
        case LFOFreqInHz:
            return m_LFOFreqInHz;
    }

}

Error_t Vibrato::setParam(Vibrato::VibratoParams param, float value) {
    switch(param) {
        case delayInSec:
            setDelay(value);
            break;
        case widthInSec:
            setWidth(value);
            break;
        case LFOFreqInHz:
            m_LFOFreqInHz = value;
    }
    return kNoError;
}

Error_t Vibrato::setDelay(float delayInSec) {
    m_delayInSec = delayInSec;
    m_delayInSamples = (int) std::round(m_delayInSec * m_sampleRate);
    updateBufferLength();
    return  kNoError;
}

Error_t Vibrato::setWidth(float widthInSec) {
    m_widthInSec = widthInSec;
    m_widthInSamples = (int) std::round(m_widthInSec * m_sampleRate);
    updateBufferLength();
    return kNoError;
}

Error_t Vibrato::setLFOFreq(float freqInHz) {
    m_LFOFreqInHz = freqInHz;
    m_pLFO->setFrequency(m_LFOFreqInHz);
    return kNoError;
}

/*
 * Updates the buffer length acc to changes in width and delay.
 * Resets and initializes buffer with zeros. Read ptr is set to delay length.
 */
Error_t Vibrato::updateBufferLength() {
    auto bufferLength = m_delayInSamples + (int) std::ceil(m_widthInSamples / 2.0f);
    auto* zeros = new float[bufferLength]();
    for (int c=0; c<m_numChannels; c++) {
        m_ppBuffer[c]->setLength(bufferLength);
        m_ppBuffer[c]->put(zeros, bufferLength);
        m_ppBuffer[c]->setReadIdx(std::ceil(m_widthInSamples / 2.0f));
    }
    return kFunctionIllegalCallError;
}





////TODO: Remove after debugging
// Write LFO output to file.
//Error_t Vibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, int numFrames) {
//    std::cout << "---------------------------------------------------" << std::endl;
//    for (int i = 0; i < numFrames; i++)
//    {
//        auto d = m_pLFO->getCurrentSampleIndex();
//        auto v = m_pLFO->getNextSample();
//        std::cout <<i << " " << d << "--" << v << std::endl;
//        for(int c = 0; c < m_numChannels; c++)
//        {
//            ppfOutputBuffer[c][i] = v;
//        }
//    }
//    return kNoError;
//}





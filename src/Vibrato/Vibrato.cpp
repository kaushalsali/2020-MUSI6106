#include "Vibrato.h"
#include "RingBuffer.h"
#include "LFO.h"

#include<iostream>

Vibrato::Vibrato(float delayInSec, float modulationWidthInSec, float LFOFreqInHz, int sampleRateInHz, int numChannels) :
        m_delayInSec(delayInSec),
        m_widthInSec(modulationWidthInSec),
        m_LFOFreqInHz(LFOFreqInHz),
        m_sampleRate(sampleRateInHz),
        m_numChannels(numChannels)
{
    // Create and initialize delay line with zeros
    m_delayInSamples = (int) std::round(delayInSec * sampleRateInHz);
    m_widthInSamples = (int) std::round(modulationWidthInSec * sampleRateInHz);
    auto bufferLength = m_delayInSamples + (int) std::ceil(m_widthInSamples / 2);
    auto* zeros = new float[bufferLength]();
    m_ppBuffer = new CRingBuffer<float>* [numChannels];
    for (int i=0; i<numChannels; i++) {
        m_ppBuffer[i] = new CRingBuffer<float>(bufferLength);
        m_ppBuffer[i]->put(zeros, bufferLength);
        m_ppBuffer[i]->setReadIdx(std::ceil(m_widthInSamples / 2)); //TODO: Should this be ceil?
    }
    delete[] zeros;
    zeros = nullptr;

    // Create LFO
    m_pLFO = new LFO(1024);
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





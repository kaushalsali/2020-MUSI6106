#include "Vibrato.h"
#include "RingBuffer.h"
#include "WaveTableOscillator.h"

Vibrato::Vibrato(int delayInSec, int modulationWidthInSec, int sampleRateInHz, int numChannels) :
    m_delayInSec(delayInSec),
    m_widthInSec(modulationWidthInSec),
    m_sampleRate(sampleRateInHz),
    m_numChannels(numChannels)
{
    auto delayInSamples = delayInSec * sampleRateInHz;
    auto widthInSamples = modulationWidthInSec * sampleRateInHz;
    for (int i=0; i<numChannels; i++)
        m_ppBuffer[i] = new CRingBuffer<float>(delayInSamples + (int) std::ceil(widthInSamples / 2)); //TODO: Use Template

    m_pLFO = new WaveTableOscillator(1024);
    m_pLFO->init(WaveTableOscillator::Waveform::Sine,
}


Vibrato::~Vibrato() {
    delete m_pLFO;
    for (int i=0; i<m_numChannels; i++)
        delete m_ppBuffer[i];
    delete[] m_ppBuffer;
}


Error_t Vibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, int numFrames) {
    for(int c = 0; c < m_numChannels; c++)
    {
        for (int i = 0; i < numFrames; i++)
        {
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i] + m_ppBuffer[c]->getPostInc();
            m_ppBuffer[c]->putPostInc(ppfOutputBuffer[c][i]);
        }
    }
    return kNoError;
}




//
// Created by Kaushal Sali on 16/02/20.
//

//
// Created by Kaushal Sali on 16/02/20.
//
#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <iostream>
#include <cassert>

#include "UnitTest++.h"
#include "LFO.h"



SUITE(LFO) {

    struct LFOData {
    public:
        LFOData() :
            m_tableSize(44100/4),
            m_waveform(LFO::Waveform::Sine),
            m_freq(441.0f),
            m_sampleRate(44100)
        {
            m_pWaveTable = new LFO(m_tableSize);
        }

        ~LFOData() {
            delete m_pWaveTable;
        }

        int m_tableSize;
        float m_freq;
        int m_sampleRate;
        LFO::Waveform m_waveform;
        LFO* m_pWaveTable;

    };

    TEST_FIXTURE(LFOData, SineGeneration)
    // Test Passes when tablesize == integer factor of 44100 (Fs)
    {
        auto dataLength = m_sampleRate * 3;
        auto* target = new float[dataLength];
        auto* output = new float[dataLength];

        CSynthesis::generateSine (target, 5.F, m_sampleRate, dataLength, 1.0F);

        m_pWaveTable->init(m_waveform, 5.F, m_sampleRate);
        for (int i=0; i<dataLength; i++) {
            output[i] = m_pWaveTable->getNextSample();
            std::cout << output[i] << "  " << target[i] << std::endl;
        }

        CHECK_ARRAY_CLOSE(target,output, dataLength, 1e-3);
    }
}

#endif
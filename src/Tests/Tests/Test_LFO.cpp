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



SUITE(WaveTable) {

    struct WaveTableOscillatorData {
    public:
        WaveTableOscillatorData() :
            m_tableSize(128),
            m_waveform(LFO::Waveform::Sine),
            m_freq(441.0f),
            m_sampleRate(44100)
        {
            m_pWaveTable = new LFO(m_tableSize);
        }

        ~WaveTableOscillatorData() {
            delete m_pWaveTable;
        }

        int m_tableSize;
        float m_freq;
        int m_sampleRate;
        LFO::Waveform m_waveform;
        LFO* m_pWaveTable;
    };

    TEST_FIXTURE(WaveTableOscillatorData, Test1) //TODO: Rename Test
    {
        std::cout << "Hello Test" <<std::endl;
        m_pWaveTable->init(m_waveform, m_freq, m_sampleRate);
        for (int i=0; i<128*5; i++) {
            // std::cout << m_pWaveTable->getCurrentSampleIndex() << " -- " << m_pWaveTable->getNextSample() << std::endl;
        }
    }
}

#endif
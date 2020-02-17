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
#include "WaveTableOscillator.h"



SUITE(WaveTable) {

    struct WaveTableOscillatorData {
    public:
        WaveTableOscillatorData() :
            m_tableSize(128),
            m_waveform(WaveTableOscillator::Waveform::Sine),
            m_freq(440.0f),
            m_sampleRate(44100)
        {
            m_pWaveTable = new WaveTableOscillator(m_tableSize);
        }

        ~WaveTableOscillatorData() {
            delete m_pWaveTable;
        }

        int m_tableSize;
        float m_freq;
        int m_sampleRate;
        WaveTableOscillator::Waveform m_waveform;
        WaveTableOscillator* m_pWaveTable;
    };

    TEST_FIXTURE(WaveTableOscillatorData, Test1) //TODO: Rename Test
    {
        std::cout << "Hello Test" <<std::endl;
        m_pWaveTable->init(m_waveform, m_freq, m_sampleRate);

    }
}

#endif
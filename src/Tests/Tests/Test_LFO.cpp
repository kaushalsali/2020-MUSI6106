//
// Created by Kaushal Sali on 16/02/20.
//

//
// Created by Kaushal Sali on 16/02/20.
//
#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>

#include "UnitTest++.h"
#include "WaveTableOscillator.h"

WaveTableOscillator<int> l;

SUITE(WaveTable) {

    struct WaveTableOscillatorData
    {
    public:
        WaveTableOscillatorData(int waveTableSize, WaveTableOscillator<void>::Waveform waveform, float freqInHz, float sampleRateInHz) :
            m_tableSize(waveTableSize),
            m_waveform(waveform),
            m_freq(freqInHz),
            m_sampleRate(sampleRateInHz),
            m_pWaveTable(new WaveTableOscillator<float>(waveTableSize))
        {
        }

        ~WaveTableOscillatorData()
        {
            delete m_pWaveTable;
        }

    private:
        int m_tableSize;
        float m_freq;
        float m_sampleRate;
        WaveTableOscillator<void>::Waveform m_waveform;
        WaveTableOscillator<float>* m_pWaveTable;
    };
}

#endif
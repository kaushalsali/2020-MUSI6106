#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>
#include <algorithm>

#include "UnitTest++.h"

#include "Vibrato.h"
#include "Vector.h"

SUITE(Vibrato) {
    struct VibratoData {
        VibratoData() :
                m_pVibrato(nullptr),
                m_ppfInputData(nullptr),
                m_ppfOutputData(nullptr),
                m_iDataLength(35131),
                m_iBlockLength(171),
                m_iNumChannels(3),
                m_fSampleRate(8000),
                m_maxDelayInSec(10),
                m_delayInSec(1),
                m_widthInSec(0.001),
                m_LFOFreqInHz(5) {
            m_pVibrato = new Vibrato(m_maxDelayInSec, m_widthInSec, m_LFOFreqInHz, m_fSampleRate, m_iNumChannels);
            m_ppfInputData = new float *[m_iNumChannels];
            m_ppfOutputData = new float *[m_iNumChannels];
            m_ppfInputTmp = new float *[m_iNumChannels];
            m_ppfOutputTmp = new float *[m_iNumChannels];
            for (int i = 0; i < m_iNumChannels; i++) {
                m_ppfInputData[i] = new float[m_iDataLength](); // () initializes to zero
                m_ppfOutputData[i] = new float[m_iDataLength]();
            }
        }

        ~VibratoData() {
            for (int i = 0; i < m_iNumChannels; i++) {
                delete[] m_ppfOutputData[i];
                delete[] m_ppfInputData[i];
            }
            delete[] m_ppfOutputTmp;
            delete[] m_ppfInputTmp;
            delete[] m_ppfOutputData;
            delete[] m_ppfInputData;

            delete m_pVibrato;
        }

        void TestProcess() {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0) {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLength);

                for (int c = 0; c < m_iNumChannels; c++) {
                    m_ppfInputTmp[c] = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                    m_ppfOutputTmp[c] = &m_ppfOutputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_pVibrato->process(m_ppfInputTmp, m_ppfOutputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }

        Vibrato *m_pVibrato;
        float **m_ppfInputData;
        float **m_ppfOutputData;
        float **m_ppfInputTmp;
        float **m_ppfOutputTmp;
        int m_iDataLength;
        int m_iBlockLength;
        int m_iNumChannels;

        float m_fSampleRate;
        float m_delayInSec;
        float m_maxDelayInSec;
        float m_widthInSec;
        float m_LFOFreqInHz;
    };

    TEST_FIXTURE (VibratoData, NoModulation) {

        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateSine(m_ppfInputData[c], 441.0, m_fSampleRate, m_iDataLength, 1);

        TestProcess();

        int delaySamples = m_delayInSec * m_fSampleRate;
        for (int c = 0; c < m_iNumChannels; c++)
                    CHECK_ARRAY_CLOSE (m_ppfInputData[c], m_ppfOutputData[c] + delaySamples,
                                       m_iDataLength - delaySamples, 1e-3);
    }


    TEST_FIXTURE (VibratoData, ZeroInput) {

        for (int c = 0; c < m_iNumChannels; c++)
            CVectorFloat::setZero(m_ppfInputData[c], m_iDataLength);

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE (m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);
    }

    TEST_FIXTURE (VibratoData, DCInput) {

        for (int c = 0; c < m_iNumChannels; c++)
            CVectorFloat::setValue(m_ppfInputData[c], 1.0f, m_iDataLength);

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE (m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);
    }

    TEST_FIXTURE (VibratoData, VaryingBlocksize) {

        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateSine(m_ppfInputData[c], 387.F, m_fSampleRate, m_iDataLength, .8F,
                                     static_cast<float>(c * M_PI_2));

        // Process on same size blocks
        TestProcess();

        // Copy same size block output to temporary variable.
        auto** target = new float* [m_iNumChannels];
        for (int c=0; c<m_iNumChannels; c++) {
            target[c] = new float[m_iDataLength];
            CVectorFloat::copy(target[c], m_ppfOutputData[c], m_iDataLength);
        }

        // Reset and init Vibrato buffer with zeros.
        m_pVibrato->resetBuffer();

        //Process for varying block size
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0) {

                int iNumFrames = std::min(static_cast<float>(iNumFramesRemaining),
                                          static_cast<float>(rand()) / RAND_MAX * 17000.F);

                for (int c = 0; c < m_iNumChannels; c++) {
                    m_ppfInputTmp[c] = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_pVibrato->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }

        for (int c = 0; c < m_iNumChannels; c++)
                    CHECK_ARRAY_CLOSE (target[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

    }
}
#endif //WITH_TESTS
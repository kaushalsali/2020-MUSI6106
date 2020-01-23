//
// Created by Kaushal Sali on 19/01/20.
//

#ifndef MUSI6106_COMBFILTER_H
#define MUSI6106_COMBFILTER_H


//#include <CombFilterIf.h>
#include <AudioFileIf.h>

class AudioRingBuffer;

// Abstract class
class CCombFilterBase {
public:
    CCombFilterBase(int delayLineLength, int numChannels, float gain=0.5);
    virtual ~CCombFilterBase();
    virtual Error_t filter(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) = 0;
    float getGain();
    Error_t setGain(float gain);
    int getDelayLineLength();
    Error_t setDelayLineLength(int delayLineLength);


protected:
    int delayLineLength;
    int numChannels;
    float gain;
    AudioRingBuffer *delayLine;
};


class CCombFilterFIR : public CCombFilterBase {
public:
    CCombFilterFIR(int delayLineLength, int numChannels, float gain=0.5);
    Error_t filter(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) override;
};


class CCombFilterIIR : public CCombFilterBase {
public:
    CCombFilterIIR(int delayLineLength, int numChannels, float gain=0.5);
    Error_t filter(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) override;
};



class AudioRingBuffer {  // Note: Ideally should be placed separately outside of CombFilter folder.
public:
    AudioRingBuffer(int bufferLength, int numChannels);
    ~AudioRingBuffer();
    Error_t insert(float **audioBuffer, int sampleIndex);
    Error_t fetch(float *sampleBuffer);
    Error_t remove();

private:
    int bufferLength;
    int numChannels;
    float **buffer;
    int front;
    int rear;
    bool empty;
    bool full;
};

#endif //MUSI6106_COMBFILTER_H



//
// Created by Kaushal Sali on 19/01/20.
//

#ifndef MUSI6106_COMBFILTER_H
#define MUSI6106_COMBFILTER_H


#include <CombFilterIf.h>
#include <AudioFileIf.h>

class AudioRingBuffer;

// Abstract class
class CCombFilterBase {  // TODO: Why is this declared in the Interface????
public:
    CCombFilterBase(int delayLineLength, int numChannels, float g=0.5);
    virtual ~CCombFilterBase();
    virtual Error_t filter(float **&ppfInputBuffer, float **&ppfOutputBuffer, int iNumberOfFrames) = 0;

protected:
    int delayLineLength;
    int numChannels;
    float g;
    AudioRingBuffer *delayLine;
};


class CCombFilterFIR : public CCombFilterBase {
public:
    CCombFilterFIR(int delayLineLength, int numChannels, float g=0.5);
    Error_t filter(float **&ppfInputBuffer, float **&ppfOutputBuffer, int iNumberOfFrames) override;
};


class CCombFilterIIR : public CCombFilterBase {
public:
    CCombFilterIIR(int delayLineLength, int numChannels, float g = 0.5);
    Error_t filter(float **&ppfInputBuffer, float **&ppfOutputBuffer, int iNumberOfFrames) override;
};



class AudioRingBuffer {
public:
    AudioRingBuffer(int bufferLength, int numChannels); // TODO: Can you specify default values here in .h?
    ~AudioRingBuffer();
    bool insert(float **&audioBuffer, int sampleIndex);
    bool fetch(float *&sampleBuffer);
    bool remove();

private:
    int bufferLength;
    int numChannels;
    float **buffer;
    int front;
    int rear;
    bool empty;
    bool full;

//    AudioRingBuffer(int bufferLength, int numChannels, int front=0, int rear=0, bool empty=true, bool full=false);
};

#endif //MUSI6106_COMBFILTER_H



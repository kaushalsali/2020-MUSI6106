//
// Created by Kaushal Sali on 19/01/20.
//

#include "CombFilter.h"

CCombFilterBase::CCombFilterBase(int delayLineLength, int numChannels, float gain) :
    delayLineLength(delayLineLength),
    numChannels(numChannels),
    gain(gain),
    delayLine(new AudioRingBuffer(delayLineLength, numChannels))
{
    auto **zeros = new float *[numChannels];
    for (int c=0; c<numChannels; c++)
        zeros[c] = new float[delayLineLength]();

    for (int i=0; i<delayLineLength; i++)  // init delayLine with zeros
        delayLine->insert(zeros, i);

    for (int c=0; c<numChannels; c++)
        delete[] zeros[c];
    delete[] zeros;
    zeros = nullptr;
}

CCombFilterBase::~CCombFilterBase() {
    delete delayLine;
    delayLine = nullptr;
}

float CCombFilterBase::getGain() {
    return gain;
}

Error_t CCombFilterBase::setGain(float gain) {
    this->gain = std::max(-1.0f, std::min(gain, 1.0f));
    return kFunctionIllegalCallError;
}

int CCombFilterBase::getDelayLineLength() {
    return delayLineLength;
}

Error_t CCombFilterBase::setDelayLineLength(int delayLineLength) {
    this->delayLineLength = std::max(1, delayLineLength);
    return kFunctionIllegalCallError;
}

CCombFilterFIR::CCombFilterFIR(int delayLineLength, int numChannels, float gain) : CCombFilterBase(delayLineLength,
                                                                                                numChannels, gain) {}

Error_t CCombFilterFIR::filter(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) {
    auto *sample = new float[numChannels];
    for (int i=0; i<iNumberOfFrames; i++) {
        delayLine->fetch(sample);
        for (int c=0; c<numChannels; c++) {
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i] + gain * sample[c];
        }
        delayLine->insert(ppfInputBuffer, i);
    }
    delete[] sample;
    sample = nullptr;
    return kFunctionIllegalCallError;
}

CCombFilterIIR::CCombFilterIIR(int delayLineLength, int numChannels, float gain) : CCombFilterBase(delayLineLength,
                                                                                                numChannels, gain) {}

Error_t CCombFilterIIR::filter(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) {
    auto *sample = new float[numChannels];
    for (int i=0; i<iNumberOfFrames; i++) {
        delayLine->fetch(sample);
        for (int c=0; c<numChannels; c++) {
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i] + gain * sample[c];
        }
        delayLine->insert(ppfOutputBuffer, i);
    }
    delete[] sample;
    sample = nullptr;
    return kFunctionIllegalCallError;
}

AudioRingBuffer::AudioRingBuffer(int bufferLength, int numChannels) :
    bufferLength(bufferLength),
    numChannels(numChannels),
    front(0),
    rear(0),
    empty(true),
    full(false)
{
    buffer = new float *[numChannels];
    for(int c=0; c<numChannels; c++) {
        buffer[c] = new float[bufferLength];
    }
}

AudioRingBuffer::~AudioRingBuffer() {
    for (int c=0; c<numChannels; c++)
        delete[] buffer[c];
    delete[] buffer;
    buffer = nullptr;
}

Error_t AudioRingBuffer::insert(float **audioBuffer, int sampleIndex) {
    if (!full) {
        for(int c=0; c<numChannels; c++) {
            buffer[c][rear] = audioBuffer[c][sampleIndex];
        }
        rear = (rear+1) % bufferLength;
        if (empty)
            empty = false;
        if (rear == front)
            full = true;
        return kNoError;
    }
    else
        return kOperationUnsuccessful;
}

Error_t AudioRingBuffer::fetch(float *sampleBuffer) {
    if (!empty) {
        for (int c = 0; c < numChannels; c++) {
            sampleBuffer[c] = buffer[c][front];
        }
        front = (front + 1) % bufferLength;
        if (full)
            full = false;
        if (front == rear)
            empty = true;
        return kNoError;
    }
    else
        return kOperationUnsuccessful;
}

Error_t AudioRingBuffer::remove() {
    if (!empty) {
        front = (front + 1) % bufferLength;
        if (full)
            full = false;
        if (front == rear)
            empty = true;
        return kNoError;
    }
    else
        return kOperationUnsuccessful;
}


/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Christof Ruch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "RingBuffer.h"

RingBuffer::RingBuffer(int numChannelsToAllocate, int numSamplesToAllocate) : AbstractFifo(numSamplesToAllocate)
{
    ringBuffer_.setSize(numChannelsToAllocate, numSamplesToAllocate);
    ringBuffer_.clear();
}

void RingBuffer::write(const float* const* channelPointers, int numChannels, int numSamples)
{
    jassert(numChannels <= ringBuffer_.getNumChannels());
    int startIndex1, blockSize1, startIndex2, blockSize2;
    prepareToWrite(numSamples, startIndex1, blockSize1, startIndex2, blockSize2);
    if (blockSize1 > 0) {
        for (int c = 0; c < std::min(numChannels, ringBuffer_.getNumChannels()); c++) {
            ringBuffer_.copyFrom(c, startIndex1, channelPointers[c], blockSize1);
        }
    }
    if (blockSize2 > 0) {
        for (int c = 0; c < std::min(numChannels, ringBuffer_.getNumChannels()); ++c) {
            ringBuffer_.copyFrom(c, startIndex2, channelPointers[c] + blockSize1, blockSize2);
        }
    }
    finishedWrite(blockSize1 + blockSize2);
}

void RingBuffer::read(float** channelPointers, int numChannels, int numSamples)
{
    jassert(numChannels == ringBuffer_.getNumChannels());
    int startIndex1, blockSize1, startIndex2, blockSize2;
    prepareToRead(numSamples, startIndex1, blockSize1, startIndex2, blockSize2);
    if (blockSize1 > 0) {
        for (int c = 0; c < std::min(numChannels, ringBuffer_.getNumChannels()); ++c) {
            FloatVectorOperations::copy(channelPointers[c], ringBuffer_.getReadPointer(c, startIndex1), blockSize1);
        }
    }
    if (blockSize2 > 0) {
        for (int c = 0; c < std::min(numChannels, ringBuffer_.getNumChannels()); ++c) {
            FloatVectorOperations::copy(channelPointers[c] + blockSize1, ringBuffer_.getReadPointer(c, blockSize2), blockSize2);
        }
    }
    finishedRead(blockSize1 + blockSize2);
}

void RingBuffer::addBuffer(std::shared_ptr<AudioBuffer<float>> bufferToAdd)
{
    write(bufferToAdd->getArrayOfReadPointers(), bufferToAdd->getNumChannels(), bufferToAdd->getNumSamples());
}

void RingBuffer::readPastData(std::shared_ptr<AudioBuffer<float>> outBuffer, int samplesOffset)
{
    outBuffer->clear();

    // We want to read past (old) data from the ring buffer - this is implemented by preparing to read everything from back then to now,
    // and then really copying only the old data we need!
    jassert(samplesOffset <= getTotalSize());
    if (samplesOffset > getTotalSize()) {
        // That wouldn't work, output nothing
        return;
    }

    if (samplesOffset <= getNumReady()) {
        int dataToReallyRead = outBuffer->getNumSamples();

        // We have enough data to read from in our ring buffer (but we won't need all of it!)
        int startIndex1, blockSize1, startIndex2, blockSize2;
        prepareToRead(samplesOffset, startIndex1, blockSize1, startIndex2, blockSize2);
        
        if (blockSize1 > 0) {
            int dataToReadFromBlock1 = std::min(dataToReallyRead, blockSize1);
            for (int c = 0; c < std::min(outBuffer->getNumChannels(), ringBuffer_.getNumChannels()); ++c) {
                FloatVectorOperations::copy(outBuffer->getWritePointer(c), ringBuffer_.getReadPointer(c, startIndex1), dataToReadFromBlock1);
            }
        }
        if (blockSize2 > 0 && dataToReallyRead > blockSize1) {
            int dataToReadFromBlock2 = std::min(dataToReallyRead - blockSize1, blockSize2);
            for (int c = 0; c < std::min(outBuffer->getNumChannels(), ringBuffer_.getNumChannels()); ++c) {
                FloatVectorOperations::copy(outBuffer->getWritePointer(c), ringBuffer_.getReadPointer(c, startIndex2), dataToReadFromBlock2);
            }
        }
    }
    else {
        // Partial read... This should not happen when the buffer is filled with 0!
        jassertfalse;
    }
}

void RingBuffer::discard(int numSamples)
{
    jassert(getNumReady() >= numSamples);
    int startIndex1, blockSize1, startIndex2, blockSize2;
    prepareToWrite(numSamples, startIndex1, blockSize1, startIndex2, blockSize2);
    finishedRead(blockSize1 + blockSize2);
}

void RingBuffer::clear()
{
    ringBuffer_.clear();
    reset();
}

void RingBuffer::zeroReset()
{
    clear();
    finishedWrite(getFreeSpace());
}

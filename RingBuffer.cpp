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

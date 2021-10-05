/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "RingBuffer.h"

RingBuffer::RingBuffer(int numChannelsToAllocate, int numSamplesToAllocate) : AbstractFifo(numSamplesToAllocate)
{
	ringBuffer_.setSize(numChannelsToAllocate, numSamplesToAllocate);
	ringBuffer_.clear();
}

void RingBuffer::write(const float* const* channelPointers, int numSamples)
{
	int startIndex1, blockSize1, startIndex2, blockSize2;
	prepareToWrite(numSamples, startIndex1, blockSize1, startIndex2, blockSize2);
	if (blockSize1 > 0) {
		for (int c = 0; c < ringBuffer_.getNumChannels(); c++) {
			ringBuffer_.copyFrom(c, startIndex1, channelPointers[c], blockSize1);
		}
	}
	if (blockSize2 > 0) {
		for (int c = 0; c < ringBuffer_.getNumChannels(); ++c) {
			ringBuffer_.copyFrom(c, startIndex2, channelPointers[c] + blockSize1, blockSize2);
		}
	}
	finishedWrite(blockSize1 + blockSize2);
}

void RingBuffer::read(float** channelPointers, int numSamples)
{
	int startIndex1, blockSize1, startIndex2, blockSize2;
	prepareToRead(numSamples, startIndex1, blockSize1, startIndex2, blockSize2);
	if (blockSize1 > 0) {
		for (int c = 0; c < ringBuffer_.getNumChannels(); ++c) {
			FloatVectorOperations::copy(channelPointers[c], ringBuffer_.getReadPointer(c, startIndex1), blockSize1);
		}
	}
	if (blockSize2 > 0) {
		for (int c = 0; c < ringBuffer_.getNumChannels(); ++c) {
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

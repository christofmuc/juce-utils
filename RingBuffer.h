/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

// Use the JUCE AbstractFifo class to build the most minimalistic RingBuffer for sample data
class RingBuffer: public AbstractFifo
{
public:
	RingBuffer(int numChannelsToAllocate, int numSamplesToAllocate);

    void write(const float* const* channelPointers, int numChannels, int numSamples);
    void read(float** channelPointers, int numChannels, int numSamples);

    void discard(int numSamples);

    void clear();

private:
    juce::AudioBuffer<float> ringBuffer_;
};

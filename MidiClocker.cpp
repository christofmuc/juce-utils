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

#include "MidiClocker.h"

#include "StreamLogger.h"

MidiClocker::MidiClocker()
{
}

double MidiClocker::getCurrentBPM()
{
    juce::ScopedLock lock(lock_);

    // Do we have at least one clock source?
    if (clockTimes_.size() == 0) {
        return -1;
    }
    if (clockTimes_.begin()->second.empty()) {
        // This shouldn't be possible
        jassert(false);
        return -1;
    }

    // Then, take the first one who sent us a clock!
    std::deque<double> &list = clockTimes_.begin()->second;
    double delta = 0.0;
    size_t count = 0;
    for (size_t i = 1; i < list.size(); i++) {
        delta += list[i] - list[i - 1];
        count++;
    }
    double averageSecondsPerBPM = delta / count;
    return 60.0 / averageSecondsPerBPM / 24; // MIDI Clock sends at 24 pulses per quarter note (ppqn)
}

void MidiClocker::processClockMessage(juce::String const &midiSource, juce::MidiMessage const &message)
{
    juce::ScopedLock lock(lock_);

    if (clockTimes_.find(midiSource) == clockTimes_.end()) {
        // First clock signal on this source!
        if (clockTimes_.size() > 0) {
            StreamLogger::instance() << "Warning: More than one MIDI input provides clock signals - what to do with " << midiSource << "?" << std::endl;
        }
        clockTimes_[midiSource] = std::deque<double>();
    }
    if (clockTimes_[midiSource].size() > 0) {
        if (clockTimes_[midiSource].back() == message.getTimeStamp()) {
            // Duplicate message?
            return;
        }
    }
    clockTimes_[midiSource].push_back(message.getTimeStamp());
    while (clockTimes_[midiSource].size() > kNumAverageClockTicks)
        clockTimes_[midiSource].pop_front();
}

size_t MidiClocker::numInputsWithClock() const
{
    return clockTimes_.size();
}

std::vector<juce::String> MidiClocker::inputsWithClock() const
{
    std::vector<juce::String> result;
    for (auto clock = clockTimes_.cbegin(); clock != clockTimes_.end(); clock++) {
        result.push_back(clock->first);
    }
    return result;
}

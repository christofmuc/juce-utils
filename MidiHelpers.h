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

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class MidiHelpers {
public:
    static juce::MidiMessage sysexMessage(std::vector<juce::uint8> const &data);
    static juce::MidiBuffer bufferFromMessages(std::vector<juce::MidiMessage> const &messages);
    static std::vector<juce::MidiMessage> generateRPN(int midiChannel, int parameterNumber, int value, bool isNRPN, bool use14BitValue, bool MSBbeforeLSB);
    static juce::uint8 checksum7bit(std::vector<juce::uint8> const &data);

    static bool isEmptySysex(juce::MidiMessage const &m);
    static juce::MidiBuffer removeEmptySysexMessages(juce::MidiBuffer const &midiBuffer);

    static bool equalSysexMessageContent(juce::MidiMessage const &message1, juce::MidiMessage const &message2, int digitsToCompare = -1);
    static bool isSysexMessageMatching(juce::MidiMessage const &message, std::vector<std::pair<size_t, juce::uint8>> const &indexAndContentCondition);
};

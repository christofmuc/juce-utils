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

#include "JuceHeader.h"

class MidiHelpers {
public:
    static MidiMessage sysexMessage(std::vector<uint8> const &data);
    static MidiBuffer bufferFromMessages(std::vector<MidiMessage> const &messages);
    static std::vector<MidiMessage> generateRPN(int midiChannel, int parameterNumber, int value, bool isNRPN, bool use14BitValue, bool MSBbeforeLSB);
    static uint8 checksum7bit(std::vector<uint8> const &data);

    static bool isEmptySysex(MidiMessage const &m);
    static MidiBuffer removeEmptySysexMessages(MidiBuffer const &midiBuffer);

    static bool equalSysexMessageContent(MidiMessage const &message1, MidiMessage const &message2, int digitsToCompare = -1);
    static bool isSysexMessageMatching(MidiMessage const &message, std::vector<std::pair<size_t, uint8>> const &indexAndContentCondition);
};

/*
 * MIT License
 *
 * Copyright (c) 2019-2023 Christof Ruch
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

class Sysex {
public:
    static std::vector<juce::MidiMessage> loadSysex(std::string const &filename);
    static std::vector<juce::MidiMessage> loadSysex(juce::InputStream &inputStream);
    static std::vector<juce::MidiMessage> vectorToMessages(std::vector<juce::uint8> const &midiData);
    static std::vector<juce::MidiMessage> memoryBlockToMessages(juce::MemoryBlock const &midiData);
    static void saveSysex(std::string const &filename, std::vector<juce::MidiMessage> const &messages);
    static std::string saveSysexIntoNewFile(std::string const &directory, std::string const &desiredFileName, std::vector<juce::MidiMessage> const &messages);
    static std::string dumpSysexToString(std::vector<juce::MidiMessage> const &messages);
};

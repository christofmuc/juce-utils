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

#include "MidiNote.h"

#include <juce_audio_basics/juce_audio_basics.h>

#include <cmath>

MidiNote::MidiNote(double frequency, double frequencyA4 /* = 440.0 */) : frequency_(frequency), frequencyA4_(frequencyA4)
{
    midiNote_ = frequencyToNoteNumber(frequency, frequencyA4_);
}

MidiNote::MidiNote(int noteNumber, double frequencyA4 /* = 440.0 */) : midiNote_(noteNumber), frequencyA4_(frequencyA4)
{
    frequency_ = noteNumberToFrequency(noteNumber, frequencyA4_);
}

int MidiNote::frequencyToNoteNumber(double frequency, double frequencyA4 /* = 440.0 */)
{
    // https://en.wikipedia.org/wiki/Piano_key_frequencies
    if (frequency > 0.0) {
        return (int) std::round(12 * std::log2(frequency / frequencyA4) + 69);
    }
    else {
        return 0;
    }
}

double MidiNote::noteNumberToFrequency(int noteNumber, double frequencyA4 /* = 440.0 */)
{
    // 69 is MIDI for A4
    return frequencyA4 * std::pow(2.0, (noteNumber - 69) / 12.0);
}

int MidiNote::noteNumber() const
{
    return midiNote_;
}

double MidiNote::frequency() const
{
    return frequency_;
}

double MidiNote::cents()
{
    // A wild ride: http://www.sengpielaudio.com/calculator-centsratio.htm
    if (frequency_ > 0.0) {
        return 1200.0 * std::log2(frequency_ / noteNumberToFrequency(midiNote_, frequencyA4_));
    }
    else {
        return 0.0;
    }
}

std::string MidiNote::name() const
{
    // Ok, JUCE can do this for us
    if (midiNote_ > 0) {
        return juce::MidiMessage::getMidiNoteName(midiNote_, true, true, 4).toStdString();
    }
    else {
        return "-";
    }
}

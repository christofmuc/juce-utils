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

#include "MidiTuning.h"

#include "MidiHelpers.h"

juce::MidiMessage MidiTuning::createTuningDumpRequest(juce::uint8 deviceID, MidiProgramNumber tuningBankNumber)
{
    // See e.g. http://www.microtonal-synthesis.com/MIDItuning.html
    jassert(tuningBankNumber.toZeroBased() >= 0 && tuningBankNumber.toZeroBased() < 128);
    return MidiHelpers::sysexMessage({ 0x7E, deviceID, 0x08 /* MIDI Tuning Standard */, 0x00 /* Micro tuning request */, (juce::uint8) tuningBankNumber.toZeroBased() });
}

bool MidiTuning::isTuningDump(juce::MidiMessage const &message)
{
    if (message.isSysEx()) {
        if (message.getSysExDataSize() > 3) {
            auto data = message.getSysExData();
            if (data[0] == 0x7E && data[2] == 0x08 && data[3] == 0x01 /* Tuning dump reply*/) {
                // Length of the message is not checked, it could be truncated
                return true;
            }
        }
    }
    return false;
}

bool MidiTuning::fromMidiMessage(juce::MidiMessage const &message, MidiTuning &tuningResult)
{
    if (isTuningDump(message)) {
        auto data = message.getSysExData();
        auto program = MidiProgramNumber::fromZeroBase(data[4]);
        std::string name(&data[5], &data[5 + 16]);
        const size_t tuningIndex = 5 + 16;
        TTuning tuning;
        for (size_t i = 0; i < 128; i++) {
            auto midiNoteTarget = MidiNote(data[tuningIndex + i * 3]);
            double toneAdjustCents =
                (100.0 * data[tuningIndex + i * 3 + 1] /* MSB, 1/128 of a semitone */) / 128.0 + (100.0 * data[tuningIndex + i * 3 + 2] /* LSB, 1/16384 of a semitone */) / 16384.0;
            tuning.emplace_back(midiNoteTarget, toneAdjustCents);
        }
        tuningResult = MidiTuning(program, name, tuning);
        return true;
    }
    return false;
}

MidiTuning::MidiTuning(MidiProgramNumber program, std::string const &name, TTuning const &tuning) : program_(program), name_(name), tuning_(tuning)
{
}

MidiTuning::MidiTuning() : program_(MidiProgramNumber::fromZeroBase(0))
{
}

std::string MidiTuning::name() const
{
    return name_;
}

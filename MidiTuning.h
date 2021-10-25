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

#include "MidiNote.h"
#include "MidiProgramNumber.h"

class MidiTuning {
public:
    using TTuning = std::vector<std::pair<MidiNote, double>>;

    static MidiMessage createTuningDumpRequest(uint8 deviceID, MidiProgramNumber tuningBankNumber);
    static bool isTuningDump(MidiMessage const &message);
    static bool fromMidiMessage(MidiMessage const &message, MidiTuning &tuningResult);

    MidiTuning(MidiProgramNumber program, std::string const &name, TTuning const &tuning);
    MidiTuning();

    std::string name() const;

private:
    MidiProgramNumber program_;
    std::string name_; // Limited to 16 ASCII chars by the standard
    TTuning tuning_;
};

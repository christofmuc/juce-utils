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

#include <juce_core/juce_core.h>

#include "MidiBankNumber.h"

class MidiProgramNumber {
public:
    static MidiProgramNumber invalidProgram();
    static MidiProgramNumber fromOneBase(int programNumber);
    static MidiProgramNumber fromZeroBase(int programNumber);
    static MidiProgramNumber fromOneBaseWithBank(MidiBankNumber bank, int programNumber);
    static MidiProgramNumber fromZeroBaseWithBank(MidiBankNumber bank, int programNumber);

    int toZeroBasedDiscardingBank() const;
    int toOneBasedDiscardingBank() const;
    int toZeroBasedWithBank() const;
    int toOneBasedWithBank() const;

    bool isValid() const;

    bool isBankKnown() const;
    MidiBankNumber bank() const;

    bool operator!=(MidiProgramNumber const& other) const;

private:
    MidiProgramNumber() : programNo_(0), isValid_(false) {}
    MidiProgramNumber(int zeroBasedNumber, MidiBankNumber bank);

    int programNo_;
    bool isValid_;
    MidiBankNumber bank_ = MidiBankNumber::invalid();
};

template <> class juce::VariantConverter<MidiProgramNumber> {
public:
    static MidiProgramNumber fromVar(const var& v);
    static var toVar(const MidiProgramNumber& t);
};

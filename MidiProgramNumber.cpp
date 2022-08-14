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

#include "MidiProgramNumber.h"

MidiProgramNumber MidiProgramNumber::fromOneBase(int programNumber)
{
    return MidiProgramNumber(programNumber - 1, MidiBankNumber::invalid());
}

MidiProgramNumber MidiProgramNumber::fromZeroBase(int programNumber)
{
    return MidiProgramNumber(programNumber, MidiBankNumber::invalid());
}

MidiProgramNumber MidiProgramNumber::fromOneBaseWithBank(MidiBankNumber bank, int programNumber)
{
    return MidiProgramNumber(programNumber - 1, bank);
}

MidiProgramNumber MidiProgramNumber::fromZeroBaseWithBank(MidiBankNumber bank, int programNumber)
{
    if (bank.isValid()) {
        return MidiProgramNumber(programNumber % bank.bankSize(), bank);
    }
    else {
        return fromZeroBase(programNumber);
    }
}

MidiProgramNumber::MidiProgramNumber(int zeroBasedNumber, MidiBankNumber bank) : programNo_(zeroBasedNumber)
{
    bank_ = bank;
    if (bank_.isValid()) {
        if (programNo_ > bank_.bankSize()) {
            jassertfalse;
            // This should better be normalized
            programNo_ = programNo_ % bank_.bankSize();
        }
    }
}

int MidiProgramNumber::toZeroBased() const
{
    return programNo_;
}

int MidiProgramNumber::toOneBased() const
{
    return programNo_ + 1;
}

int MidiProgramNumber::toZeroBasedWithBank() const
{
    if (bank_.isValid()) {
        return programNo_ + bank_.toZeroBased() * bank_.bankSize();
    }
    else {
        return programNo_;
    }
}

int MidiProgramNumber::toOneBasedWithBank() const
{
    return toZeroBasedWithBank() + 1;
}


bool MidiProgramNumber::isBankKnown() const 
{
    return bank_.isValid();
}

MidiBankNumber MidiProgramNumber::bank() const
{
    return bank_;
}

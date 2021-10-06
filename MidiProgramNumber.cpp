/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "MidiProgramNumber.h"

MidiProgramNumber MidiProgramNumber::fromOneBase(int programNumber)
{
	return MidiProgramNumber(programNumber - 1);
}

MidiProgramNumber MidiProgramNumber::fromZeroBase(int programNumber)
{
	return MidiProgramNumber(programNumber);
}

MidiProgramNumber::MidiProgramNumber(int zeroBasedNumber) : programNo_(zeroBasedNumber)
{
}

int MidiProgramNumber::toZeroBased() const
{
	return programNo_;
}

int MidiProgramNumber::toOneBased() const
{
	return programNo_ + 1;
}

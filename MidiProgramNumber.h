/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

class MidiProgramNumber {
public:
	static MidiProgramNumber fromOneBase(int programNumber);
	static MidiProgramNumber fromZeroBase(int programNumber);

	MidiProgramNumber() : programNo_(0) {}

	int toZeroBased() const;
	int toOneBased() const;

private:
	MidiProgramNumber(int zeroBasedNumber);

	int programNo_;
};

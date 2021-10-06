/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "MidiProgramNumber.h"
#include "MidiNote.h"

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

/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "MidiTuning.h"

#include "MidiHelpers.h"

MidiMessage MidiTuning::createTuningDumpRequest(uint8 deviceID, MidiProgramNumber tuningBankNumber)
{	
	// See e.g. http://www.microtonal-synthesis.com/MIDItuning.html
	jassert(tuningBankNumber.toZeroBased() >= 0 && tuningBankNumber.toZeroBased() < 128);
	return MidiHelpers::sysexMessage({ 0x7E, deviceID, 0x08 /* MIDI Tuning Standard */, 0x00 /* Micro tuning request */, (uint8) tuningBankNumber.toZeroBased() });
}

bool MidiTuning::isTuningDump(MidiMessage const &message)
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

bool MidiTuning::fromMidiMessage(MidiMessage const &message, MidiTuning &tuningResult)
{
	if (isTuningDump(message)) {
		auto data = message.getSysExData();
		auto program = MidiProgramNumber::fromZeroBase(data[4]);
		std::string name(data[5], data[5 + 16]);
		const size_t tuningIndex = 5 + 16;
		TTuning tuning;
		for (size_t i = 0; i < 128; i++) {
			auto midiNoteTarget = MidiNote(data[tuningIndex + i * 3]);
			double toneAdjustCents = (100.0 * data[tuningIndex + i * 3 + 1] /* MSB, 1/128 of a semitone */) / 128.0 + (100.0 * data[tuningIndex + i * 3 + 2] /* LSB, 1/16384 of a semitone */) / 16384.0;
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

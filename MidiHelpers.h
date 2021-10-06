/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class MidiHelpers {
public:
	static MidiMessage sysexMessage(std::vector<uint8> const &data);
	static MidiBuffer bufferFromMessages(std::vector<MidiMessage> const &messages);
	static std::vector<MidiMessage> generateRPN(int midiChannel, int parameterNumber, int value, bool isNRPN, bool use14BitValue, bool MSBbeforeLSB);
	static uint8 checksum7bit(std::vector<uint8> const &data);

	static bool isEmptySysex(MidiMessage const &m);
	static MidiBuffer removeEmptySysexMessages(MidiBuffer const &midiBuffer);

	static bool equalSysexMessageContent(MidiMessage const &message1, MidiMessage const &message2, int digitsToCompare = -1);
	static bool isSysexMessageMatching(MidiMessage const &message, std::vector<std::pair<size_t, uint8>> const &indexAndContentCondition);
};

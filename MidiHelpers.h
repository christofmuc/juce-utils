#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MidiHelpers {
public:
	static MidiMessage sysexMessage(std::vector<uint8> const &data);
	static MidiBuffer bufferFromMessages(std::vector<MidiMessage> const &messages);
	static std::vector<MidiMessage> generateRPN(int midiChannel, int parameterNumber, int value, bool isNRPN, bool use14BitValue, bool MSBbeforeLSB);
	static bool equalSysexMessageContent(MidiMessage const &message1, MidiMessage const &message2, int digitsToCompare = -1);
	static uint8 checksum7bit(std::vector<uint8> const &data);
};


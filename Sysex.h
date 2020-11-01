/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class Sysex {
public:
	static std::vector<MidiMessage> loadSysex(std::string const &filename);
	static std::vector<MidiMessage> loadSysex(InputStream &inputStream);
	static std::vector<MidiMessage> vectorToMessages(std::vector<uint8> const &midiData);
	static std::vector<MidiMessage> memoryBlockToMessages(MemoryBlock const &midiData);
	static void saveSysex(std::string const &filename, std::vector<juce::MidiMessage> const &messages);
	static std::string saveSysexIntoNewFile(std::string const &directory, std::string const &desiredFileName, std::vector<juce::MidiMessage> const &messages);
};


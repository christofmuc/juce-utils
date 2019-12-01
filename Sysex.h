#pragma once

#include "JuceHeader.h"


class Sysex {
public:
	static std::vector<MidiMessage> loadSysex(std::string const &filename);
	static std::vector<MidiMessage> loadSysex(InputStream &inputStream);
	static void saveSysex(std::string const &filename, std::vector<juce::MidiMessage> const &messages);
	static std::string saveSysexIntoNewFile(std::string const &directory, std::string const &desiredFileName, std::vector<juce::MidiMessage> const &messages);
};

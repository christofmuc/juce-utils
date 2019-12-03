/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include <string>

struct MidiNetworkLocation {

	MidiNetworkLocation() {}

	MidiNetworkLocation(std::string const &inputName, std::string const &outputName, int midiChannel) :
		valid(true), inputName(inputName), outputName(outputName), midiChannel(midiChannel) {}

	bool valid = false;
	std::string inputName;
	std::string outputName;
	int midiChannel;
};


/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include <string>

#include "MidiChannel.h"

struct MidiNetworkLocation {

	MidiNetworkLocation() : midiChannel(MidiChannel::invalidChannel()) {}

	MidiNetworkLocation(std::string const &inputName, std::string const &outputName, MidiChannel midiChannel) :
		valid(true), inputName(inputName), outputName(outputName), midiChannel(midiChannel) {}

	bool valid = false;
	std::string inputName;
	std::string outputName;
	MidiChannel midiChannel;
};


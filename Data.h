/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class Data {
public:
	static Data &instance();

	static const var& getProperty(const Identifier& name); // Convenience function for instance().get().getProperty()
	static const var& getEphemeralProperty(const Identifier& name);
	static Value getPropertyAsValue(const Identifier& name); // Convenience function for instance().get().getPropertyAsValue()
	static Value getEphemeralPropertyAsValue(const Identifier& name);

	ValueTree& get();
	ValueTree& getEphemeral();

	void initializeFromSettings();
	void saveToSettings();

private:
	Data();

	static Data instance_;

	ValueTree tree_;
	ValueTree ephemeralTree_;
};

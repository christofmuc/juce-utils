/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class DebounceTimer : public Timer {
public:
	void callDebounced(std::function<void()> action, int milliseconds);

	virtual void timerCallback() override;

private:
	std::function<void()> action_;
};

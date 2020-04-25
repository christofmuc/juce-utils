/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class WaitForEvent : public Thread {
public:
	WaitForEvent(std::function<bool()> hasHappened);
	WaitForEvent(std::function<bool()> hasHappened, Thread *threadToBeNotified);

	virtual void run() override;

private:
	std::function<bool()> hasHappened_;
	Thread *notify_;
};

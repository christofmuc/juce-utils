/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "DebounceTimer.h"

void DebounceTimer::callDebounced(std::function<void()> action, int milliseconds)
{
	stopTimer();
	action_ = action;
	startTimer(milliseconds);
}

void DebounceTimer::timerCallback()
{
	// If we reached this, we can execute the function and stop the timer!
	stopTimer();
	action_();
}

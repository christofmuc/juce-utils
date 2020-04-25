/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "WaitForEvent.h"

WaitForEvent::WaitForEvent(std::function<bool()> hasHappened, Thread *threadToBeNotified) : Thread("WaitForEvent"), hasHappened_(hasHappened), notify_(threadToBeNotified)
{
}

WaitForEvent::WaitForEvent(std::function<bool()> hasHappened) : Thread("WaitForEvent"), hasHappened_(hasHappened), notify_(nullptr)
{
}

void WaitForEvent::run()
{
	while (!threadShouldExit()) {
		sleep(100);
		if (hasHappened_()) {
			if (notify_) {
				notify_->notify();
				return;
			}
		}
	}
}

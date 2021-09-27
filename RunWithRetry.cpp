/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "RunWithRetry.h"

#include "Logger.h"

void RunWithRetry::start(std::function<void()> action, std::function<bool()> retryRequired, int numRetries, int retryIntervalMS, std::string const &message)
{
	retryObjects_.push_back(new RunWithRetry(action, retryRequired, numRetries, message));
	action();
	retryObjects_.back()->startTimer(retryIntervalMS);
}

RunWithRetry::RunWithRetry(std::function<void()> action, std::function<bool()> retryRequired, int numRetries, std::string const &message) : action_(action), retryRequiredPredicate_(retryRequired), numRetries_(numRetries), message_(message)
{
	retries_ = 0;
}

void RunWithRetry::timerCallback()
{
	if (retryRequiredPredicate_()) {
		retries_++;
		if (retries_ <= numRetries_) {
			action_();
		}
		else {
			SimpleLogger::instance()->postMessage("Giving up retrying " + message_);
			stopTimer();
			//TODO this needs some cleanup logic to remove the stopped timer from memory
		}
	}
	else {
		stopTimer();
		//TODO this needs some cleanup logic to remove the stopped timer from memory
	}
}

std::vector<RunWithRetry *> RunWithRetry::retryObjects_;

/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class RunWithRetry : public Timer {
public:
	static void start(std::function<void()> action, std::function<bool()> retryRequired, int numRetries, int retryIntervalMS, std::string const &message);

private:
	RunWithRetry(std::function<void()> action, std::function<bool()> retryRequired, int numRetries, std::string const &message);

	void timerCallback() override;

	static std::vector<RunWithRetry *> retryObjects_;

	std::function<void()> action_;
	std::function<bool()> retryRequiredPredicate_;
	int numRetries_;
	std::string message_;
	int retries_;
};

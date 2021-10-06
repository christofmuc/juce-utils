/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Christof Ruch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

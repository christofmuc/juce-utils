/*
 * MIT License
 *
 * Copyright (c) 2019-2025 Christof Ruch
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

#include <spdlog/spdlog.h>


// Static member definitions
std::vector<std::shared_ptr<RunWithRetry>> RunWithRetry::retryObjects_;
juce::CriticalSection RunWithRetry::retryLock_;

void RunWithRetry::start(std::function<void()> action, std::function<bool()> retryRequired, std::function<bool()> stopCondition, int numRetries, int retryIntervalMS, const std::string& message)
{
    auto instance = std::shared_ptr<RunWithRetry>(new RunWithRetry(action, retryRequired, stopCondition, numRetries, retryIntervalMS, message));

    {
        juce::ScopedLock lock(retryLock_);
        retryObjects_.push_back(instance);
    }

    instance->attemptAction();
}

RunWithRetry::RunWithRetry(std::function<void()> action, std::function<bool()> retryRequired, std::function<bool()> stopCondition, int numRetries, int retryIntervalMS, const std::string& message) :
    action_(std::move(action)), retryRequiredPredicate_(std::move(retryRequired)), stopCondition_(std::move(stopCondition)), numRetries_(numRetries), retryIntervalMS_(retryIntervalMS),
    message_(message), retries_(0)
{
}

void RunWithRetry::attemptAction()
{
    if (stopCondition_()) {
        // Remove self from the retry list and delete
        removeFromList();
        return;
    }

    if (!retryRequiredPredicate_()) {
        // No retry needed yet, schedule another check
        juce::Timer::callAfterDelay(retryIntervalMS_, [self = shared_from_this()] { self->attemptAction(); });
        return;
    }

    if (retries_ < numRetries_) {
        retries_++;
        action_();
        // Schedule next retry attempt
        juce::Timer::callAfterDelay(retryIntervalMS_, [self = shared_from_this()] { self->attemptAction(); });
    }
    else {
        // Stop retrying and remove from list
        removeFromList();
    }
}

void RunWithRetry::removeFromList()
{
    juce::ScopedLock lock(retryLock_);
    retryObjects_.erase(std::remove_if(retryObjects_.begin(), retryObjects_.end(), [this](const std::shared_ptr<RunWithRetry>& obj) { return obj.get() == this; }), retryObjects_.end());
}

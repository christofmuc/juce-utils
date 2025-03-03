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

#pragma once

#include <juce_events/juce_events.h>
#include <memory>
#include <vector>

class RunWithRetry : public std::enable_shared_from_this<RunWithRetry> {

public:
    static void start(std::function<void()> action, std::function<bool()> retryRequired, std::function<bool()> stopCondition, int numRetries, int retryIntervalMS, const std::string& message);

private:
    RunWithRetry(std::function<void()> action, std::function<bool()> retryRequired, std::function<bool()> stopCondition, int numRetries, int retryIntervalMS, const std::string& message);
    void removeFromList();

    void attemptAction();

    static std::vector<std::shared_ptr<RunWithRetry>> retryObjects_;
    static juce::CriticalSection retryLock_;

    std::function<void()> action_;
    std::function<bool()> retryRequiredPredicate_;
    std::function<bool()> stopCondition_;
    int numRetries_;
    int retryIntervalMS_;
    std::string message_;
    int retries_;
};

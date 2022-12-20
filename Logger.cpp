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

#include "Logger.h"

#include <juce_events/juce_events.h>

class BootstrapLogger : public SimpleLogger {
public:
    virtual void postMessage(const juce::String& message) override;
    virtual void postMessageOncePerRun(const juce::String& message) override;

    std::vector<juce::String> getLogMessagesDuringBoot();

private:
    std::vector<juce::String> logMessagesDuringBoot_;
};

SimpleLogger::SimpleLogger()
{
    if (instance_ != nullptr) {
        // Replace the BootstrapLogger with the real logger
        BootstrapLogger* bootstrap = dynamic_cast<BootstrapLogger*>(instance_);
        if (bootstrap == nullptr) {
            throw new std::runtime_error("This is a singleton, can't create twice");
        }

        // Don't do this immediately, but rather when the message queue runs!
        juce::MessageManager::callAsync([this, bootstrap]() {
            for (auto const& text : bootstrap->getLogMessagesDuringBoot()) {
                postMessage(text);
            }
            delete bootstrap;
        });
    }
    instance_ = this;
    juce::Logger::setCurrentLogger(instance_);
    hasBeenShutdown_ = false;

    // TODO this code doesn't belong here...
    /*fileLogger_.reset(FileLogger::createDefaultAppLogger("knobkraft", "knobkraft.log", "Starting KnobKraft Version unknown"));
    FileLogger::setCurrentLogger(fileLogger_.get());*/
}

SimpleLogger::~SimpleLogger()
{
}

SimpleLogger* SimpleLogger::instance()
{
    if (!hasBeenShutdown_) {
        return instance_;
    }
    else {
        return nullptr;
    }
}

void SimpleLogger::shutdown()
{
    Logger::setCurrentLogger(nullptr);
    hasBeenShutdown_ = true; // To prevent logging during destruction
}

void SimpleLogger::markBenchmarkPoint(const juce::String& message)
{
    juce::Time now = juce::Time::getCurrentTime();
    std::stringstream result;
    result << "Benchmark at " << now.toISO8601(true) << ": " << message;
    postMessage(result.str());
}

void SimpleLogger::writeToFile(const juce::String& message)
{
    fileLogger_->writeToLog(message);
}

SimpleLogger* SimpleLogger::instance_ = new BootstrapLogger();
bool SimpleLogger::hasBeenShutdown_ = false;

void SimpleLogger::logMessage(const juce::String& message)
{
    postMessage(message);
}

void BootstrapLogger::postMessage(const juce::String& message)
{
    logMessagesDuringBoot_.push_back(message);
}

void BootstrapLogger::postMessageOncePerRun(const juce::String& message)
{
    //jassertfalse;
    logMessagesDuringBoot_.push_back(message);
}

std::vector<juce::String> BootstrapLogger::getLogMessagesDuringBoot()
{
    return logMessagesDuringBoot_;
}

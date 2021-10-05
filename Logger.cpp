/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "Logger.h"

class BootstrapLogger : public SimpleLogger {
public:
	virtual void postMessage(const String& message) override;
	virtual void postMessageOncePerRun(const String& message) override;

	std::vector<String> getLogMessagesDuringBoot();

private:
	std::vector<String> logMessagesDuringBoot_;
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
		MessageManager::callAsync([this, bootstrap]() {
			for (auto const& text : bootstrap->getLogMessagesDuringBoot()) {
				postMessage(text);
			}
			delete bootstrap;
		});
	}
	instance_ = this;
	juce::Logger::setCurrentLogger(instance_);
	hasBeenShutdown_ = false;

	//TODO this code doesn't belong here...
	/*fileLogger_.reset(FileLogger::createDefaultAppLogger("knobkraft", "knobkraft.log", "Starting KnobKraft Version unknown"));
	FileLogger::setCurrentLogger(fileLogger_.get());*/
}

SimpleLogger::~SimpleLogger()
{
}

SimpleLogger * SimpleLogger::instance()
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

void SimpleLogger::markBenchmarkPoint(const String &message)
{
	Time now = Time::getCurrentTime();
	std::stringstream result;
	result << "Benchmark at " << now.toISO8601(true) << ": " << message;
	postMessage(result.str());
}

void SimpleLogger::writeToFile(const String &message) 
{
	fileLogger_->writeToLog(message);
}

SimpleLogger * SimpleLogger::instance_ = new BootstrapLogger();
bool SimpleLogger::hasBeenShutdown_ = false;

void SimpleLogger::logMessage(const String& message)
{
	postMessage(message);
}

void BootstrapLogger::postMessage(const String& message)
{
	logMessagesDuringBoot_.push_back(message);
}

void BootstrapLogger::postMessageOncePerRun(const String& message)
{
	jassertfalse;
	logMessagesDuringBoot_.push_back(message);
}

std::vector<String> BootstrapLogger::getLogMessagesDuringBoot()
{
	return logMessagesDuringBoot_;
}

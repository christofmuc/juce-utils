/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "Logger.h"

SimpleLogger::SimpleLogger()
{
	if (instance_ != nullptr) {
		throw new std::runtime_error("This is a singleton, can't create twice");
	}
	instance_ = this;
	hasBeenShutdown_ = false;

	fileLogger_.reset(FileLogger::createDefaultAppLogger("knobkraft", "knobkraft.log", "Starting KnobKraft Version unknown"));
	FileLogger::setCurrentLogger(fileLogger_.get());
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

SimpleLogger * SimpleLogger::instance_ = nullptr;
bool SimpleLogger::hasBeenShutdown_ = false;

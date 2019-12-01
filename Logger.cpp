#include "Logger.h"

SimpleLogger::SimpleLogger()
{
	if (instance_ != nullptr) {
		throw new std::runtime_error("This is a singleton, can't create twice");
	}
	instance_ = this;

	fileLogger_.reset(FileLogger::createDefaultAppLogger("knobkraft", "knobkraft.log", "Starting KnobKraft Version unknown"));
	FileLogger::setCurrentLogger(fileLogger_.get());
}

SimpleLogger * SimpleLogger::instance()
{
	return instance_;
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

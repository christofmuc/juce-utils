#pragma once

#include "JuceHeader.h"

class SimpleLogger {
public:
	SimpleLogger();

	static SimpleLogger *instance();

	virtual void postMessage(const String& message) = 0;
	virtual void markBenchmarkPoint(const String &message);
	
	void writeToFile(const String &message);

private:
	static SimpleLogger *instance_;
	std::unique_ptr<Logger> fileLogger_;
};


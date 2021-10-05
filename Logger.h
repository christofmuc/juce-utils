/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class SimpleLogger : private Logger {
public:
	SimpleLogger();
	virtual ~SimpleLogger();

	static SimpleLogger *instance();
	static void shutdown();

	virtual void postMessage(const String& message) = 0;
	virtual void postMessageOncePerRun(const String& message) = 0;
	virtual void markBenchmarkPoint(const String &message);
	
	void writeToFile(const String &message);

protected:
	void logMessage(const String& message) override;

private:
	static SimpleLogger *instance_;
	std::unique_ptr<Logger> fileLogger_;
	static bool hasBeenShutdown_;

};


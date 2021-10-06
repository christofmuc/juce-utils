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
#pragma once

#include "JuceHeader.h"

class Settings {
public:
	static void setSettingsID(String const &id); // Call this before any call to instance
	static Settings & instance();
	static void shutdown();
	void saveAndClose();
	void flush();

	std::string get(std::string const &key, std::string const &defaultValue = std::string());
	void set(std::string const &key, std::string const &value);
	bool keyIsSet(std::string const &key);

	File getSessionStorageDir() const;

	// Singleton!
	Settings(Settings const &) = delete;
	void operator=(Settings const&) = delete;

	// Can't be private because of unique_ptr
	Settings();

private:
	static std::unique_ptr<Settings> instance_;
	static String settingsID_;
	ApplicationProperties properties_;
};

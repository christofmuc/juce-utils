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

class StreamLogger {
public:
	// See https://stackoverflow.com/questions/1134388/stdendl-is-of-unknown-type-when-overloading-operator
	typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
	typedef CoutType& (*StandardEndLine)(CoutType&);

	// Singleton implementation
	static StreamLogger &instance();
	~StreamLogger();

	StreamLogger &operator <<(const char *string);
	StreamLogger &operator <<(std::string const &string);
	StreamLogger &operator <<(juce::String const &string);
	StreamLogger &operator <<(StandardEndLine manipulator);
	StreamLogger &operator <<(int value);
	StreamLogger &operator <<(uint64 value);
	StreamLogger &operator <<(double value);

	void flushBuffer(bool force = false);

	StreamLogger(StreamLogger const &) = delete;
	void operator=(StreamLogger const&) = delete;

private:
	StreamLogger() {}

	std::stringstream buffer;
};

/*
 * MIT License
 *
 * Copyright (c) 2019-2023 Christof Ruch
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
#include "StreamLogger.h"

StreamLogger::~StreamLogger()
{
}

void StreamLogger::flushBuffer(bool force /* = false */)
{
    std::string endl = "\n";
    std::string toEmit = buffer.str();
    // If the whole string ends on a newline, just write it. Else, we must split it and call writeToLog twice
    if (force || (toEmit.size() > 0 && toEmit.at(toEmit.length() - 1) == '\n')) {
        juce::Logger::getCurrentLogger()->writeToLog(toEmit);
        buffer.str(std::string());
    }
    else if (toEmit.find(endl) != std::string::npos) {
        // TODO - this is wrong and slow. What I really want is to reverse search for the endl, and send this off
        size_t start = 0;
        size_t end;
        while ((end = toEmit.find(endl, start)) != std::string::npos) {
            juce::Logger::getCurrentLogger()->writeToLog(toEmit.substr(start, end - start));
            start = end + endl.length();
        }
        buffer.str(std::string());
    }
}

StreamLogger &StreamLogger::instance()
{
    static StreamLogger instance_;
    return instance_;
}

StreamLogger &StreamLogger::operator<<(double value)
{
    buffer << value;
    return *this;
}

StreamLogger &StreamLogger::operator<<(int value)
{
    buffer << std::fixed << std::setprecision(2) << value;
    return *this;
}

StreamLogger &StreamLogger::operator<<(const char *string)
{
    buffer << string;
    flushBuffer();
    return *this;
}

StreamLogger &StreamLogger::operator<<(juce::String const &string)
{
    buffer << string;
    flushBuffer();
    return *this;
}

StreamLogger &StreamLogger::operator<<(std::string const &string)
{
    buffer << string;
    flushBuffer();
    return *this;
}

StreamLogger &StreamLogger::operator<<(StandardEndLine manipulator)
{
    buffer << manipulator;
    flushBuffer();
    return *this;
}

StreamLogger &StreamLogger::operator<<(uint64_t value)
{
    buffer << value;
    flushBuffer();
    return *this;
}

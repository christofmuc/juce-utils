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

#pragma once

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

class Data {
public:
    static Data& instance();

    static const juce::var getProperty(const juce::Identifier& name); // Convenience function for instance().get().getProperty()
    static const juce::var getEphemeralProperty(const juce::Identifier& name);
    static const juce::var getRuntimeProperty(const juce::Identifier& name);
    static juce::Value getPropertyAsValue(const juce::Identifier& name); // Convenience function for instance().get().getPropertyAsValue()
    static juce::Value getEphemeralPropertyAsValue(const juce::Identifier& name);
    static juce::Value getRuntimePropertyAsValue(const juce::Identifier& name);
    static void ensurePropertyExists(const juce::Identifier& name, juce::var defaultValue);
    static void ensureEphemeralPropertyExists(const juce::Identifier& name, juce::var defaultValue);
    static void ensureRuntimePropertyExists(const juce::Identifier& name, juce::var defaultValue);
    static void reset();

    juce::ValueTree& get();
    juce::ValueTree& getEphemeral();
    juce::ValueTree& getRuntime();

    void initializeFromSettings();
    void saveToSettings();

private:
    Data();

    static Data instance_;

    std::unique_ptr<juce::ValueTree> tree_;
    std::unique_ptr<juce::ValueTree> ephemeralTree_;
    std::unique_ptr<juce::ValueTree> runtimeTree_;
};

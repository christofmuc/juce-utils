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

#include <juce_data_structures/juce_data_structures.h>

class LambdaValueListener : private juce::Value::Listener {
public:
    LambdaValueListener(juce::Value const& value, std::function<void(juce::Value& newValue)> listener) : listener_(listener)
    {
        value_.referTo(value);
        value_.addListener(this);
    }

    void trigger() { listener_(value_); }

private:
    virtual void valueChanged(juce::Value& value) override
    {
        if (listener_) {
            listener_(value);
        }
    }

    juce::Value value_;
    std::function<void(juce::Value& newValue)> listener_;
};

class ListenerSet {
public:
    void addListener(juce::Value const& value, std::function<void(juce::Value& newValue)> listener) { listeners_.emplace_back(std::make_unique<LambdaValueListener>(value, listener)); }

    void clear() { listeners_.clear(); }
    void triggerAll()
    {
        for (auto& l : listeners_) {
            l->trigger();
        }
    }

private:
    std::vector<std::unique_ptr<LambdaValueListener>> listeners_;
};

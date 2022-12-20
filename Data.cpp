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

#include "Data.h"

#include "Settings.h"


Data Data::instance_;

Data& Data::instance()
{
    return instance_;
}

const juce::var Data::getProperty(const juce::Identifier& name)
{
    return instance().get().getProperty(name);
}

const juce::var Data::getEphemeralProperty(const juce::Identifier& name)
{
    return instance().getEphemeral().getProperty(name);
}

const juce::var Data::getRuntimeProperty(const juce::Identifier& name)
{
    return instance().getRuntime().getProperty(name);
}

juce::Value Data::getPropertyAsValue(const juce::Identifier& name)
{
    return instance().get().getPropertyAsValue(name, nullptr);
}

juce::Value Data::getEphemeralPropertyAsValue(const juce::Identifier& name)
{
    return instance().getEphemeral().getPropertyAsValue(name, nullptr);
}

juce::Value Data::getRuntimePropertyAsValue(const juce::Identifier& name)
{
    return instance().getRuntime().getPropertyAsValue(name, nullptr);
}

void Data::ensurePropertyExists(const juce::Identifier& name, juce::var defaultValue)
{
    if (!instance_.get().hasProperty(name)) {
        instance_.get().setProperty(name, defaultValue, nullptr);
    }
}

void Data::ensureEphemeralPropertyExists(const juce::Identifier& name, juce::var defaultValue)
{
    if (!instance_.getEphemeral().hasProperty(name)) {
        instance_.getEphemeral().setProperty(name, defaultValue, nullptr);
    }
}

void Data::ensureRuntimePropertyExists(const juce::Identifier& name, juce::var defaultValue)
{
    if (!instance_.getRuntime().hasProperty(name)) {
        instance_.getRuntime().setProperty(name, defaultValue, nullptr);
    }
}

void Data::reset()
{
    instance_.tree_ = std::make_unique<juce::ValueTree>(juce::Identifier("Setup"));
    instance_.ephemeralTree_ = std::make_unique<juce::ValueTree>(juce::Identifier("Ephemeral"));
}

Data::Data() 
    :
    tree_ { std::make_unique<juce::ValueTree>(juce::Identifier("Setup")) }, ephemeralTree_ { std::make_unique<juce::ValueTree>(juce::Identifier("Ephemeral")) }, runtimeTree_ {
        std::make_unique<juce::ValueTree>(juce::Identifier("EphemeralSurvivingReset"))
    }
{}

juce::ValueTree& Data::get()
{
    return *tree_;
}

juce::ValueTree& Data::getEphemeral()
{
    return *ephemeralTree_;
}

juce::ValueTree& Data::getRuntime()
{
    return *runtimeTree_;
}

void Data::initializeFromSettings()
{
    auto xmlDoc = Settings::instance().get("ClientSettings", "");
    auto topLevel = juce::parseXML(xmlDoc);
    if (topLevel) {
        *tree_ = juce::ValueTree::fromXml(*topLevel);
    }
}

void Data::saveToSettings()
{
    Settings::instance().set("ClientSettings", tree_->toXmlString().toStdString());
}

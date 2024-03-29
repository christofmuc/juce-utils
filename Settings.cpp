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
#include "Settings.h"

#include "StreamLogger.h"

juce::String Settings::settingsID_ = "JammerNetz"; // This can be overridden for testing, so you can start more than one client on a single machine and don't overwrite your settings file

std::unique_ptr<Settings> Settings::instance_;

Settings &Settings::instance()
{
    if (!instance_) {
        instance_ = std::make_unique<Settings>();
    }
    return *instance_;
}

void Settings::shutdown()
{
    instance_.reset();
}

void Settings::setSettingsID(juce::String const &id)
{
    settingsID_ = id;
}

std::string Settings::get(std::string const &key, std::string const &defaultValue)
{
    return properties_.getUserSettings()->getValue(key, defaultValue).toStdString();
}

int Settings::get(std::string const &key, int defaultValue)
{
    auto int_as_text = properties_.getUserSettings()->getValue(key, juce::String(defaultValue)).toStdString();
    return atoi(int_as_text.c_str());
}

void Settings::set(std::string const &key, std::string const &value)
{
    properties_.getUserSettings()->setValue(key, juce::String(value));
    properties_.getUserSettings()->setNeedsToBeSaved(true);
}

bool Settings::keyIsSet(std::string const &key)
{
    return properties_.getUserSettings()->containsKey(key);
}

juce::File Settings::getSessionStorageDir() const
{
    auto dir = juce::File(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getFullPathName() + "/JammerNetzSession");
    if (!dir.exists()) {
        dir.createDirectory();
    }
    else {
        if (dir.existsAsFile()) {
            jassert(false);
            StreamLogger::instance() << "Can't record to " << dir.getFullPathName() << " as it is a file and not a directory!" << std::endl;
            return juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getFullPathName();
        }
    }
    return dir;
}

juce::File const &Settings::getPropertiesFile() noexcept
{
    return properties_.getUserSettings()->getFile();
}

Settings::Settings()
{
    juce::PropertiesFile::Options options;
    options.folderName = settingsID_;
    options.applicationName = settingsID_;
    options.filenameSuffix = ".settings";
    options.commonToAllUsers = false;
    options.osxLibrarySubFolder = "Application Support"; // Required to avoid an assert in JUCE
    properties_.setStorageParameters(options);
}

void Settings::saveAndClose()
{
    properties_.closeFiles();
}

void Settings::flush()
{
    properties_.saveIfNeeded();
}

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

#include "FileHelpers.h"

juce::File getOrCreateSubdirectory(juce::File dir, juce::String const &name)
{
    juce::File subdir = dir.getChildFile(name);
    if (!subdir.exists()) {
        subdir.createDirectory();
    }
    return subdir;
}

int FileDateComparatorOldestFirst::compareElements(juce::File const &first, juce::File const &second)
{
    if (first.getLastModificationTime() < second.getLastModificationTime()) return -1;
    if (first.getLastModificationTime() > second.getLastModificationTime()) return 1;
    return 0;
}

int FileDateComparatorNewestFirst::compareElements(juce::File const &first, juce::File const &second)
{
    if (first.getLastModificationTime() < second.getLastModificationTime()) return 1;
    if (first.getLastModificationTime() > second.getLastModificationTime()) return -1;
    return 0;
}

TemporaryDirectory::TemporaryDirectory(juce::String prefix, juce::String suffix)
{
    juce::File tempDir = juce::File::getSpecialLocation(juce::File::tempDirectory);
    dir_ = tempDir.getNonexistentChildFile(prefix, suffix);
    dir_.createDirectory();
}

TemporaryDirectory::~TemporaryDirectory()
{
    dir_.deleteRecursively(false);
}

juce::File TemporaryDirectory::asFile()
{
    return dir_;
}

std::string TemporaryDirectory::name()
{
    return dir_.getFullPathName().toStdString();
}

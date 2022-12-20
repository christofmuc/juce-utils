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

#include "Sysex.h"

#include "Logger.h"

std::vector<juce::MidiMessage> Sysex::loadSysex(std::string const &filename)
{
    std::vector<juce::MidiMessage> messages;

    juce::File sysexFile = juce::File::createFileWithoutCheckingPath(filename);
    if (sysexFile.existsAsFile()) {
        // This could be a ZIP file?
        if (sysexFile.getFileExtension().toLowerCase() == ".zip") {
            juce::ZipFile zip(sysexFile);
            for (int i = 0; i < zip.getNumEntries(); i++) {
                auto entry = zip.getEntry(i);
                juce::File zipEntry = juce::File::createFileWithoutCheckingPath(entry->filename);
                if (zipEntry.getFileExtension().toLowerCase() == ".mid" || zipEntry.getFileExtension().toLowerCase() == ".syx") {
                    // That's an interesting entry
                    SimpleLogger::instance()->postMessage(juce::String("Opening ") + juce::String(entry->filename));
                    auto zipStream = zip.createStreamForEntry(i);
                    if (zipStream) {
                        auto newMessages = loadSysex(*zipStream);
                        std::copy(newMessages.cbegin(), newMessages.cend(), std::back_inserter(messages));
                        delete zipStream;
                    }
                }
            }
        }
        else if (sysexFile.getFileExtension().toLowerCase() == ".txt") {
            // Let's assume this is one of the funny files which contains the sysex hex dump as text - I found some with MKS80 patches that looked like that!
            juce::FileInputStream inputStream(sysexFile);
            auto content = inputStream.readEntireStreamAsString();
            juce::MemoryBlock memory;
            memory.loadFromHexString(content);
            juce::MemoryInputStream sysexStream(memory, false);
            return loadSysex(sysexStream);
        }
        else {
            // Single file
            juce::FileInputStream inputStream(sysexFile);
            return loadSysex(inputStream);
        }
    }

    return messages;
}

std::vector<juce::MidiMessage> Sysex::loadSysex(juce::InputStream &inputStream)
{
    std::vector<juce::MidiMessage> messages;

    // It could be SMF (Standard Midi File). Then there is a bit more structure we need to parse
    juce::MidiFile smfFile;
    if (smfFile.readFrom(inputStream, false)) {
        // Loop over all tracks
        for (int track = 0; track < smfFile.getNumTracks(); track++) {
            auto t = smfFile.getTrack(track);
            juce::MidiMessageSequence::MidiEventHolder *const *currentMsg;
            for (currentMsg = t->begin(); currentMsg != t->end(); currentMsg++) {
                // If this is a sysex message, keep it
                if ((*currentMsg)->message.isSysEx()) {
                    messages.push_back((*currentMsg)->message);
                }
            }
        }
    }
    else {
        // More likely, this is a pure sysex file where we read the raw messages from a binary stream
        inputStream.setPosition(0);
        std::vector<juce::uint8> data((size_t) inputStream.getTotalLength());
        inputStream.read(&data[0], (int) inputStream.getTotalLength()); // 4 GB Limit
        return vectorToMessages(data);
    }
    return messages;
}

std::vector<juce::MidiMessage> Sysex::vectorToMessages(std::vector<juce::uint8> const &midiData)
{
    std::vector<juce::MidiMessage> messages;

    // Now produce the Sysex messages from the file
    size_t inPointer = 0;
    juce::uint8 lastStatusByte = 0xf0; // Sysex message
    while (inPointer < midiData.size()) {
        int bytesUsed = 0;
        // TODO - this crashes in case the data is not well formed sysex (example: Depeche Mode TI sound set, there is a message which ends on 0xff 0xff).
        messages.push_back(juce::MidiMessage(&midiData[inPointer], (int) (midiData.size() - inPointer), bytesUsed, lastStatusByte, 0.0, false));
        inPointer += (size_t) bytesUsed;
    }
    return messages;
}

std::vector<juce::MidiMessage> Sysex::memoryBlockToMessages(juce::MemoryBlock const &midiData)
{
    std::vector<juce::MidiMessage> messages;
    size_t inPointer = 0;
    juce::uint8 lastStatusByte = 0xf0; // Sysex message
    while (inPointer < midiData.getSize()) {
        int bytesUsed = 0;
        // TODO - this crashes in case the data is not well formed sysex (example: Depeche Mode TI sound set, there is a message which ends on 0xff 0xff).
        messages.push_back(juce::MidiMessage(&midiData[inPointer], (int) (midiData.getSize() - inPointer), bytesUsed, lastStatusByte, 0.0, false));
        inPointer += (size_t) bytesUsed;
    }
    return messages;
}

void Sysex::saveSysex(std::string const &filename, std::vector<juce::MidiMessage> const &messages)
{
    juce::File sysExFile(filename);
    if (sysExFile.existsAsFile()) {
        sysExFile.deleteFile();
    }

    juce::File sysexFile = juce::File::createFileWithoutCheckingPath(filename);

    juce::FileOutputStream outputStream(sysexFile);
    if (sysexFile.existsAsFile()) {
        for (auto message : messages) {
            outputStream.write(message.getRawData(), (size_t) message.getRawDataSize());
        }
    }
}

std::string Sysex::saveSysexIntoNewFile(std::string const &directory, std::string const &desiredFileName, std::vector<juce::MidiMessage> const &messages)
{
    juce::File dir(directory);
    if (dir.isDirectory()) {
        juce::File newFile = dir.getNonexistentChildFile(desiredFileName, ".syx", false);
        saveSysex(newFile.getFullPathName().toStdString(), messages);
        return newFile.getFullPathName().toStdString();
    }
    jassert(false);
    return "Failure";
}

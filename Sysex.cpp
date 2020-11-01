/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "Sysex.h"

#include "Logger.h"

std::vector<MidiMessage> Sysex::loadSysex(std::string const &filename)
{
	std::vector<MidiMessage> messages;

	File sysexFile = File::createFileWithoutCheckingPath(filename);
	if (sysexFile.existsAsFile()) {
		// This could be a ZIP file?
		if (sysexFile.getFileExtension().toLowerCase() == ".zip") {
			ZipFile zip(sysexFile);
			for (int i = 0; i < zip.getNumEntries(); i++) {
				auto entry = zip.getEntry(i);
				File zipEntry = File::createFileWithoutCheckingPath(entry->filename);
				if (zipEntry.getFileExtension().toLowerCase() == ".mid" || zipEntry.getFileExtension().toLowerCase() == ".syx") {
					// That's an interesting entry
					SimpleLogger::instance()->postMessage(String("Opening ") + String(entry->filename));
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
			FileInputStream inputStream(sysexFile);
			auto content = inputStream.readEntireStreamAsString();
			MemoryBlock memory;
			memory.loadFromHexString(content);
			MemoryInputStream sysexStream(memory, false);
			return loadSysex(sysexStream);
		}
		else {
			// Single file
			FileInputStream inputStream(sysexFile);
			return loadSysex(inputStream);
		}
	}

	return messages;
}

std::vector<juce::MidiMessage> Sysex::loadSysex(InputStream &inputStream)
{
	std::vector<MidiMessage> messages;

	// It could be SMF (Standard Midi File). Then there is a bit more structure we need to parse
	MidiFile smfFile;
	if (smfFile.readFrom(inputStream, false)) {
		// Loop over all tracks
		for (int track = 0; track < smfFile.getNumTracks(); track++) {
			auto t = smfFile.getTrack(track);
			MidiMessageSequence::MidiEventHolder* const* currentMsg;
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
		std::vector<uint8> data((size_t) inputStream.getTotalLength());
		inputStream.read(&data[0], (int)inputStream.getTotalLength()); // 4 GB Limit
		return vectorToMessages(data);
	}
	return messages;
}

std::vector<juce::MidiMessage> Sysex::vectorToMessages(std::vector<uint8> const &midiData)
{
	std::vector<MidiMessage> messages;

	// Now produce the Sysex messages from the file
	size_t inPointer = 0;
	uint8 lastStatusByte = 0xf0; // Sysex message
	while (inPointer < midiData.size()) {
		int bytesUsed = 0;
		//TODO - this crashes in case the data is not well formed sysex (example: Depeche Mode TI sound set, there is a message which ends on 0xff 0xff). 
		messages.push_back(MidiMessage(&midiData[inPointer], (int)(midiData.size() - inPointer), bytesUsed, lastStatusByte, 0.0, false));
		inPointer += bytesUsed;
	}
	return messages;
}

std::vector<juce::MidiMessage> Sysex::memoryBlockToMessages(MemoryBlock const &midiData)
{
	std::vector<MidiMessage> messages;
	size_t inPointer = 0;
	uint8 lastStatusByte = 0xf0; // Sysex message
	while (inPointer < midiData.getSize()) {
		int bytesUsed = 0;
		//TODO - this crashes in case the data is not well formed sysex (example: Depeche Mode TI sound set, there is a message which ends on 0xff 0xff). 
		messages.push_back(MidiMessage(&midiData[inPointer], (int)(midiData.getSize() - inPointer), bytesUsed, lastStatusByte, 0.0, false));
		inPointer += bytesUsed;
	}
	return messages;
}

void Sysex::saveSysex(std::string const &filename, std::vector<juce::MidiMessage> const &messages) {
	File sysExFile(filename);
	if (sysExFile.existsAsFile()) {
		sysExFile.deleteFile();
	}

	File sysexFile = File::createFileWithoutCheckingPath(filename);

	FileOutputStream outputStream(sysexFile);
	if (sysexFile.existsAsFile()) {
		for (auto message : messages) {
			outputStream.write(message.getRawData(), message.getRawDataSize());
		}
	}
}

std::string Sysex::saveSysexIntoNewFile(std::string const &directory, std::string const &desiredFileName, std::vector<juce::MidiMessage> const &messages)
{
	File dir(directory);
	if (dir.isDirectory()) {
		File newFile = dir.getNonexistentChildFile(desiredFileName, ".syx", false);
		saveSysex(newFile.getFullPathName().toStdString(), messages);
		return newFile.getFullPathName().toStdString();
	}
	jassert(false);
	return "Failure";
}


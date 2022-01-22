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

#include "MidiRecorder.h"

#include "Settings.h"
#include "StreamLogger.h"

MidiRecorder::MidiRecorder(AudioDeviceManager &deviceManager) : isRecording_(false), deviceManager_(deviceManager)
{
	// Just enable *all* Midi devices. Not sure if that's smart in the long run, but hey...
	auto devices = MidiInput::getAvailableDevices();
	for (int i = 0; i < devices.size(); i++) {
		enableMidiInput(devices[i]);
	}
	clocker_ = std::make_shared<MidiClocker>();
}

void MidiRecorder::saveToFile(String filename)
{
	Time now = Time::getCurrentTime();
	File directory = Settings::instance().getSessionStorageDir();
	File midFile = directory.getNonexistentChildFile(String(filename) + now.formatted("-%Y-%m-%d-%H-%M-%S"), ".mid", false);
	MidiFile midiFile;
	for (auto track : recorded_) {
		if (track.second.getNumEvents() > 0) {
			track.second.updateMatchedPairs();
			midiFile.addTrack(track.second);
		}
	}
	FileOutputStream outputStream(midFile, 16384);
	//midiFile.setSmpteTimeFormat(25, 400); // Tenths of milliseconds at 25 fps
	midiFile.setTicksPerQuarterNote(96);
	midiFile.writeTo(outputStream);
}

std::weak_ptr<MidiClocker>  MidiRecorder::getClocker()
{
	return clocker_;
}

MidiRecorder::~MidiRecorder()
{
	// Remove all registered callbacks
	for (auto callback : callbacks_) {
		deviceManager_.removeMidiInputDeviceCallback(callback.first, callback.second);
	}
	saveToFile("sessionMidi");
}

void MidiRecorder::startRecording()
{
	recordingStartTime_ = Time::getMillisecondCounterHiRes() / 1000.0;
	isRecording_ = true;
}

void MidiRecorder::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	if (!message.isActiveSense() && !message.isMidiClock()) {
		StreamLogger::instance() << message.getTimeStamp() << " " << message.getDescription() << std::endl;
		MidiMessage relativeTimestampMessage = message;
		double deltaSeconds = message.getTimeStamp() - recordingStartTime_;
		double deltaTicksPerBeat = deltaSeconds * 120 * 96 / 60; // 96 Ticks per quarter note at 120 bpm
		relativeTimestampMessage.setTimeStamp(deltaTicksPerBeat);
		recorded_[source->getName()].addEvent(relativeTimestampMessage);
	}
	else if (message.isMidiClock()) {
		if (clocker_) {
			clocker_->processClockMessage(source->getName(), message);
		}
	}
}

void MidiRecorder::handlePartialSysexMessage(MidiInput* source, const uint8* messageData, int numBytesSoFar, double timestamp)
{
	 // What to do with this one?
	ignoreUnused(source, messageData, numBytesSoFar, timestamp);
}

void MidiRecorder::enableMidiInput(MidiDeviceInfo info)
{
	// Only enable and register once
	if (!deviceManager_.isMidiInputDeviceEnabled(info.identifier)) {
		deviceManager_.setMidiInputDeviceEnabled(info.identifier, true);
	}
	if (callbacks_.find(info.identifier) == callbacks_.end()) {
		deviceManager_.addMidiInputDeviceCallback(info.identifier, this);
		callbacks_[info.identifier] = this;
	}
	if (recorded_.find(info.identifier) == recorded_.end()) {
		recorded_[info.identifier] = juce::MidiMessageSequence();
	}
}

void MidiRecorder::disableMidiInput(MidiDeviceInfo info)
{
	if (deviceManager_.isMidiInputDeviceEnabled(info.identifier)) {
		deviceManager_.setMidiInputDeviceEnabled(info.identifier, false);
	}
	if (callbacks_.find(info.identifier) == callbacks_.end()) {
		deviceManager_.removeMidiInputDeviceCallback(info.identifier, callbacks_[info.identifier]);
		callbacks_.erase(info.identifier);
	}
}

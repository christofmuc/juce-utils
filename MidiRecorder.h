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

#include "MidiClocker.h"

class MidiRecorder : private juce::MidiInputCallback {
public:
	MidiRecorder(juce::AudioDeviceManager &deviceManager);
	virtual ~MidiRecorder() override;

	void startRecording();
	void saveToFile(juce::String filename);

	std::weak_ptr<MidiClocker> getClocker();

private:
	// Midi Input Callback
	virtual void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
	virtual void handlePartialSysexMessage(juce::MidiInput* source, const juce::uint8* messageData, int numBytesSoFar, double timestamp) override;

	void enableMidiInput(juce::MidiDeviceInfo info);
	void disableMidiInput(juce::MidiDeviceInfo info);

	std::map<juce::String, juce::MidiMessageSequence> recorded_;
	bool isRecording_;
	double recordingStartTime_;

	std::map<String, juce::MidiInputCallback *>  callbacks_;
	juce::AudioDeviceManager &deviceManager_;

	std::shared_ptr<MidiClocker> clocker_; // To determine the BPM from the Midi Clock signals
};

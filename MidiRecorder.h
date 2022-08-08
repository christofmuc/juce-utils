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

class MidiRecorder : private MidiInputCallback {
public:
	MidiRecorder(AudioDeviceManager &deviceManager);
	virtual ~MidiRecorder() override;

	void startRecording();
	void saveToFile(String filename);

	std::weak_ptr<MidiClocker> getClocker();

private:
	// Midi Input Callback
	virtual void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;
	virtual void handlePartialSysexMessage(MidiInput* source, const uint8* messageData, int numBytesSoFar, double timestamp) override;

	void enableMidiInput(MidiDeviceInfo name);
	void disableMidiInput(MidiDeviceInfo input);

	std::map<String, MidiMessageSequence> recorded_;
	bool isRecording_;
	double recordingStartTime_;

	std::map<String, MidiInputCallback *>  callbacks_;
	AudioDeviceManager &deviceManager_;

	std::shared_ptr<MidiClocker> clocker_; // To determine the BPM from the Midi Clock signals
};

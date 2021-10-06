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

enum class RecordingType {
	WAV,
	FLAC,
	AIFF
};


class AudioRecorder : public AudioIODeviceCallback {
public:
	AudioRecorder(File directory, std::string const &baseFileName, RecordingType recordingType);
	virtual ~AudioRecorder() override;

	void startRecording(std::string const &filename, bool fromSilenceToSilence, std::function<void()> onSilence);
	void stopRecording();
	bool isRecording() const;
	bool hasDetectedSignal() const;

	RelativeTime getElapsedTime() const;
	String getFilename() const;
	File getFile() const;

	void updateChannelInfo(int sampleRate, int numChannels);

	File getDirectory() const;
	void setDirectory(File &directory);

	virtual void audioDeviceIOCallback(const float** inputChannelData, int numInputChannels, float** outputChannelData, int numOutputChannels, int numSamples) override;
	virtual void audioDeviceAboutToStart(AudioIODevice* device) override;
	virtual void audioDeviceStopped() override;

private:
	void saveBlock(const float* const* data, int numSamples);

	Time startTime_;
	uint64 samplesWritten_;
	File activeFile_;
	File directory_;
	std::string baseFileName_;
	RecordingType recordingType_;
	AudioFormatWriter *writer_;
	std::unique_ptr<TimeSliceThread> thread_;
	std::unique_ptr<AudioFormatWriter::ThreadedWriter> writeThread_;

	int lastSampleRate_;
	int lastNumChannels_;

	bool hasFoundStart_;
	bool automaticRecordFromSilenceToSilence_;
	size_t silenceDuration_;
	std::function<void()> onSilence_;
};

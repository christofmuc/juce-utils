/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
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
	virtual ~AudioRecorder();

	void setRecording(bool recordOn);
	bool isRecording() const;

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
};



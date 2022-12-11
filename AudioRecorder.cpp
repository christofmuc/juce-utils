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

#include "AudioRecorder.h"

#include "Logger.h"

const float kSignalThreshold = 0.001f; // Signals below this value are considered noise (don't trigger recording)

AudioRecorder::AudioRecorder(File directory, std::string const& baseFileName, RecordingType recordingType) :
    samplesWritten_(0), directory_(directory), baseFileName_(baseFileName), recordingType_(recordingType), writer_(nullptr), automaticRecordFromSilenceToSilence_(false), silenceDuration_(0)
{
    thread_ = std::make_unique<TimeSliceThread>("RecorderDiskWriter");
    thread_->startThread();
}


AudioRecorder::~AudioRecorder()
{
    // Stop writing, make sure to finalize file
    writeThread_.reset();
    // Give it a second to flush and exit
    thread_->stopThread(1000);
}

void AudioRecorder::startRecording(std::string const& filename, bool fromSilenceToSilence, std::function<void()> onSilence)
{
    automaticRecordFromSilenceToSilence_ = fromSilenceToSilence;
    activeFile_ = File(filename);
    onSilence_ = onSilence;
    if (isRecording()) {
        // Stop current recorder first
        writeThread_.reset();
    }

    // This will reinitialize and start the recorder
    samplesWritten_ = 0;
    hasFoundStart_ = false;
    silenceDuration_ = 0;
    updateChannelInfo(lastSampleRate_, lastNumChannels_);
}

void AudioRecorder::stopRecording()
{
    writeThread_.reset();
}

bool AudioRecorder::isRecording() const
{
    return writeThread_ != nullptr;
}

bool AudioRecorder::hasDetectedSignal() const
{
    return isRecording() && hasFoundStart_;
}

RelativeTime AudioRecorder::getElapsedTime() const
{
    return RelativeTime(samplesWritten_ / (double) lastSampleRate_);
}

juce::String AudioRecorder::getFilename() const
{
    return activeFile_.getFullPathName();
}

juce::File AudioRecorder::getFile() const
{
    return activeFile_;
}

void AudioRecorder::updateChannelInfo(int sampleRate, int numChannels)
{
    lastSampleRate_ = sampleRate;
    lastNumChannels_ = numChannels;

    // We have changed the channel setup - as our output files do like a varying number of channels (you need a DAW project for that)
    // let's close the current file and start a new one
    writeThread_.reset();

    // Create the audio format writer
    std::unique_ptr<AudioFormat> audioFormat;
    std::string fileExtension;
    switch (recordingType_) {
    case RecordingType::WAV:
        audioFormat = std::make_unique<WavAudioFormat>();
        fileExtension = ".wav";
        break;
    case RecordingType::FLAC:
        audioFormat = std::make_unique<FlacAudioFormat>();
        fileExtension = ".flac";
        break;
    case RecordingType::AIFF:
        audioFormat = std::make_unique<AiffAudioFormat>();
        fileExtension = ".aiff";
        break;
    }

    // Need to check that sample rate, bit depth, and channel layout are supported!
    int bitDepthRequested = 16;
    bool bitsOk = false;
    for (auto allowed : audioFormat->getPossibleBitDepths())
        if (allowed == bitDepthRequested) bitsOk = true;
    if (!bitsOk) {
        jassert(false);
        SimpleLogger::instance()->postMessage("Error: trying to create a file with a bit depth that is not supported by the format: " + String(bitDepthRequested));
        return;
    }

    bool rateOk = false;
    for (auto rate : audioFormat->getPossibleSampleRates())
        if (rate == sampleRate) rateOk = true;
    if (!rateOk) {
        jassert(false);
        SimpleLogger::instance()->postMessage("Error: trying to create a file with a sample rate that is not supported by the format: " + String(sampleRate));
        return;
    }

    /*// Set up the channel layout
    AudioChannelSet channels;
    int numChannels = 0;
    for (int c = 0; c < MAXCHANNELSPERCLIENT; c++) {
            switch (channelSetup.channels[c].target) {
            case Unused:
                    // Ignore
                    break;
            case Left:
                    channels.addChannel(AudioChannelSet::left);
                    numChannels++;
                    break;
            case Right:
                    channels.addChannel(AudioChannelSet::right);
                    numChannels++;
                    break;
            case SendOnly:
                    // Fall through
            case Mono:
                    channels.addChannel(AudioChannelSet::centre);
                    numChannels++;
                    break;
            }
    }

    // Check if WAV likes it
    if (!audioFormat->isChannelLayoutSupported(channels)) {
            return;
    }*/

    // Set up a new audio file to write to
    startTime_ = Time::getCurrentTime();
    if (activeFile_.getFullPathName().isEmpty()) {
        return;
        // activeFile_ = directory_.getNonexistentChildFile(String(baseFileName_) + startTime_.formatted("-%Y-%m-%d-%H-%M-%S"), fileExtension, false);
    }
    if (activeFile_.existsAsFile()) {
        SimpleLogger::instance()->postMessage("Overwriting file " + activeFile_.getFullPathName());
        activeFile_.deleteFile();
    }
    OutputStream* outStream = new FileOutputStream(activeFile_, 16384);

    // Create the writer based on the format and file
    StringPairArray metaData;
    writer_ = audioFormat->createWriterFor(outStream, sampleRate, (unsigned) numChannels, bitDepthRequested, metaData, 1 /* unused by wav */);
    if (!writer_) {
        jassert(false);
        delete outStream;
        SimpleLogger::instance()->postMessage("Fatal: Could not create writer for Audio file, can't record to disk");
        return;
    }

    // Finally, create the new writer associating it with the background thread
    writeThread_ = std::make_unique<AudioFormatWriter::ThreadedWriter>(writer_, *thread_, 16384);
    samplesWritten_ = 0;
}

void AudioRecorder::saveBlock(const float* const* data, int numSamples)
{
    // Don't crash on me when there is surprisingly no data in the package
    if (data && data[0]) {
        if (!writeThread_->write(data, numSamples)) {
            // TODO - need a smarter strategy than that
            SimpleLogger::instance()->postMessage("Ups, FIFO full and can't write block to disk, lost it!");
        }
        samplesWritten_ += (uint64) numSamples;
    }
}

juce::File AudioRecorder::getDirectory() const
{
    return directory_;
}

void AudioRecorder::setDirectory(File& directory)
{
    // Stop writing if any
    writeThread_.reset();
    directory_ = directory;
}

#if JUCE_VERSION < 0x070000
void AudioRecorder::audioDeviceIOCallback(const float** inputChannelData, int numInputChannels, float** outputChannelData, int numOutputChannels, int numSamples)
{
#else
void AudioRecorder::audioDeviceIOCallbackWithContext (const float* const* inputChannelData,
                                               int numInputChannels,
                                               float* const* outputChannelData,
                                               int numOutputChannels,
                                               int numSamples,
                                               const AudioIODeviceCallbackContext& context)
                                               {
                                                   ignoreUnused(context);
#endif
    // First, hand through all input channels to the output channels so you have a monitor signal
    int nextInput = 0;
    for (auto channel = 0; channel < numOutputChannels; ++channel) {
        if (outputChannelData[channel]) {
            // Output not nullptr, needs filling. Find the next unsued input channel
            while (nextInput < numInputChannels && inputChannelData[nextInput] == nullptr)
                nextInput++;
            if (inputChannelData[nextInput] != nullptr) {
                std::copy(inputChannelData[nextInput], inputChannelData[nextInput] + numSamples, outputChannelData[channel]);
            }
            else {
                // No more inputs, clear output
                std::fill(outputChannelData[channel], outputChannelData[channel] + numSamples, 0.0f);
            }
        }
    }

    // Now decide if we are going to record anything to disk
    if (numInputChannels == lastNumChannels_ && numInputChannels > 0) {
        if (automaticRecordFromSilenceToSilence_) {
            bool silence = true;
            // Check if some data in there...
            for (int s = 0; s < numSamples; s++) {
                if (fabs(inputChannelData[0][s]) > kSignalThreshold) {
                    hasFoundStart_ = true;
                    silence = false;
                    break;
                }
            }
            if (silence) {
                silenceDuration_ += (size_t) numSamples;
            }
            else {
                silenceDuration_ = 0;
            }
        }
        else {
            // Start immediately
            hasFoundStart_ = true;
        }

        if (hasFoundStart_ && writeThread_) {
            saveBlock(inputChannelData, numSamples);

            if (automaticRecordFromSilenceToSilence_ && silenceDuration_ > 0.01 * lastSampleRate_) {
                // End of story
                writeThread_.reset();
                MessageManager::callAsync([this]() { onSilence_(); });
                hasFoundStart_ = false;
            }
        }
    }
    else {
        jassertfalse;
    }
}

void AudioRecorder::audioDeviceAboutToStart(AudioIODevice* device)
{
    auto inputChannelMask = device->getActiveInputChannels();
    hasFoundStart_ = false;
    updateChannelInfo((int) device->getCurrentSampleRate(), inputChannelMask.countNumberOfSetBits());
}

void AudioRecorder::audioDeviceStopped()
{
    writeThread_.reset();
}

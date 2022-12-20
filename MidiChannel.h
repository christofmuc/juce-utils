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

#pragma once

// Just saw too many bugs not knowing if the int is zero or one based...

class MidiChannel {
public:
    static MidiChannel fromZeroBase(int zeroBasedInt) { return MidiChannel(zeroBasedInt); }
    static MidiChannel fromOneBase(int oneBasedInt) { return MidiChannel(oneBasedInt - 1); }
    static MidiChannel omniChannel() { return MidiChannel(0, true); }                                       // For now, assume Omni is channel 1
    static MidiChannel MPEmode(int zerobasedChannel) { return MidiChannel(zerobasedChannel, false, true); } // Set MPE mode to a specific channel, but make it a valid channel!
    static MidiChannel invalidChannel() { return MidiChannel(); }

    int toZeroBasedInt() const { return zerobasedChannel_; }
    int toOneBasedInt() const { return zerobasedChannel_ + 1; }

    bool isValid() const { return isValid_; }
    bool isOmni() const { return isOmni_; }
    bool isMPE() const { return isMPE_; }

private:
    int zerobasedChannel_;
    bool isValid_;
    bool isOmni_;
    bool isMPE_; // Sequential synths can be switched to MPE mode where they react to multiple MIDI channels

    // Don't call the constructor with an int, you wouldn't know what you mean. This is why it is private
    explicit MidiChannel(int zerobasedChannel, bool isOmni = false, bool isMPE = false) : zerobasedChannel_(zerobasedChannel), isOmni_(isOmni), isMPE_(isMPE)
    {
        isValid_ = zerobasedChannel >= 0 && zerobasedChannel < 16;
    }

    MidiChannel() : zerobasedChannel_(-1), isValid_(false), isOmni_(false), isMPE_(false) {}
};

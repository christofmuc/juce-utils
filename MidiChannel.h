/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

// Just saw too many bugs not knowing if the int is zero or one based...

class MidiChannel {
public:
	static MidiChannel fromZeroBase(int zeroBasedInt) { return MidiChannel(zeroBasedInt); }
	static MidiChannel fromOneBase(int oneBasedInt) { return MidiChannel(oneBasedInt - 1); }
	static MidiChannel omniChannel() { return MidiChannel(0, true); } // For now, assume Omni is channel 1
	static MidiChannel invalidChannel() { return MidiChannel(); }

	int toZeroBasedInt() const { return zerobasedChannel_; }
	int toOneBasedInt() const { return zerobasedChannel_ + 1; }

	bool isValid() const { return isValid_; }
	bool isOmni() const { return isOmni_; }

private:
	int zerobasedChannel_;
	bool isValid_;
	bool isOmni_;

	// Don't call the constructor with an int, you wouldn't know what you mean. This is why it is private
	MidiChannel(int zerobasedChannel, bool isOmni = false) : zerobasedChannel_(zerobasedChannel), isOmni_(isOmni) {
		isValid_ = zerobasedChannel >= 0 && zerobasedChannel < 16;
	};

	MidiChannel() : zerobasedChannel_(-1), isValid_(false), isOmni_(false) {
	}
};


/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "MidiHelpers.h"

juce::MidiMessage MidiHelpers::sysexMessage(std::vector<uint8> const &sysEx)
{
	return MidiMessage::createSysExMessage(sysEx.data(), (int)sysEx.size());
}

juce::MidiBuffer MidiHelpers::bufferFromMessages(std::vector<MidiMessage> const &messages)
{
	// We assume the same timestamp for all messages in this little helper
	MidiBuffer buffer;
	int num = 0;
	for (auto message : messages) {
		jassert(message.getRawDataSize() <= 65535);
		buffer.addEvent(message, num++);
	}
	return buffer;
}

std::vector<MidiMessage> MidiHelpers::generateRPN(int midiChannel,
	int parameterNumber,
	int value,
	bool isNRPN,
	bool use14BitValue,
	bool MSBbeforeLSB)
{
	jassert(midiChannel > 0 && midiChannel <= 16);
	jassert(parameterNumber >= 0 && parameterNumber < 16384);
	jassert(value >= 0 && value < (use14BitValue ? 16384 : 128));

	uint8 parameterLSB = uint8(parameterNumber & 0x7f);
	uint8 parameterMSB = uint8(parameterNumber >> 7);

	uint8 valueLSB = use14BitValue ? uint8(value & 0x7f) : 0x00;
	uint8 valueMSB = use14BitValue ? uint8(value >> 7) : uint8(value);

	uint8 channelByte = uint8(0xb0 + midiChannel - 1);

	std::vector<MidiMessage> buffer;

	if (MSBbeforeLSB) {
		buffer.push_back(MidiMessage(channelByte, isNRPN ? 0x63 : 0x65, parameterMSB));
		buffer.push_back(MidiMessage(channelByte, isNRPN ? 0x62 : 0x64, parameterLSB));
		buffer.push_back(MidiMessage(channelByte, 0x06, valueMSB));
		if (use14BitValue)
			buffer.push_back(MidiMessage(channelByte, 0x26, valueLSB));
	}
	else {
		buffer.push_back(MidiMessage(channelByte, isNRPN ? 0x62 : 0x64, parameterLSB));
		buffer.push_back(MidiMessage(channelByte, isNRPN ? 0x63 : 0x65, parameterMSB));
		// sending the value LSB is optional, but must come before sending the value MSB:
		if (use14BitValue)
			buffer.push_back(MidiMessage(channelByte, 0x26, valueLSB));
		buffer.push_back(MidiMessage(channelByte, 0x06, valueMSB));
	}

	return buffer;
}

bool MidiHelpers::equalSysexMessageContent(MidiMessage const &message1, MidiMessage const &message2, int digitsToCompare /* = -1 */)
{
	if (!(message1.isSysEx() & message2.isSysEx())) return false;
	if (digitsToCompare == -1) { 
		if (message1.getSysExDataSize() != message2.getSysExDataSize()) return false;
		// The complete message should be compared
		digitsToCompare = message1.getSysExDataSize(); 
	}
	else {
		if (message1.getSysExDataSize() < digitsToCompare || message2.getSysExDataSize() < digitsToCompare) return false;
	}
	for (int i = 0; i < message1.getSysExDataSize() && i < digitsToCompare; i++) {
		if (message1.getSysExData()[i] != message2.getSysExData()[i]) {
			return false;
		}
	}
	return true;
}

bool MidiHelpers::isSysexMessageMatching(MidiMessage const &message, std::vector<std::pair<size_t, uint8>> const &indexAndContentCondition)
{
	if (!message.isSysEx()) {
		return false;
	}

	for (auto const &condition : indexAndContentCondition) {
		// Check if the value at index (first) equals content (second). Do check index access first!
		if ((size_t)message.getSysExDataSize() <= condition.first) {
			// Index out of range
			return false;
		}
		if ((size_t)message.getSysExData()[condition.first] != condition.second) {
			// Value mismatch
			return false;
		}
	}
	return true;
}

juce::uint8 MidiHelpers::checksum7bit(std::vector<uint8> const &data)
{
	int sum = 0;
	std::for_each(data.begin(), data.end(), [&](uint8 byte) { sum += byte;  });
	return sum & 0x7f;
}

bool MidiHelpers::isEmptySysex(MidiMessage const &m) {
	return 	m.isSysEx() && m.getSysExDataSize() == 0;
}

juce::MidiBuffer MidiHelpers::removeEmptySysexMessages(MidiBuffer const &midiBuffer)
{
	MidiBuffer filtered;
	for (auto message : midiBuffer) {
		auto m = message.getMessage();
		// Suppress empty sysex messages, they seem to confuse vintage hardware (e.g. the Kawai K3 in particular)
		if (isEmptySysex(m)) continue;		
		jassert(m.getRawDataSize() <= 65535);
		filtered.addEvent(m, message.samplePosition);
	}
	return filtered;
}

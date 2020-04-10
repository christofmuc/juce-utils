#include "PackedDataFormatInfo.h"

std::vector<juce::uint8> PackedDataFormatInfo::applyMapping(std::vector<PackedDataFormatInfo> const &kToneFormatDefinition, std::vector<uint8> const &source, size_t sizeDestination)
{
	// Build up the APR tone record, which will be our preferred internal format and the one to store stuff in the database
	// If I trust the documentation correctly, this is also a message the MKS-50 will recognize without the user pressing a front panel button
	std::vector<uint8> aprBlock(sizeDestination, 0);
	for (auto info : kToneFormatDefinition) {
		// Read the value from the input block
		uint8 value = (source[info.byteIndex] >> info.lsbIndex) & ((1 << info.bitCount) - 1);
		// Map it!
		if (info.converter_) {
			value = info.converter_(value);
		}
		// Poke it into the right place 
		aprBlock[info.targetParameter] |= value << info.targetBitIndex;
	}
	return aprBlock;
}

#pragma once

#include "JuceHeader.h"

// This is the definition of the tone data format, which is really bit packed with individual bits spread all over the place. RAM was expensive...
class PackedDataFormatInfo {
public:
	PackedDataFormatInfo(int byteIndex, int lsbIndex, int bitCount, int targetParameter, int targetBitIndex = 0) :
		byteIndex(byteIndex), lsbIndex(lsbIndex), bitCount(bitCount), targetParameter(targetParameter), targetBitIndex(targetBitIndex) {
	}

	PackedDataFormatInfo(int byteIndex, int lsbIndex, int bitCount, int targetParameter, std::function<uint8(uint8)> converter) :
		PackedDataFormatInfo(byteIndex, lsbIndex, bitCount, targetParameter) {
		converter_ = converter;
	}

	int byteIndex;
	int lsbIndex;
	int bitCount;
	int targetParameter;
	int targetBitIndex;
	std::function<uint8(uint8)> converter_;

	static std::vector<uint8> applyMapping(std::vector<PackedDataFormatInfo> const &kToneFormatDefinition, std::vector<uint8> const &source, size_t sizeDestination);
};

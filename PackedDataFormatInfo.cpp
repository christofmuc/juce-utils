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
#include "PackedDataFormatInfo.h"

std::vector<juce::uint8> PackedDataFormatInfo::applyMapping(std::vector<PackedDataFormatInfo> const &kToneFormatDefinition, std::vector<uint8> const &source, size_t sizeDestination)
{
	// Build up the APR tone record, which will be our preferred internal format and the one to store stuff in the database
	// If I trust the documentation correctly, this is also a message the MKS-50 will recognize without the user pressing a front panel button
	std::vector<uint8> aprBlock(sizeDestination, 0);
	for (auto info : kToneFormatDefinition) {
		// Read the value from the input block
		uint8 value = (source[(size_t) info.byteIndex] >> info.lsbIndex) & ((1 << info.bitCount) - 1);
		// Map it!
		if (info.converter_) {
			value = info.converter_(value);
		}
		// Poke it into the right place
		aprBlock[(size_t) info.targetParameter] |= value << info.targetBitIndex;
	}
	return aprBlock;
}

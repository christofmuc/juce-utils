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

#include <stdint.h>
#include <functional>
#include <vector>

// This is the definition of the tone data format, which is really bit packed with individual bits spread all over the place. RAM was expensive...
class PackedDataFormatInfo {
public:
    PackedDataFormatInfo(int byteInd, int lsbInd, int bitCnt, int target, int targetBitInd = 0) :
        byteIndex(byteInd), lsbIndex(lsbInd), bitCount(bitCnt), targetParameter(target), targetBitIndex(targetBitInd)
    {
    }

    PackedDataFormatInfo(int byteInd, int lsbInd, int bitCnt, int target, std::function<uint8_t(uint8_t)> converter) : PackedDataFormatInfo(byteInd, lsbInd, bitCnt, target) { converter_ = converter; }

    int byteIndex;
    int lsbIndex;
    int bitCount;
    int targetParameter;
    int targetBitIndex;
    std::function<uint8_t(uint8_t)> converter_;

    static std::vector<uint8_t> applyMapping(std::vector<PackedDataFormatInfo> const &kToneFormatDefinition, std::vector<uint8_t> const &source, size_t sizeDestination);
};

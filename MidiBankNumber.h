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

#include <juce_core/juce_core.h>
#include <nlohmann/json.hpp>


class MidiBankNumber {
public:
    static MidiBankNumber fromOneBase(int bankNo, int banksize);
    static MidiBankNumber fromZeroBase(int bankNo, int banksize);
    static MidiBankNumber invalid();

    bool isValid() const;
    int toZeroBased() const;
    int toOneBased() const;

    int bankSize() const;

    bool operator!=(MidiBankNumber const& other) const;

    nlohmann::json toJson() const;
    static MidiBankNumber fromJson(nlohmann::json& json);

private:
    MidiBankNumber(int zeroBasedNumber, int banksize);

    int bankNo_;
    int bankSize_;
};

template <> class juce::VariantConverter<MidiBankNumber> {
public:
    static MidiBankNumber fromVar(const var& v);
    static var toVar(const MidiBankNumber& t);
};

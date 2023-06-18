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

#include "MidiBankNumber.h"

#include <spdlog/spdlog.h>

#include <stdexcept>

MidiBankNumber MidiBankNumber::fromOneBase(int bankNo, int banksize)
{
    return MidiBankNumber(bankNo - 1, banksize);
}

MidiBankNumber MidiBankNumber::fromZeroBase(int bankNo, int banksize)
{
    return MidiBankNumber(bankNo, banksize);
}

MidiBankNumber MidiBankNumber::invalid()
{
    return MidiBankNumber(-1, 0);
}

MidiBankNumber::MidiBankNumber(int zeroBasedNumber, int banksize) : bankNo_(zeroBasedNumber), bankSize_(banksize)
{
}

bool MidiBankNumber::isValid() const
{
    return bankNo_ != -1;
}

int MidiBankNumber::toZeroBased() const
{
    if (!isValid()) {
        throw new std::runtime_error("Illegal State");
    }
    return bankNo_;
}

int MidiBankNumber::toOneBased() const
{
    if (!isValid()) {
        throw new std::runtime_error("Illegal State");
    }
    return bankNo_ + 1;
}

int MidiBankNumber::bankSize() const
{
    return bankSize_;
}

bool MidiBankNumber::operator!=(MidiBankNumber const& other) const
{
    return bankNo_ != other.bankNo_;
}

nlohmann::json MidiBankNumber::toJson() const
{
    try {
        if (isValid()) {
            return { { "valid", true }, { "bank", bankNo_ }, { "bankSize", bankSize_ } };
        }
        else {
            return { { "valid", false } };
        }
    }
    catch (std::exception& e) {
        spdlog::error("{}", e.what());
        return { { "valid", false } };
    }
}

MidiBankNumber MidiBankNumber::fromJson(nlohmann::json& json)
{
    try {
        if (json["valid"] == false) {
            return MidiBankNumber::invalid();
        }
        else {
            return MidiBankNumber::fromZeroBase(json["bank"], json["bankSize"]);
        }
    }
    catch (std::exception& e) {
        jassertfalse;
        spdlog::error("{}", e.what());
        return MidiBankNumber::invalid();
    }
}


MidiBankNumber juce::VariantConverter<MidiBankNumber>::fromVar(const juce::var& v)
{
    std::string text = v.toString().toStdString();
    try {
        auto json = nlohmann::json::parse(text);
        return MidiBankNumber::fromJson(json);
    }
    catch (std::exception& e) {
        jassertfalse;
        spdlog::error("{}", e.what());
        return MidiBankNumber::invalid();
    }
}


juce::var juce::VariantConverter<MidiBankNumber>::toVar(const MidiBankNumber& t)
{
    auto asJson = t.toJson();
    return String(asJson.dump());
}

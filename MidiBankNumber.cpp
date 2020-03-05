/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "MidiBankNumber.h"

MidiBankNumber MidiBankNumber::fromOneBase(int bankNo)
{
	return MidiBankNumber(bankNo - 1);
}

MidiBankNumber MidiBankNumber::fromZeroBase(int bankNo)
{
	return MidiBankNumber(bankNo);
}

MidiBankNumber MidiBankNumber::invalid()
{
	return MidiBankNumber(-1);
}

MidiBankNumber::MidiBankNumber(int zeroBasedNumber) : bankNo_(zeroBasedNumber)
{
}

int MidiBankNumber::isValid() const
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

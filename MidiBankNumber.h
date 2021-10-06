/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include <stdexcept>

class MidiBankNumber {
public:
	static MidiBankNumber fromOneBase(int bankNo);
	static MidiBankNumber fromZeroBase(int bankNo);
	static MidiBankNumber invalid();

	int isValid() const;
	int toZeroBased() const;
	int toOneBased() const;

private:
	MidiBankNumber(int zeroBasedNumber);

	int bankNo_;
};

#pragma once

#include "JuceHeader.h"

enum class ValueType {
	Integer,
	Bool,
	Lookup
};

struct  TypedNamedValue {
	String name;
	String sectionName;
	Value value;
	ValueType valueType;
	int minValue;
	int maxValue;
	std::map<int, std::string> lookup;
};


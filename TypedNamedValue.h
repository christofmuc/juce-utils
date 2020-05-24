/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

enum class ValueType {
	Integer,
	Bool,
	String,
	Lookup
};

class TypedNamedValue {
public:
	//! Construct a Bool type
	TypedNamedValue(String const &name, String const &sectionName, bool defaultValue);
	//! Construct a String type (edit field)
	TypedNamedValue(String const &name, String const &sectionName, String const &defaultValue, int maxLength);
	//! Construct an Integer type
	TypedNamedValue(String const &name, String const &sectionName, int defaultValue, int minValue, int maxValue);
	//! Construct a lookup value type
	TypedNamedValue(String const &name, String const &sectionName, int defaultValue, std::map<int, std::string> const &lookup);

	virtual ~TypedNamedValue() = default;

	Value &value();
	String name() const;
	String sectionName() const;
	ValueType valueType() const;
	int minValue() const;
	int maxValue() const;
	bool enabled() const;
	std::map<int, std::string> lookup() const;

	// For lookups only
	int indexOfValue(std::string const &text) const;  //TODO - suboptimal linear search
	std::string lookupValue();
	void setLookup(std::map<int, std::string> const &newLookup);

protected:
	String name_;
	String sectionName_;
	Value value_;
	ValueType valueType_;
	int minValue_;
	int maxValue_;
	std::map<int, std::string> lookup_;
	bool enabled_;
};

class TypedNamedValueSet : public std::vector<std::shared_ptr<TypedNamedValue>> {
public:
	std::shared_ptr<TypedNamedValue> typedNamedValueByName(std::string const &name); //TODO - suboptimal linear search
	bool hasValue(std::string const &name);
	Value &valueByName(std::string const &name); //TODO - suboptimal linear search
};


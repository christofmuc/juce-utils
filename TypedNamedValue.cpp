/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "TypedNamedValue.h"

TypedNamedValue::TypedNamedValue(String const &name, String const &sectionName, bool defaultValue) : name_(name), sectionName_(sectionName), valueType_(ValueType::Bool)
{
	value_ = Value(defaultValue);
	minValue_ = 0;
	maxValue_ = 1;
	enabled_ = true;
}

TypedNamedValue::TypedNamedValue(String const &name, String const &sectionName, String const &defaultValue, int maxLength) : name_(name), sectionName_(sectionName), valueType_(ValueType::String)
{
	value_ = Value(defaultValue);
	minValue_ = 0;
	maxValue_ = maxLength;
	enabled_ = true;
}

TypedNamedValue::TypedNamedValue(String const &name, String const &sectionName, int defaultValue, int minValue, int maxValue) : name_(name), sectionName_(sectionName), valueType_(ValueType::Integer)
{
	value_ = Value(defaultValue);
	minValue_ = minValue;
	maxValue_ = maxValue;
	enabled_ = true;
}

TypedNamedValue::TypedNamedValue(String const &name, String const &sectionName, int defaultValue, std::map<int, std::string> const &lookup) : name_(name), sectionName_(sectionName), valueType_(ValueType::Lookup)
{
	setLookup(lookup);
	value_ = Value(defaultValue);
	enabled_ = true;
}

TypedNamedValue::TypedNamedValue(String const &name, String const &sectionName, File const &defaultValue, bool isDirectory) : name_(name), sectionName_(sectionName), valueType_(isDirectory ? ValueType::Pathname : ValueType::Filename)
{
	value_ = Value(defaultValue.getFullPathName());
	enabled_ = true;
}

TypedNamedValue::TypedNamedValue(String const &name, String const &sectionName, Colour defaultValue) : name_(name), sectionName_(sectionName), valueType_(ValueType::Color)
{
	value_ = Value(defaultValue.toString());
	enabled_ = true;
}

juce::Value &TypedNamedValue::value()
{
	return value_;
}

juce::String TypedNamedValue::name() const
{
	return name_;
}

juce::String TypedNamedValue::sectionName() const
{
	return sectionName_;
}

ValueType TypedNamedValue::valueType() const
{
	return valueType_;
}

int TypedNamedValue::minValue() const
{
	return minValue_;
}

int TypedNamedValue::maxValue() const
{
	return maxValue_;
}

bool TypedNamedValue::enabled() const
{
	return enabled_;
}

std::map<int, std::string> TypedNamedValue::lookup() const
{
	return lookup_;
}

int TypedNamedValue::indexOfValue(std::string const &text) const
{
	for (auto const &d : lookup_) {
		if (d.second == text) {
			return d.first;
		}
	}
	return 0;
}

std::string TypedNamedValue::lookupValue()
{
	return lookup_[int(value_.getValue())];
}

void TypedNamedValue::setLookup(std::map<int, std::string> const &newLookup)
{
	lookup_ = newLookup;
	if (lookup_.empty()) {
		minValue_ = maxValue_ = 0;
	}
	else {
		minValue_ = lookup_.begin()->first;
		maxValue_ = lookup_.rbegin()->first;
	}
}

std::shared_ptr<TypedNamedValue> TypedNamedValueSet::typedNamedValueByName(std::string const &name)
{
	for (auto tnv : *this) {
		if (tnv->name().toStdString() == name) {
			return tnv;
		}
	}
	jassertfalse;
	return {};
}

bool TypedNamedValueSet::hasValue(std::string const &name)
{
	for (auto tnv : *this) {
		if (tnv->name().toStdString() == name) {
			return true;
		}
	}
	return false;
}

juce::Value &TypedNamedValueSet::valueByName(std::string const &name)
{
	for (auto tnv : *this) {
		if (tnv->name().toStdString() == name) {
			return tnv->value();
		}
	}
	throw std::runtime_error("Illegal argument - key does not exist");
}

void TypedNamedValueSet::addToValueTree(ValueTree &tree)
{
	for (auto param : *this) {
		tree.setProperty(param->name(), param->value().getValue(), nullptr);
		auto v = tree.getPropertyAsValue(Identifier(param->name()), nullptr, false);
		param->value().referTo(v);
	}
}

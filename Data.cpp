/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "Data.h"

#include "Settings.h"


Data Data::instance_;

Data & Data::instance()
{
	return instance_;
}

const juce::var& Data::getProperty(const Identifier& name)
{
	return Data::instance().get().getProperty(name);
}

const juce::var& Data::getEphemeralProperty(const Identifier& name)
{
	return Data::instance().getEphemeral().getProperty(name);
}

juce::Value Data::getPropertyAsValue(const Identifier& name)
{
	return Data::instance().get().getPropertyAsValue(name, nullptr);
}

juce::Value Data::getEphemeralPropertyAsValue(const Identifier& name)
{
	return Data::instance().getEphemeral().getPropertyAsValue(name, nullptr);
}

Data::Data() : tree_(Identifier("Setup")), ephemeralTree_(Identifier("AppStateNotStored"))
{
}

juce::ValueTree & Data::get()
{
	return tree_;
}

juce::ValueTree& Data::getEphemeral()
{
	return ephemeralTree_;
}

void Data::initializeFromSettings()
{
	auto xmlDoc = Settings::instance().get("ClientSettings", "");
	auto topLevel = juce::parseXML(xmlDoc);
	if (topLevel) {
		tree_ = ValueTree::fromXml(*topLevel);
	}
}

void Data::saveToSettings()
{
	Settings::instance().set("ClientSettings", tree_.toXmlString().toStdString());
}

#include "Settings.h"

#include "StreamLogger.h"

juce::String Settings::settingsID_ = "JammerNetz"; // This can be overridden for testing, so you can start more than one client on a single machine and don't overwrite your settings file

std::unique_ptr<Settings> Settings::instance_;

Settings & Settings::instance()
{
	if (!instance_) {
		instance_ = std::make_unique<Settings>();
	}
	return *instance_;
}

void Settings::shutdown()
{
	instance_.reset();
}

void Settings::setSettingsID(String const &id)
{
	settingsID_ = id;
}

std::string Settings::get(std::string const &key, std::string const &defaultValue) 
{
	return properties_.getUserSettings()->getValue(key, defaultValue).toStdString();
}

void Settings::set(std::string const &key, std::string const &value)
{
	properties_.getUserSettings()->setValue(key, String(value));
	properties_.getUserSettings()->setNeedsToBeSaved(true);
}

bool Settings::keyIsSet(std::string const &key) 
{
	return properties_.getUserSettings()->containsKey(key);
}

File Settings::getSessionStorageDir() const
{
	auto dir = File(File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() + "/JammerNetzSession");
	if (!dir.exists()) {
		dir.createDirectory();
	}
	else {
		if (dir.existsAsFile()) {
			jassert(false);
			StreamLogger::instance() << "Can't record to " << dir.getFullPathName() << " as it is a file and not a directory!" << std::endl;
			return File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName();
		}
	}
	return dir;
}

Settings::Settings()
{
	PropertiesFile::Options options;
	options.folderName = settingsID_;
	options.applicationName = settingsID_;
	options.filenameSuffix = ".settings";
	options.commonToAllUsers = false;
	options.osxLibrarySubFolder = "Application Support"; // Required to avoid an assert in JUCE
	properties_.setStorageParameters(options);
}

void Settings::saveAndClose()
{
	properties_.closeFiles();
}

void Settings::flush()
{
	properties_.saveIfNeeded();
}


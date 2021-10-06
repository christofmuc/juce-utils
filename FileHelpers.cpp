/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "FileHelpers.h"

juce::File getOrCreateSubdirectory(File dir, String const &name)
{
	File subdir = dir.getChildFile(name);
	if (!subdir.exists()) {
		subdir.createDirectory();
	}
	return subdir;
}

int FileDateComparatorOldestFirst::compareElements(File const &first, File const &second)
{
	if (first.getLastModificationTime() < second.getLastModificationTime()) return -1;
	if (first.getLastModificationTime() > second.getLastModificationTime()) return 1;
	return 0;
}

int FileDateComparatorNewestFirst::compareElements(File const &first, File const &second)
{
	if (first.getLastModificationTime() < second.getLastModificationTime()) return 1;
	if (first.getLastModificationTime() > second.getLastModificationTime()) return -1;
	return 0;
}

TemporaryDirectory::TemporaryDirectory()
{
	File tempDir = File::getSpecialLocation(File::tempDirectory);
	dir_ = tempDir.getNonexistentChildFile("knobkraft", "tmp");
	dir_.createDirectory();
}

TemporaryDirectory::~TemporaryDirectory() {
	dir_.deleteRecursively(false);
}

juce::File TemporaryDirectory::asFile()
{
	return dir_;
}

std::string TemporaryDirectory::name()
{
	return dir_.getFullPathName().toStdString();
}

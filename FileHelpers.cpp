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

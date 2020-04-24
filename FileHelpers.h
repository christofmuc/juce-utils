/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

//! Do get the file for the subdirectory, if it doesn't exist create a directory and return it
File getOrCreateSubdirectory(File dir, String const &name);

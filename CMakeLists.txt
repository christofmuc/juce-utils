#
#  Copyright (c) 2019 Christof Ruch. All rights reserved.
#
#  Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
#

project(juce-utils)

# Find JUCE, and add the components we need
IF(NOT DEFINED JUCE_LIBRARIES)
	SET(JUCE_DIR_HINT "${CMAKE_CURRENT_LIST_DIR}/../thirdparty")
	find_package(JUCE REQUIRED 
		COMPONENTS 
			juce_core		
			juce_events
			juce_audio_basics
			juce_audio_devices
			juce_data_structures
	)
ENDIF()

# Define the sources for the static library
set(Sources
    Data.cpp
	Data.h
	DebounceTimer.cpp DebounceTimer.h
	Logger.cpp
	Logger.h
	MidiBankNumber.cpp MidiBankNumber.h
	MidiClocker.cpp
	MidiClocker.h
	MidiHelpers.cpp 
	MidiHelpers.h
	MidiNetworkLocation.h
	MidiNote.cpp
	MidiNote.h
	MidiProgramNumber.cpp MidiProgramNumber.h
	MidiRecorder.cpp
	MidiRecorder.h
	MidiTuning.cpp MidiTuning.h
	PackedDataFormatInfo.cpp PackedDataFormatInfo.h
	Settings.cpp
	Settings.h
	StreamLogger.cpp
	StreamLogger.h
	Sysex.cpp
	Sysex.h
	TypedNamedValue.h
	README.md
	LICENSE.md
)

# Setup library
add_library(juce-utils ${Sources})
target_include_directories(juce-utils INTERFACE ${CMAKE_CURRENT_LIST_DIR} PRIVATE ${JUCE_INCLUDES})

# Pedantic about warnings
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(juce-utils PRIVATE /W4 /WX)
else()
    # lots of warnings and all warnings as errors
    #target_compile_options(juce-utils PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

#
#  Copyright (c) 2019 Christof Ruch. All rights reserved.
#
#  Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
#

project(juce-utils)

# We need a library called juce-static defined providing JUCE 6

# Define the sources for the static library
set(Sources
	AudioRecorder.cpp AudioRecorder.h 
    Data.cpp Data.h
	DebounceTimer.cpp DebounceTimer.h
	FileHelpers.cpp FileHelpers.h
	JuceHeader.h
	Logger.cpp Logger.h
	MidiBankNumber.cpp MidiBankNumber.h
	MidiChannel.h
	MidiClocker.cpp MidiClocker.h
	MidiHelpers.cpp MidiHelpers.h
	MidiNetworkLocation.h
	MidiNote.cpp MidiNote.h
	MidiProgramNumber.cpp MidiProgramNumber.h
	MidiRecorder.cpp MidiRecorder.h
	MidiTuning.cpp MidiTuning.h
	PackedDataFormatInfo.cpp PackedDataFormatInfo.h
	ProgressHandler.h
	Settings.cpp Settings.h
	StreamLogger.cpp StreamLogger.h
	Sysex.cpp Sysex.h
	TypedNamedValue.cpp TypedNamedValue.h 
	WaitForEvent.cpp WaitForEvent.h
	README.md
	LICENSE.md
)

# Setup library
add_library(juce-utils ${Sources})
target_include_directories(juce-utils INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(juce-utils PUBLIC juce-static)

# Pedantic about warnings
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(juce-utils PRIVATE /W4 /WX)
else()
    # lots of warnings and all warnings as errors
    #target_compile_options(juce-utils PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

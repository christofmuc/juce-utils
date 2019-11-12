#
#  Copyright (c) 2019 Christof Ruch. All rights reserved.
#
#  Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
#

# Find JUCE, and add the components we need
SET(JUCE_DIR_HINT "${CMAKE_CURRENT_LIST_DIR}/../thirdparty")
find_package(JUCE REQUIRED 
	COMPONENTS 
		juce_core		
		juce_events
		juce_audio_basics
		juce_audio_devices
		juce_data_structures
)

# Define the sources for the static library
set(Sources
    Data.cpp
	Data.h
	MidiClocker.cpp
	MidiClocker.h
	MidiNote.cpp
	MidiNote.h
	MidiRecorder.cpp
	MidiRecorder.h
	Settings.cpp
	Settings.h
	StreamLogger.cpp
	StreamLogger.h
	README.md
	LICENSE.md
)

# Setup library
add_library(JUCE-MIDI-Helpers ${Sources})
target_include_directories(JUCE-MIDI-Helpers INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(JUCE-MIDI-Helpers PRIVATE ${JUCE_LIBRARIES})

# Pedantic about warnings
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(JUCE-MIDI-Helpers PRIVATE /W4 /WX)
else()
    # lots of warnings and all warnings as errors
    target_compile_options(JUCE-MIDI-Helpers PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

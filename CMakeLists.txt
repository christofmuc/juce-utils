#[[
MIT License

Copyright (c) 2019-2021 Christof Ruch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
]]

project(juce-utils)

# We need a library called juce-static defined providing JUCE 6

# Define the sources for the static library
set(Sources
	AudioRecorder.cpp AudioRecorder.h
    Data.cpp Data.h
	DebounceTimer.cpp DebounceTimer.h
	FileHelpers.cpp FileHelpers.h
	I18NHelper.cpp I18NHelper.h
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
	RingBuffer.cpp RingBuffer.h
	RunWithRetry.cpp RunWithRetry.h
	Settings.cpp Settings.h
	StreamLogger.cpp StreamLogger.h
	Sysex.cpp Sysex.h
	TypedNamedValue.cpp TypedNamedValue.h
	WaitForEvent.cpp WaitForEvent.h
	README.md
	LICENSE.md
)

option(GETTEXT_AVAILABLE "Set this to true if package gettext is available and you want to build the I18NHelpers" OFF)

# Setup library
add_library(juce-utils ${Sources})

if (GETTEXT_AVAILABLE)
	target_compile_definitions(juce-utils PUBLIC GETTEXT_AVAILABLE)
	target_compile_definitions(juce-utils PUBLIC USE_GETTEXT_TEXT_DOMAIN="digitalStagePC")
	if(VCPKG_INSTALLED_DIR)
		target_include_directories(juce-utils PUBLIC "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
		target_link_directories(juce-utils PUBLIC "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug>/lib")
	endif()
	if(MSVC)
		set(I18NLib intl)
	else()
		set(I18NLibs "intl iconv")
	endif()
endif()

target_include_directories(juce-utils INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(juce-utils PUBLIC ${I18NLib} juce-static)

# Pedantic about warnings and precompiled headers
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(juce-utils PRIVATE /W4 /WX)
	if(REALLY_USE_PRECOMPILED_HEADERS)
		target_precompile_headers(juce-utils 
			PRIVATE
				"JuceHeader.h"
				<string>
			)
	endif()
else()
    # lots of warnings and all warnings as errors
    #target_compile_options(juce-utils PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

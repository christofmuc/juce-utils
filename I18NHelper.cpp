/*
 * MIT License
 *
 * Copyright (c) 2019-2022 Christof Ruch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "I18NHelper.h"

#include "JuceHeader.h"

#include "Logger.h"

#include <stdlib.h>

#ifndef USE_GETTEXT_TEXT_DOMAIN
#define USE_GETTEXT_TEXT_DOMAIN "juceutils"
#endif

#ifdef GETTEXT_AVAILABLE

File gLocalePath;

juce::String U8(const char* translatedString)
{
    if (translatedString != nullptr) {
        return juce::String::fromUTF8(translatedString);
    }
    else {
        jassertfalse;
        return "<missing_trans>";
    }
}

void globalSetupLocale()
{
	File executablePath = File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile).getParentDirectory();
	gLocalePath = executablePath.getChildFile("share");
	// Normal case - the share directory with the gmo files is a subdirectory of the executable directory
	if (!gLocalePath.exists()) {
		// Special case - if we are building with a multi-config generator and are running a development version, the share path could be one directory up!
		File alternativeLocalePath = executablePath.getParentDirectory().getChildFile("share");
		if (!alternativeLocalePath.exists()) {
			SimpleLogger::instance()->postMessage("Translation files not found at " + gLocalePath.getFullPathName() + ", turning off translations!");
			return;
		}
		gLocalePath = alternativeLocalePath;
	}
	auto result = bindtextdomain(USE_GETTEXT_TEXT_DOMAIN, gLocalePath.getFullPathName().getCharPointer());
	SimpleLogger::instance()->postMessage("Bindtext domain gave us " + String(result));
#if defined(WIN32) || defined(__APPLE__)
	auto displayLocale = juce::SystemStats::getDisplayLanguage().getCharPointer();
    bind_textdomain_codeset(USE_GETTEXT_TEXT_DOMAIN, "utf-8");
#else
    auto displayLocale = ::setlocale (LC_MESSAGES, "");
#endif
	switchDisplayLanguage(displayLocale);
}

void switchDisplayLanguage(const char* languageID)
{
	const char* localeToSet;

#if defined(WIN32) || defined(__APPLE__)
	// Windows famously ignores the setlocale, but rather insists on reading the environment. Additionally, Windows uses de-DE and not de_DE like Posix.
	// We discard country specific language at this point to keep it simple
	std::string cleanupId(languageID);
	std::replace(cleanupId.begin(), cleanupId.end(), '-', '_');
	localeToSet = cleanupId.c_str();
#ifdef WIN32
	_putenv_s("LC_ALL", localeToSet);
#else
    setenv("LANG", localeToSet, 1);
#endif
#else
	::setlocale(LC_MESSAGES, languageID);
    localeToSet = languageID;
#endif

	// Make sure there is a directory of that ID
	if (!gLocalePath.getChildFile(localeToSet).exists()) {
		SimpleLogger::instance()->postMessage("User locale " + String(localeToSet) + " requested but no matching directory at " + gLocalePath.getFullPathName());
	}

	textdomain(USE_GETTEXT_TEXT_DOMAIN);
	SimpleLogger::instance()->postMessage("Setting user language to " + String(languageID) + " reported to " + String(localeToSet));
}

#endif

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

#ifdef WIN32
#include <windows.h>
#include <WinNls.h>
#endif

#ifndef USE_GETTEXT_TEXT_DOMAIN
#define USE_GETTEXT_TEXT_DOMAIN "juceutils"
#endif

#ifdef GETTEXT_AVAILABLE

void globalSetupLocale()
{
	File executablePath = File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile).getParentDirectory();
	File localePath = executablePath.getChildFile("share");
	// Normal case - the share directory with the gmo files is a subdirectory of the executable directory
	if (localePath.exists()) {
	}
	else {
		// Special case - if we are building with a multi-config generator and are running a development version, the share path could be one directory up!
		File alternativeLocalePath = executablePath.getParentDirectory().getChildFile("share");
		if (!alternativeLocalePath.exists()) {
			SimpleLogger::instance()->postMessage("Translation files not found at " + localePath.getFullPathName() + ", turning off translations!");
			return;
		}
		localePath = alternativeLocalePath;
	}
	auto result = bindtextdomain(USE_GETTEXT_TEXT_DOMAIN, localePath.getFullPathName().getCharPointer());
	SimpleLogger::instance()->postMessage("Bindtext domain gave us " + String(result));
	auto displayLocale = juce::SystemStats::getDisplayLanguage();
	displayLocale = "de";
	switchDisplayLanguage(displayLocale.getCharPointer());
}

void switchDisplayLanguage(const char* languageID)
{
#ifdef WIN32
//::SetThreadLocale(MAKELCID(languageID, SORT_DEFAULT));
	auto localeSet = setlocale(LC_ALL, "");
#else
	auto localeSet = setlocale(LC_ALL, languageID);
#endif
	textdomain(USE_GETTEXT_TEXT_DOMAIN);
	SimpleLogger::instance()->postMessage("Setting user language to " + String(languageID) + " reported to " + String(localeSet));
}

#endif

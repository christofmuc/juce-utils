/*
 * MIT License
 *
 * Copyright (c) 2019-2023 Christof Ruch
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

#include <juce_core/juce_core.h>

#include "SpdLogJuce.h"
#include <spdlog/spdlog.h>

#include <stdlib.h>

#ifndef USE_GETTEXT_TEXT_DOMAIN
#define USE_GETTEXT_TEXT_DOMAIN "juceutils"
#endif

#ifdef GETTEXT_AVAILABLE

juce::File gLocalePath;

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
    juce::File executablePath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile).getParentDirectory();
    gLocalePath = executablePath.getChildFile("share");
    // Normal case - the share directory with the gmo files is a subdirectory of the executable directory
    if (!gLocalePath.exists()) {
        // Special case - if we are building with a multi-config generator and are running a development version, the share path could be one directory up!
        juce::File alternativeLocalePath = executablePath.getParentDirectory().getChildFile("share");
        if (!alternativeLocalePath.exists()) {
            spdlog::warn("Translation files not found at {}, turning off translations!", gLocalePath.getFullPathName());
            return;
        }
        gLocalePath = alternativeLocalePath;
    }
    auto result = bindtextdomain(USE_GETTEXT_TEXT_DOMAIN, gLocalePath.getFullPathName().getCharPointer());
    spdlog::debug("Bindtext domain gave us {}", juce::String(result));
#if defined(WIN32) || defined(__APPLE__)
    std::string displayLocale = juce::SystemStats::getDisplayLanguage().toStdString();
#else
    std::string displayLocale = ::setlocale(LC_MESSAGES, "");
#endif
    bind_textdomain_codeset(USE_GETTEXT_TEXT_DOMAIN, "utf-8");
    switchDisplayLanguage(displayLocale.c_str());
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
    auto returnValue = ::setlocale(LC_MESSAGES, languageID);
    if (returnValue == nullptr) {
        spdlog::debug("User locale {} requested but error returned", juce::String(languageID));
    }
    localeToSet = languageID;
#endif

    // Make sure there is a directory of that ID
    if (!gLocalePath.getChildFile(localeToSet).exists()) {
        spdlog::debug("User locale {}" + +" requested but no matching directory at {}", juce::String(localeToSet), gLocalePath.getFullPathName());
    }

    textdomain(USE_GETTEXT_TEXT_DOMAIN);
    spdlog::debug("Setting user language to {} reported to {}", juce::String(languageID), juce::String(localeToSet));
}

#endif

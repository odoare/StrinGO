/*
  ==============================================================================

    FactoryPresets.h
    Created: 21 Sep 2025 10:00:00am
    Author:  Olivier Doaré

    Part of Image-In project

    Licenced under the LGPLv3

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>

namespace FactoryPresets
{
    // Helper to hold preset information. Using juce::String for the name
    // makes it more flexible than a raw char pointer.
    struct Preset
    {
        juce::String name;
        const char* data;
        int dataSize;
    };

    // This function now dynamically builds the preset list from BinaryData
    static juce::Array<Preset> getAvailablePresets()
    {
        juce::Array<Preset> presets;

        for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
        {
            juce::String filename(BinaryData::originalFilenames[i]);
            // Check if the file is an XML file inside the Presets/XML directory
            if (filename.endsWithIgnoreCase(".xml"))
            {
                // Create a user-friendly name from the filename (e.g., "DeepDive.xml" -> "Deep Dive")
                auto friendlyName = filename.upToLastOccurrenceOf(".", false, false)
                                            .replaceCharacter('_', ' ');

                int dataSize = 0;
                const char* resourceData = BinaryData::getNamedResource(BinaryData::namedResourceList[i], dataSize);

                presets.add (Preset { friendlyName, resourceData, dataSize });
            }
        }
        return presets;
    }

} // namespace FactoryPresets

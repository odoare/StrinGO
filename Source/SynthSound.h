/*
  ==============================================================================

    SynthSound.h
    Created: 8 Jan 2024 8:48:39pm
    Author:  od

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
  bool appliesToNote(int midiNoteNumber) override {return midiNoteNumber<73;}
  bool appliesToChannel(int midiChannel) override {return true;}

};

/*
  ==============================================================================

    CracksGenerator.h
    Created: 18 Jan 2024 3:43:48pm
    Author:  od

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CracksGenerator
{
private:
  // Counter incremented at each call of next sample to update crack probability
  int normalizedDensity;

  float lastValue = -1.f;

  juce::Random rnd;

public:

  // Density of clicks as number of clicks per minute
  int density;
  int sampleRateMin;

  CracksGenerator(/* args */);
  ~CracksGenerator();

  void prepare(juce::dsp::ProcessSpec spec);
  void reset();
  float nextSample();

};
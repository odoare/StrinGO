/*
  ==============================================================================

    OneShotSampler.h
    Created: 19 Jan 2024 12:56:27pm
    Author:  od

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SampleArrays.h"
#include <iostream>

class OneShotSampler
{
private:
  /* data */
  float wave[ARRAYSIZE];
  float sampleRate;
  float referenceFrequency;
  float playingFrequency;
  float filterFreqFactor { 5.0 };
  float level { 1.0 };
  int currentWaveNumber;

  float getSampleAtPos(float pos);

  juce::dsp::IIR::Filter<float> filter;

public:
  OneShotSampler();
  ~OneShotSampler();
  void prepare(juce::dsp::ProcessSpec spec);
  void setWave(float *wav);
  void setWaveByNumber(int waveNumber);
  void start();
  void stop();
  void setFilterFreq(float freq);
  void setLevel(float lvl);
  void setReferenceFrequency(float freq);
  void setPlayingFrequency(float freq);
  float processNextSample();
  int stringNum;

  float position { 0.f };
  float increment;
  bool isRunning { false };

};


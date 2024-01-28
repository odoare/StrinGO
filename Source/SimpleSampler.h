/*
  ==============================================================================

    SimpleSampler.h
    Created: 19 Jan 2024 12:56:27pm
    Author:  od

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SampleArrays.h"
#include <iostream>

class SimpleSampler
{
private:
  /* data */
  float* wave;
  float sampleRate;
  float referenceFrequency;
  float playingFrequency;
  float filterFreqFactor { 5.0 };
  float filterFreqVelocityInfluence {0.f};
  float filterFreqVelocityFactor {1.f};
  float levelVelocityInfluence {0.f};
  float levelVelocityFactor {1.f};
  float level { 1.0 };
  bool loop { false };
  float attack{0.f}, decay{0.f}, sustain{1.f}, release{5.f};
  int currentWaveNumber;

  float getSampleAtPos(float pos);

  juce::dsp::IIR::Filter<float> filter;
  juce::ADSR adsr;

public:
  SimpleSampler();
  ~SimpleSampler();
  void prepare(juce::dsp::ProcessSpec spec);
  void setWave(float *wav);
  void setWaveByNumber(int waveNumber);
  void start();
  void stop();
  void setFilterFreqFactor(float freq);
  void setFilterFreqVelocityInfluence(float val);
  void setLevelVelocityInfluence(float val);
  void setVelocity(float vel);
  void setLevel(float lvl);
  void setReferenceFrequency(float freq);
  void setPlayingFrequency(float freq);
  void setLooping(bool l);
  void setAttack(float a);
  void setDecay(float d);
  void setSustain(float s);
  void setRelease(float r);
  float processNextSample();
  int stringNum;

  float position { 0.f };
  float increment;
  bool isRunning { false };

};

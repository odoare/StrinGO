/*
  ==============================================================================

    synthVoice.h
    Created: 8 Jan 2024 8:47:38pm
    Author:  od

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "StringReso.h"
#include "CracksGenerator.h"
// #include "SimpleSampler.h"

#define INVPI 0.318309886183791

//#define DEBUG

class SynthVoice : public juce::SynthesiserVoice
{
public:

  bool canPlaySound (juce::SynthesiserSound* sound) override;

  void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
 	//Called to start a new note.
 
  void stopNote (float velocity, bool allowTailOff) override;
  // Called to stop a note.
 
  void pitchWheelMoved (int newPitchWheelValue) override;
 	// Called to let the voice know that the pitch wheel has been moved.
 
  void controllerMoved (int controllerNumber, int newControllerValue) override;
 	// Called to let the voice know that a midi controller has been moved.

  void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
 
  void renderNextBlock (juce::AudioBuffer< float > &buffer, int startSample, int numSamples) override;

  void setNoiseFilterFreq(float freq);
  void setNoiseFilterVelocityFreqInfluence(float factor);
  void setVelocity(float vel);
  void setNoiseLevel(float lvl);

  void setSamplerLevel(float lvl);
  void setSamplerFilterFreqFactor(float freqFactor);

  void setCrackDensity(int d);
  void setCrackFilterFreq(float freq);
  void setCrackLevel(float lvl);

  StringReso stringReso;
  juce::ADSR adsr1, adsr2, adsrN, adsrC;//, adsrO;
  juce::ADSR::Parameters adsr1Params, adsr2Params, adsrNParams, adsrCParams, adsrOParams;

private:

  juce::dsp::ProcessSpec processSpec;

  juce::Random randomNoise;

  juce::dsp::IIR::Filter<float> noiseFilter;
  float noiseFilterFreq  { 1000.f };
  float noiseLevel { 1.0f };
  float noiseFilterVelocityFreqInfluence;

  // OneShotSampler sampler;
  // float samplerLevel { 1.0 };
  // float samplerFilterFreqFactor {5.0} ;

  CracksGenerator cracksGenerator;
  juce::dsp::IIR::Filter<float> crackFilter;
  float crackFilterFreq  { 1000.f };
  float crackLevel { 1.0 };

  juce::AudioBuffer<float> inBuffer, synthBuffer;

  bool isPrepared { false };
};

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

#define INVPI 0.318309886183791

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

  void prepareToPlay (juce::AudioBuffer<float> *sharedInputBuffer, double sampleRate, int samplesPerBlock, int outputChannels);
 
  void renderNextBlock (juce::AudioBuffer< float > &buffer, int startSample, int numSamples) override;

  void setNoiseLPFilterFreq(float freq);
  void setNoiseLPFilterFreqVelocityInfluence(float factor);
  void setNoiseHPFilterFreq(float freq);
  void setNoiseHPFilterFreqVelocityInfluence(float factor);
  void setNoiseLevel(float lvl);
  void setNoiseLevelVelocityInfluence(float val);
  void setVelocity(float vel);

  void setCrackDensity(int d);
  void setCrackLPFilterFreq(float freq);
  void setCrackLPFilterFreqVelocityInfluence(float val);
  void setCrackLevel(float lvl);
  void setCrackLevelVelocityInfluence(float val);

  StringReso stringReso;
  juce::ADSR adsr1, adsr2, adsrN, adsrC;//, adsrO;
  juce::ADSR::Parameters adsr1Params, adsr2Params, adsrNParams, adsrCParams, adsrOParams;

  juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> smoothInputGain;

private:

  juce::dsp::ProcessSpec processSpec;

  juce::Random randomNoise;

  juce::dsp::IIR::Filter<float> noiseLPFilter;
  juce::dsp::IIR::Filter<float> noiseHPFilter;
  float noiseLPFilterFreq  { 1000.f };
  float noiseHPFilterFreq  { 1000.f };
  float noiseLevel { 1.0f };
  float noiseLPFilterFreqVelocityInfluence {0.f};
  float noiseLPFilterFreqVelocityFactor {1.f};
  float noiseHPFilterFreqVelocityInfluence {0.f};
  float noiseHPFilterFreqVelocityFactor {1.f};  
  float noiseLevelVelocityInfluence {0.f};
  float noiseLevelVelocityFactor {1.f};

  CracksGenerator cracksGenerator;
  juce::dsp::IIR::Filter<float> crackLPFilter;
  float crackLPFilterFreq  { 1000.f };
  float crackLevel { 1.0 };
  float crackLPFilterFreqVelocityInfluence {0.f};
  float crackLPFilterFreqVelocityFactor {1.f};
  float crackLevelVelocityInfluence {0.f};
  float crackLevelVelocityFactor {1.f};

  juce::AudioBuffer<float> inBuffer, synthBuffer;
  juce::AudioBuffer<float> *sharedBuffer;

  bool isPrepared { false };
};

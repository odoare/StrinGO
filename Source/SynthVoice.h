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

  void setVelocity(float vel);

  StringReso stringReso;
  juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> smoothInputGain;

private:

  juce::dsp::ProcessSpec processSpec;
  juce::AudioBuffer<float> inBuffer, synthBuffer;
  juce::AudioBuffer<float> *sharedBuffer;
  bool isPrepared { false };
};

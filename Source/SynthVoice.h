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

private:

  juce::ADSR adsr1, adsr2;
  juce::ADSR::Parameters adsr1Params, adsr2Params;

  juce::Random randomNoise;

  juce::AudioBuffer<float> synthBuffer;

  juce::dsp::Oscillator<float> osc { [](float x) {return std::sin(x); }};
  juce::dsp::Gain<float> gain;

  StringReso stringReso;

  bool isPrepared { false };
};

/*
  ==============================================================================

    synthVoice.cpp
    Created: 8 Jan 2024 8:47:38pm
    Author:  od

  ==============================================================================
*/

#include "SynthVoice.h"
#include <iostream>

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
  return dynamic_cast<juce::SynthesiserSound*>(sound)!=nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
  float noteFreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
  std::cout << "begin start_Note " << midiNoteNumber << " at frequency " << noteFreq << "Hz" << std::endl;
  // if (noteFreq < stringReso.maxFreq)
  // {
    // osc.setFrequency (juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber), true);
    adsr1.noteOn();
    adsr2.noteOn();
    stringReso.setStringFreq(noteFreq);
    stringReso.setIsOn(true);
    stringReso.getParams();
    noiseFilterFreq = noteFreq*5.f;
    noiseFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(stringReso.processSpec.sampleRate,noiseFilterFreq);

    std::cout << "end start_Note" << std::endl;
  // }
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
  std::cout << "begin stop_Note   " << std::endl;

  adsr1.noteOff();
  adsr2.noteOff();
  stringReso.setIsOn(false);
  stringReso.getParams();

  // if (!allowTailOff || !adsr1.isActive())
  //   clearCurrentNote();

  std::cout << "end stop_Note" << std::endl;
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{

}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{

}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{

  std::cout << "Begin SynthVoice::prepareToPLay   "; 

  adsr1.setSampleRate (sampleRate);
  adsr2.setSampleRate (sampleRate);

  processSpec.maximumBlockSize = samplesPerBlock;
  processSpec.sampleRate = sampleRate;
  processSpec.numChannels = outputChannels;

  osc.prepare(processSpec);
  gain.prepare(processSpec);

  osc.setFrequency(220.f);
  gain.setGainLinear(0.01f);

  adsr1Params.attack = 0.01f;
  adsr1Params.decay = 0.01f;
  adsr1Params.sustain = 1.f;
  adsr1Params.release = 10.f;
  adsr1.setParameters(adsr1Params);

  adsr2Params.attack = 0.01f;
  adsr2Params.decay = 0.01f;
  adsr2Params.sustain = 0.f;
  adsr2Params.release = 0.1f;
  adsr2.setParameters(adsr2Params);

  stringReso.prepare(processSpec, 10.f);

  noiseFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,noiseFilterFreq);

  isPrepared = true;

  std::cout << "End SynthVoice::prepareToPLay   " << std::endl;
}

void SynthVoice::setNoiseFilterFreq(float freq)
{
  if (freq!=noiseFilterFreq)
  {
    noiseFilterFreq = freq;
    noiseFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,noiseFilterFreq);
  }
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &buffer, int startSample, int numSamples)
{
  jassert(isPrepared);

  // if (!isVoiceActive())
  //   return;
  
  inBuffer.setSize(1, numSamples, false, false, true);
  inBuffer.clear();

  synthBuffer.setSize(1, numSamples, false, false, true);
  synthBuffer.clear();

  for (int channel=0; channel<inBuffer.getNumChannels(); ++channel)
  {
    auto* channelData = inBuffer.getWritePointer (channel);
    for (int sample=0; sample<numSamples; ++sample)
      channelData[sample]=noiseFilter.processSample(randomNoise.nextFloat()-0.5f);
  }
  adsr2.applyEnvelopeToBuffer(inBuffer, 0, inBuffer.getNumSamples());
  
  stringReso.process(inBuffer, synthBuffer, 0, numSamples);
  //adsr1.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
  for (int channel=0; channel<buffer.getNumChannels(); ++channel)
  {
    buffer.addFrom(channel,startSample, synthBuffer, 0, 0, numSamples);

    // if (!adsr1.isActive())
    //   clearCurrentNote();

  }

}

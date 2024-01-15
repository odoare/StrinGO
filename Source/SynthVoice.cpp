/*
  ==============================================================================

    synthVoice.cpp
    Created: 8 Jan 2024 8:47:38pm
    Author:  od

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
  return dynamic_cast<juce::SynthesiserSound*>(sound)!=nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
  //osc.setFrequency (juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber), true);
  adsr1.noteOn();
  adsr2.noteOn();
  stringReso.setStringFreq(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
  stringReso.setIsOn(true);
  stringReso.getParams();
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
  adsr1.noteOff();
  adsr2.noteOff();
  stringReso.setIsOn(false);

  stringReso.getParams();

  if (!allowTailOff || !adsr1.isActive())
    clearCurrentNote();

}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{

}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{

}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{

  adsr1.setSampleRate (sampleRate);
  adsr2.setSampleRate (sampleRate);

  juce::dsp::ProcessSpec spec;
  spec.maximumBlockSize = samplesPerBlock;
  spec.sampleRate = sampleRate;
  spec.numChannels = outputChannels;

  osc.prepare(spec);
  gain.prepare(spec);

  osc.setFrequency(220.f);
  gain.setGainLinear(0.01f);

  adsr1Params.attack = 0.01f;
  adsr1Params.decay = 0.01f;
  adsr1Params.sustain = 1.f;
  adsr1Params.release = 5.f;
  adsr1.setParameters(adsr1Params);

  adsr2Params.attack = 0.01f;
  adsr2Params.decay = 0.5f;
  adsr2Params.sustain = 0.f;
  adsr2Params.release = 0.1f;
  adsr2.setParameters(adsr2Params);

  stringReso.prepare(spec, 10.f);
  
  isPrepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &buffer, int startSample, int numSamples)
{
  jassert(isPrepared);

  if (!isVoiceActive())
    return;

  juce::AudioBuffer<float> inBuffer(buffer.getNumChannels(),numSamples);
  inBuffer.clear();

  synthBuffer.setSize(buffer.getNumChannels(), numSamples, false, false, true);
  synthBuffer.clear();

  for (int channel=0; channel<inBuffer.getNumChannels(); ++channel)
  {
    auto* channelData = inBuffer.getWritePointer (channel);
    for (int sample=0; sample<numSamples; ++sample)
      channelData[sample]=randomNoise.nextFloat()-0.5f;
  }
  adsr2.applyEnvelopeToBuffer(inBuffer, 0, inBuffer.getNumSamples());
  
  //juce::dsp::AudioBlock<float> inBlock(outputBuffer, outputBuffer.getNumChannels(), startSample, numSamples);
  // juce::dsp::AudioBlock<float> synthBlock(synthBuffer);

  // for (int channel=0; channel<outputBuffer.getNumChannels(); ++channel)
  // {
  //   synthBuffer.addFrom(channel,0,outputBuffer,channel,startSample,numSamples,1.f);
  // }
  // juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
  // osc.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
  // gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));

  // adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
  // Apply dsp to synthBuffer
  stringReso.process(inBuffer, synthBuffer, startSample, numSamples);
  adsr1.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
  for (int channel=0; channel<buffer.getNumChannels(); ++channel)
  {
    buffer.addFrom(channel,startSample, synthBuffer, channel, 0, numSamples);

    if (!adsr1.isActive())
      clearCurrentNote();

  }

}

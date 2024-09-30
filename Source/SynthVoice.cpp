/*
  ==============================================================================

    synthVoice.cpp
    Created: 8 Jan 2024 8:47:38pm
    Author:  od

  ==============================================================================
*/

#include "SynthVoice.h"

#ifdef DEBUG
  #include <iostream>
#endif

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
  return dynamic_cast<juce::SynthesiserSound*>(sound)!=nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
  float noteFreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

  #ifdef DEBUG
    std::cout << "begin start_Note " << midiNoteNumber << " at frequency " << noteFreq << "Hz" << std::endl;
  #endif

  setVelocity(velocity);

  stringReso.setStringFreq(noteFreq);
  stringReso.setIsOn(true);

  #ifdef DEBUG
    std::cout << "end start_Note" << std::endl;
  #endif
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{

  #ifdef DEBUG
    std::cout << "begin stop_Note   " << std::endl;
  #endif

  stringReso.setIsOn(false);

  // if (!allowTailOff || !adsr1.isActive())
  //   clearCurrentNote();

  #ifdef DEBUG
    std::cout << "end stop_Note" << std::endl;
  #endif
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{

}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{

}

void SynthVoice::prepareToPlay (juce::AudioBuffer<float> *sharedInputBuffer, double sampleRate, int samplesPerBlock, int outputChannels)
{

  #ifdef DEBUG
    std::cout << "Begin SynthVoice::prepareToPLay   "; 
  #endif

  sharedBuffer = sharedInputBuffer;
  
  processSpec.maximumBlockSize = samplesPerBlock;
  processSpec.sampleRate = sampleRate;
  processSpec.numChannels = outputChannels;

  stringReso.prepare(processSpec, 10.f);

  smoothInputGain.reset(0.01f*sampleRate);
  smoothInputGain.setCurrentAndTargetValue(0.f);

  isPrepared = true;

  #ifdef DEBUG
    std::cout << "End SynthVoice::prepareToPLay   " << std::endl;
  #endif
}

void SynthVoice::setVelocity(float vel)
{
  stringReso.setVelocity(vel);
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &buffer, int startSample, int numSamples)
{
  jassert(isPrepared);

  // if (!isVoiceActive())
  //   return;
  
  inBuffer.setSize(1, numSamples, false, false, true);
  //inBuffer.clear();
  //inBuffer.copyFrom(0,0,buffer,0,startSample,numSamples);

  synthBuffer.setSize(2, numSamples, false, false, true);
  //synthBuffer.clear();

  for (int channel=0; channel<inBuffer.getNumChannels(); ++channel)
  {
    auto* channelData = inBuffer.getWritePointer (channel);
    auto* sharedData = sharedBuffer->getReadPointer(channel);
    for (int sample=0; sample<numSamples; ++sample)
      channelData[sample] = smoothInputGain.getNextValue()*sharedData[startSample+sample];
  }
  
  stringReso.process(inBuffer, synthBuffer, 0, numSamples);

  for (int channel=0; channel<buffer.getNumChannels(); ++channel)
  {
    buffer.addFrom(channel,startSample, synthBuffer, channel, 0, numSamples);
    //std::cout << "Channel " << channel << "\n";
    // if (!adsr1.isActive())
    //   clearCurrentNote();

  }

}

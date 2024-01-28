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

  #ifdef DEBUG
    std::cout << "begin start_Note " << midiNoteNumber << " at frequency " << noteFreq << "Hz" << std::endl;
  #endif

  adsr1.noteOn();
  adsr2.noteOn();
  adsrN.noteOn();
  // adsrO.noteOn();
  adsrC.noteOn();

  // sampler.setPlayingFrequency(noteFreq);
  // sampler.start();

  // osc.setFrequency(noteFreq);
  
  setVelocity(velocity);

  stringReso.setStringFreq(noteFreq);
  stringReso.setVelocity(velocity);
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

  adsr1.noteOff();
  adsr2.noteOff();
  adsrN.noteOff();
  adsrC.noteOff();

  //sampler.stop();    // if (position<0.f)
    //   std::cout << stringNum << ">N "; 
    // else 
    //   std::cout << stringNum << ">" << position << " ";
    // if (!isRunning) std::cout << "Position " << position << " STOP SAMPLER" << std::endl;
  
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

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{

  #ifdef DEBUG
    std::cout << "Begin SynthVoice::prepareToPLay   "; 
  #endif

  adsr1.setSampleRate (sampleRate);
  adsr2.setSampleRate (sampleRate);
  adsrN.setSampleRate (sampleRate);
  adsrC.setSampleRate (sampleRate);
  
  processSpec.maximumBlockSize = samplesPerBlock;
  processSpec.sampleRate = sampleRate;
  processSpec.numChannels = outputChannels;

  stringReso.prepare(processSpec, 10.f);

  cracksGenerator.prepare(processSpec);

  noiseLPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,noiseLPFilterFreq);
  noiseHPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate,noiseLPFilterFreq);

  isPrepared = true;

  // std::cout << "End SynthVoice::prepareToPLay   " << std::endl;
}

void SynthVoice::setNoiseLPFilterFreq(float freq)
{
  if (freq!=noiseLPFilterFreq)
  {
    noiseLPFilterFreq = freq;
    noiseLPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,noiseLPFilterFreq);
  }
}

void SynthVoice::setNoiseLPFilterFreqVelocityInfluence(float factor)
{
  noiseLPFilterFreqVelocityInfluence = factor;
}

void SynthVoice::setNoiseHPFilterFreq(float freq)
{
  if (freq!=noiseHPFilterFreq)
  {
    noiseHPFilterFreq = freq;
    noiseHPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(processSpec.sampleRate,noiseHPFilterFreq);
  }
}

void SynthVoice::setNoiseHPFilterFreqVelocityInfluence(float factor)
{
  noiseHPFilterFreqVelocityInfluence = factor;
}


void SynthVoice::setVelocity(float vel)
{
  float filterVelocityFreqFactor = juce::jmap<float>(float(vel), 1-noiseHPFilterFreqVelocityInfluence, 1) ;
  noiseHPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,noiseHPFilterFreq*filterVelocityFreqFactor);
}

void SynthVoice::setNoiseLevel(float lvl)
{
  noiseLevel = lvl;
}


void SynthVoice::setCrackDensity(int d)
{
  cracksGenerator.density = d;
}

void SynthVoice::setCrackFilterFreq(float freq)
{
  if (freq!=crackFilterFreq)
  {
    crackFilterFreq = freq;
    crackFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,crackFilterFreq);
  }
}

void SynthVoice::setCrackLevel(float lvl)
{
  crackLevel = lvl;
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
      channelData[sample] = adsrN.getNextSample() * noiseLPFilter.processSample(noiseLevel*(randomNoise.nextFloat()-0.5f))
        + adsrC.getNextSample() * crackFilter.processSample(crackLevel*cracksGenerator.nextSample());
  }
  
  stringReso.process(inBuffer, synthBuffer, 0, numSamples);

  for (int channel=0; channel<buffer.getNumChannels(); ++channel)
  {
    buffer.addFrom(channel,startSample, synthBuffer, 0, 0, numSamples);

    // if (!adsr1.isActive())
    //   clearCurrentNote();

  }

}

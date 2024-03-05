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

  adsrN.noteOn();
  adsrC.noteOn();

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

  adsrN.noteOff();
  adsrC.noteOff();  
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

  adsrN.setSampleRate (sampleRate);
  adsrC.setSampleRate (sampleRate);
  
  processSpec.maximumBlockSize = samplesPerBlock;
  processSpec.sampleRate = sampleRate;
  processSpec.numChannels = outputChannels;

  stringReso.prepare(processSpec, 10.f);

  cracksGenerator.prepare(processSpec);

  noiseLPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,noiseLPFilterFreq);
  noiseHPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate,noiseLPFilterFreq);

  smoothInputGain.reset(0.01f*sampleRate);
  smoothInputGain.setCurrentAndTargetValue(0.f);

  isPrepared = true;

  #ifdef DEBUG
    std::cout << "End SynthVoice::prepareToPLay   " << std::endl;
  #endif
}

void SynthVoice::setNoiseLPFilterFreq(float freq)
{
  if (freq!=noiseLPFilterFreq)
  {
    noiseLPFilterFreq = freq;
    noiseLPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,noiseLPFilterFreq*noiseLPFilterFreqVelocityFactor);
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

void SynthVoice::setVelocity(float vel)
{
  noiseLPFilterFreqVelocityFactor = juce::jmap<float>(float(vel), 1-noiseLPFilterFreqVelocityInfluence, 1) ;
  noiseLPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,noiseLPFilterFreq*noiseLPFilterFreqVelocityFactor);
  noiseLevelVelocityFactor = juce::jmap<float>(float(vel), 1-noiseLevelVelocityInfluence, 1) ;
  // std::cout << "Noise Level Velocity Factor : " << noiseLevelVelocityInfluence << std::endl;

  crackLPFilterFreqVelocityFactor = juce::jmap<float>(float(vel), 1-crackLPFilterFreqVelocityInfluence, 1) ;
  crackLPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,crackLPFilterFreq*crackLPFilterFreqVelocityFactor);
  crackLevelVelocityFactor = juce::jmap<float>(float(vel), 1-crackLevelVelocityInfluence, 1) ;
  // std::cout << "Crack Level Velocity Factor : " << noiseLevelVelocityInfluence << std::endl;

  stringReso.setVelocity(vel);
}

void SynthVoice::setNoiseLevel(float lvl)
{
  noiseLevel = lvl;
}

void SynthVoice::setNoiseLevelVelocityInfluence(float val)
{
  noiseLevelVelocityInfluence = val;
}

void SynthVoice::setCrackDensity(int d)
{
  cracksGenerator.setDensity(d);
}

void SynthVoice::setCrackLPFilterFreq(float freq)
{
  if (freq!=crackLPFilterFreq)
  {
    crackLPFilterFreq = freq;
    crackLPFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,crackLPFilterFreq*crackLPFilterFreqVelocityFactor);
  }
}

void SynthVoice::setCrackLPFilterFreqVelocityInfluence(float val)
{
  crackLPFilterFreqVelocityInfluence = val;
}

void SynthVoice::setCrackLevel(float lvl)
{
  crackLevel = lvl;
}

void SynthVoice::setCrackLevelVelocityInfluence(float val)
{
  crackLevelVelocityInfluence = val;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &buffer, int startSample, int numSamples)
{
  jassert(isPrepared);

  // if (!isVoiceActive())
  //   return;
  
  inBuffer.setSize(1, numSamples, false, false, true);
  inBuffer.clear();
  //inBuffer.copyFrom(0,0,buffer,0,startSample,numSamples);

  synthBuffer.setSize(1, numSamples, false, false, true);
  synthBuffer.clear();

  for (int channel=0; channel<inBuffer.getNumChannels(); ++channel)
  {
    auto* channelData = inBuffer.getWritePointer (channel);
    auto* sharedData = sharedBuffer->getReadPointer(channel);
    for (int sample=0; sample<numSamples; ++sample)
      channelData[sample] = smoothInputGain.getNextValue()*sharedData[startSample+sample] + adsrN.getNextSample() * noiseLevelVelocityFactor * noiseHPFilter.processSample(noiseLPFilter.processSample(noiseLevel*(randomNoise.nextFloat()-0.5f)))
        + adsrC.getNextSample() * crackLevelVelocityFactor * crackLPFilter.processSample(crackLevel*cracksGenerator.nextSample());
  }
  
  stringReso.process(inBuffer, synthBuffer, 0, numSamples);

  for (int channel=0; channel<buffer.getNumChannels(); ++channel)
  {
    buffer.addFrom(channel,startSample, synthBuffer, 0, 0, numSamples);

    // if (!adsr1.isActive())
    //   clearCurrentNote();

  }

}

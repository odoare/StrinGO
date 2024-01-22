/*
  ==============================================================================

    OneShotSampler.cpp
    Created: 19 Jan 2024 12:56:27pm
    Author:  od

  ==============================================================================
*/

#include "SimpleSampler.h"

OneShotSampler::OneShotSampler()
{
}

OneShotSampler::~OneShotSampler()
{
}

void OneShotSampler::prepare(juce::dsp::ProcessSpec spec)
{
  sampleRate = spec.sampleRate;
  referenceFrequency = sampleRate/ARRAYSIZE;
  increment = playingFrequency/(ARRAYSIZE*referenceFrequency);
  setFilterFreqFactor(filterFreqFactor);
  setFilterVelocityFreqFactor(filterVelocityFreqFactor);
}

void OneShotSampler::setWave(float *wav)
{
  wave = wav;
}

void OneShotSampler::setWaveByNumber(int waveNumber)
{
  if (waveNumber!=currentWaveNumber)
  {
    currentWaveNumber = waveNumber;
    setWave(getWave(waveNumber));
  }
}

void OneShotSampler::start()
{

  position = 0.f;
  filter.reset();
  isRunning = true;

  // std::cout << "Start sampler     FREQ = "
  //           << playingFrequency 
  //           << "     Ref freq = " << referenceFrequency
  //           << "     Level = " << level
  //           << "     Increment = " << increment
  //           << "     Position = " << position
  //           << std::endl;
}

void OneShotSampler::stop()
{
  isRunning = false;
  position = 0.f;
  // std::cout << "Stopped" << std::endl;
}

void OneShotSampler::setFilterFreqFactor(float freqFactor)
{
  filterFreqFactor = freqFactor;
}

void OneShotSampler::setFilterVelocityFreqFactor(float factor)
{
  filterVelocityFreq = factor;
}

void OneShotSampler::setVelocity(float vel)
{
  filterVelocityFreqFactor = juce::jmap<float>(float(vel), 1-filterVelocityFreq, 1) ;
  filter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,playingFrequency * filterFreqFactor * filterVelocityFreqFactor);
}


void OneShotSampler::setLevel(float lvl)
{
  level = lvl;
}

void OneShotSampler::setReferenceFrequency(float freq)
{
  referenceFrequency = freq;
  increment = playingFrequency/(ARRAYSIZE*referenceFrequency);
}

void OneShotSampler::setPlayingFrequency(float freq)
{
  playingFrequency = freq;
  increment = playingFrequency/(ARRAYSIZE*referenceFrequency);
  setFilterFreqFactor(filterFreqFactor);
}

float OneShotSampler::getSampleAtPos(float pos)
{
  float sampleNumber = (ARRAYSIZE-1)*pos;
  int sampleNumber0 = floor(sampleNumber);
  float samplePos01 = sampleNumber - sampleNumber0;
  return juce::jmap<float>(samplePos01,wave[sampleNumber0],wave[sampleNumber0+1]);
  // return 0;
}

float OneShotSampler::processNextSample()
{
  if (isRunning)
  {
    float value = level*filter.processSample(getSampleAtPos(position));
    position = position + increment;
    isRunning = (position<1.f);
    return value;
  }
  else return 0.f;
}

/*

Waveform::Waveform()
{
}

Waveform::~Waveform()
{
}

float Waveform::getSampleAtPos(float pos)
{
  float sampleNumber = (ARRAYSIZE-1)*pos;
  int sampleNumber0 = floor(sampleNumber);
  float samplePos0 = sampleNumber - floor(sampleNumber);
  return juce::jmap<float>(samplePos0,wave[sampleNumber0],wave[sampleNumber0+1]);
}

void Waveform::setWave(Wave& wav)
{
  //for (int i=0; i<ARRAYSIZE; i++) wave[i]=wav[i];
  wave = wav;
}
*/

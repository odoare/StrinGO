/*
  ==============================================================================

    SimpleSampler.cpp
    Created: 19 Jan 2024 12:56:27pm
    Author:  od

  ==============================================================================
*/

#include "SimpleSampler.h"

SimpleSampler::SimpleSampler()
{
}

SimpleSampler::~SimpleSampler()
{
}

void SimpleSampler::prepare(juce::dsp::ProcessSpec spec)
{
  sampleRate = spec.sampleRate;
  referenceFrequency = sampleRate/ARRAYSIZE;
  increment = playingFrequency/(ARRAYSIZE*referenceFrequency);
  setFilterFreqFactor(filterFreqFactor);
  setFilterVelocityFreqFactor(filterVelocityFreqFactor);
}

void SimpleSampler::setWave(float *wav)
{
  wave = wav;
}

void SimpleSampler::setWaveByNumber(int waveNumber)
{
  if (waveNumber!=currentWaveNumber)
  {
    currentWaveNumber = waveNumber;
    setWave(getWave(waveNumber));
  }
}

void SimpleSampler::start()
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

void SimpleSampler::stop()
{
  isRunning = false;
  position = 0.f;
  // std::cout << "Stopped" << std::endl;
}

void SimpleSampler::setFilterFreqFactor(float freqFactor)
{
  filterFreqFactor = freqFactor;
}

void SimpleSampler::setFilterVelocityFreqFactor(float factor)
{
  filterVelocityFreq = factor;
}

void SimpleSampler::setVelocity(float vel)
{
  filterVelocityFreqFactor = juce::jmap<float>(float(vel), 1-filterVelocityFreq, 1) ;
  filter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,playingFrequency * filterFreqFactor * filterVelocityFreqFactor);
}


void SimpleSampler::setLevel(float lvl)
{
  level = lvl;
}

void SimpleSampler::setReferenceFrequency(float freq)
{
  referenceFrequency = freq;
  increment = playingFrequency/(ARRAYSIZE*referenceFrequency);
}

void SimpleSampler::setPlayingFrequency(float freq)
{
  playingFrequency = freq;
  increment = playingFrequency/(ARRAYSIZE*referenceFrequency);
  setFilterFreqFactor(filterFreqFactor);
}

void SimpleSampler::setLooping(bool l)
{
  loop = l;
  std::cout << loop << std::endl;
}

float SimpleSampler::getSampleAtPos(float pos)
{
  float sampleNumber = (ARRAYSIZE-1)*pos;
  int sampleNumber0 = floor(sampleNumber);
  float samplePos01 = sampleNumber - sampleNumber0;
  return juce::jmap<float>(samplePos01,wave[sampleNumber0],wave[sampleNumber0+1]);
  // return 0;
}

float SimpleSampler::processNextSample()
{
  if (isRunning)
  {
    float value = level*filter.processSample(getSampleAtPos(position));
    position = position + increment;
    if (position>=1.f)
      {
        position = 0.f;
        isRunning = loop;
      }
    return value;
  }
  else return 0.f;
}

/*
  ==============================================================================

    SimpleSampler.cpp
    Created: 19 Jan 2024 12:56:27pm
    Author:  od

  ==============================================================================
*/

#include "SimpleSampler.h"

#ifdef DEBUG
  #include <iostream>
#endif

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
  setFilterFreqVelocityInfluence(filterFreqVelocityInfluence);

  juce::ADSR::Parameters para{attack,decay,sustain,release};
  adsr.setParameters(para);
  adsr.setSampleRate(spec.sampleRate);
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

  juce::ADSR::Parameters para{attack,decay,sustain,release};
  adsr.setParameters(para);

  position = 0.f;
  filter.reset();
  isRunning = true;

  adsr.noteOn();

  #ifdef DEBUG
    std::cout << "Start sampler     FREQ = "
              << playingFrequency 
              << "     Ref freq = " << referenceFrequency
              << "     Level = " << level
              << "     Increment = " << increment
              << "     Position = " << position
              << "     Attack = " << para.attack
              << "     Decay = " << para.decay
              << "     Sustain = " << para.sustain
              << "     Release = " << para.release
              << std::endl;
  #endif
}

void SimpleSampler::stop()
{
  adsr.noteOff();

  #ifdef DEBUG
    std::cout << "Stopped" << std::endl;
  #endif
}

void SimpleSampler::setFilterFreqFactor(float freqFactor)
{
  filterFreqFactor = freqFactor;
}

void SimpleSampler::setFilterFreqVelocityInfluence(float val)
{
  filterFreqVelocityInfluence = val;
}

void SimpleSampler::setLevelVelocityInfluence(float val)
{
  levelVelocityInfluence = val;
}

void SimpleSampler::setVelocity(float vel)
{
  filterFreqVelocityFactor = juce::jmap<float>(float(vel), 1-filterFreqVelocityInfluence, 1) ;
  levelVelocityFactor = juce::jmap<float>(float(vel), 1-levelVelocityInfluence, 1) ;
  filter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,playingFrequency * filterFreqFactor * filterFreqVelocityFactor);
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
}

void SimpleSampler::setAttack(float a)
{
  attack = a;
}
void SimpleSampler::setDecay(float d)
{
  decay = d;
}
void SimpleSampler::setSustain(float s)
{
  sustain = s;
}
void SimpleSampler::setRelease(float r)
{
  release = r;
}

float SimpleSampler::getSampleAtPos(float pos)
{
  float sampleNumber = (ARRAYSIZE-1)*pos;
  int sampleNumber0 = floor(sampleNumber);
  float samplePos01 = sampleNumber - sampleNumber0;
  return juce::jmap<float>(samplePos01,wave[sampleNumber0],wave[sampleNumber0+1]);
}

float SimpleSampler::processNextSample()
{
  if (isRunning)
  {
    float value = level
                    *levelVelocityFactor
                    *adsr.getNextSample()
                    *filter.processSample(getSampleAtPos(position));
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

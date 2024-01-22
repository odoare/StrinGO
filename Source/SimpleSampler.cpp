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
  setFilterFreq(filterFreqFactor);
}

void OneShotSampler::setWave(float *wav)
{
  for (int sample=0; sample<ARRAYSIZE; sample++)
  {
    wave[sample] = wav[sample];
    // wave = wav;
  }
}

void OneShotSampler::setWaveByNumber(int waveNumber)
{
  if (waveNumber!=currentWaveNumber)
  {
    currentWaveNumber = waveNumber;
    if (waveNumber==0) setWave(sine);
    else if (waveNumber==1) setWave(saw);
    else if (waveNumber==2) setWave(square);
    else if (waveNumber==3) setWave(tri);
      else if (waveNumber==4) setWave(piano);
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

void OneShotSampler::setFilterFreq(float freqFactor)
{
  filterFreqFactor = freqFactor;
  filter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,playingFrequency * filterFreqFactor);
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
  setFilterFreq(filterFreqFactor);
        // std::cout << "Fréquence : " << freq <<  std::endl;
        // std::cout << "Incrément : " << increment <<  std::endl;
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
    //float value = level*getSampleAtPos(position);
    // std::cout << increment << "<";
    position = position + increment;
    isRunning = (position<1.f);
    //std::cout <<  position << " " ;
    // if (position<0.f)
    //   std::cout << stringNum << ">N "; 
    // else 
    //   std::cout << stringNum << ">" << position << " ";
    // if (!isRunning) std::cout << "Position " << position << " STOP SAMPLER" << std::endl;
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

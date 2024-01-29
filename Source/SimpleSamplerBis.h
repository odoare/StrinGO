/*
  ==============================================================================

    OneShotSampler.h
    Created: 19 Jan 2024 12:56:27pm
    Author:  od

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SampleArraysBis.h"
#include <iostream>

class OneShotSampler
{
private:
  /* data */
  WaveType wave { WaveType::sawType };
  float sampleRate;
  float referenceFrequency;
  float playingFrequency;
  float level { 1.0 };

  float getSampleAtPos(float pos)
  {
    float sampleNumber = (ARRAYSIZE-1)*pos;
    int sampleNumber0 = floor(sampleNumber);
    float samplePos01 = sampleNumber - sampleNumber0;
    //return juce::jmap<float>(samplePos01,waves[wave][sampleNumber0],waves[wave][sampleNumber0+1]);
    return 0;
  }


public:

  OneShotSampler();
  ~OneShotSampler();
  
  void prepare(juce::dsp::ProcessSpec spec)
    {
      sampleRate = spec.sampleRate;
      referenceFrequency = sampleRate/ARRAYSIZE;
      increment = playingFrequency/(ARRAYSIZE*referenceFrequency);

      std::cout << "HERE" << std::endl;
      std::cout << "Finished OneshotSampler prepare" << std::endl;
    }

  void setWave(WaveType wav)
  {
    // for (int sample=0; sample<ARRAYSIZE; sample++)
    // {
    //   wave[sample] = wav[sample];
      wave = wav;
    // }
    std::cout << "WAVE: " << wave << std::endl; 
  }

  void start()
  {
    position = 0.f;
    isRunning = true;

    std::cout << "Start sampler     FREQ = "
              << playingFrequency 
              << "     Ref freq = " << referenceFrequency
              << "     Level = " << level
              << "     Increment = " << increment
              << "     Position = " << position
              << std::endl;
  }


  void stop()
  {
    isRunning = false;
    position = 0.f;
    std::cout << "Stopped" << std::endl;
  }

  void setLevel(float lvl)
  {
    level = lvl;
  }

  void setReferenceFrequency(float freq)
  {
    referenceFrequency = freq;
    increment = playingFrequency/(ARRAYSIZE*referenceFrequency);
  }


  void setPlayingFrequency(float freq)
  {
    playingFrequency = freq;
    increment = playingFrequency/(ARRAYSIZE*referenceFrequency);
          std::cout << "Fréquence : " << freq <<  std::endl;
          std::cout << "Incrément : " << increment <<  std::endl;
  }


  float processNextSample()
  {
    if (isRunning)
    {
      float value = level*getSampleAtPos(position);
      // std::cout << increment << "<";
      position = position + increment;
      isRunning = (position<1.f);
      //std::cout <<  position << " " ;
      if (position<0.f)
        std::cout << stringNum << ">N "; 
      else 
        std::cout << stringNum << ">" << position << " ";
      if (!isRunning) std::cout << "Position " << position << " STOP SAMPLER" << std::endl;
      return value;
    }
    else return 0.f;
  }

  float position { 0.f };
  float increment;
  bool isRunning { false };
  int stringNum;

};


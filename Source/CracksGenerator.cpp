/*
  ==============================================================================

    CracksGenerator.cpp
    Created: 18 Jan 2024 3:43:48pm
    Author:  od

  ==============================================================================
*/

#include "CracksGenerator.h"


CracksGenerator::CracksGenerator(/* args */)
{

}

CracksGenerator::~CracksGenerator()
{
}

void CracksGenerator::prepare(juce::dsp::ProcessSpec spec)
{
  sampleRateMin = int(60*spec.sampleRate);
}

float CracksGenerator::nextSample()
{
  int value = rnd.nextInt(sampleRateMin);
  
  if (density>=value)
  {
    return 2*(rnd.nextFloat()-0.5f);
  }
  else return 0.f;
}

void CracksGenerator::setDensity(int d)
{
  density = d;
}

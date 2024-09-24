/*
  ==============================================================================

    StringReso.h
    Created: 4 Jan 2024 10:20:10pm
    Author:  od

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../lib/dsp/SimpleSampler.h"
#include <iostream>

#define NUMSTRINGS 2
#define SMOOTH_TIME 0.1f
#define SEMITONE 1.059463094359295f

#define LFOSAMPLESUPDATE 100

#define INPOSMAX 0.45f
#define INPOSMIN 0.05f
#define OUTPOSMAX 0.95f
#define OUTPOSMIN 0.55f

#define FINEMIN -1.f
#define FINEMAX 1.f
#define COARSEMIN -12.f
#define COARSEMAX 12.f

#define MAXCOUPLING 0.5f

#define NUMLFO 2

int suivant(int N, int i);

std::string makeStringResoLfoParam(std::string param, int num, int string);
std::string makeStringResoParam(std::string param, int string);

class StringReso
{
public:

  typedef struct
  {
    float freq;
    float amp;
    bool level[NUMSTRINGS];
    bool fine[NUMSTRINGS];
    bool coarse[NUMSTRINGS];
    bool inPos[NUMSTRINGS];
    bool outPos[NUMSTRINGS];
    bool samplerLevel;
    bool samplerFreq;
  } LfoParams;

  typedef struct
  {
    juce::ADSR::Parameters adsrParams1, adsrParamsN;
    float portamento;
    float stringPeriodInSamples;
    float level[NUMSTRINGS];
    float freqCoarseFactor[NUMSTRINGS];
    float freqFineFactor[NUMSTRINGS];
    float inPos[NUMSTRINGS];
    float outPos[NUMSTRINGS];
    float feedbackGainOn[NUMSTRINGS];
    float feedbackFreqOn[NUMSTRINGS];
    float levelOn[NUMSTRINGS];
    float feedbackGainOff[NUMSTRINGS];
    float feedbackFreqOff[NUMSTRINGS];
    float levelOff[NUMSTRINGS];
    float coupling[NUMSTRINGS];
    float velocityLevel;

    float noiseLPFilterFreq  { 1000.f };
    float noiseHPFilterFreq  { 1000.f };
    float noiseLevel { 1.0f };
    float noiseLPFilterFreqVelocityInfluence {0.f};
    float noiseLPFilterFreqVelocityFactor {1.f};
    float noiseHPFilterFreqVelocityInfluence {0.f};
    float noiseHPFilterFreqVelocityFactor {1.f};  
    float noiseLevelVelocityInfluence {0.f};
    float noiseLevelVelocityFactor {1.f};

    bool isOn;
    LfoParams lfoParams[NUMLFO];    
  } StringResoParams;

  StringReso();

  ~StringReso();

  //----------------------------------------------------------------
  void prepare(const juce::dsp::ProcessSpec spec, float minFreq);

  //----------------------------------------------------------------------------
  void process(juce::AudioBuffer<float>& inBuffer, juce::AudioBuffer<float>& outBuffer, int startSample, int numSamples);

  //-----------------------------------------
  void setPortamentoTime(float time);
  // void setADSR1(juce::ADSR::Parameters adsrParams);

  //--------------------------
  StringResoParams getParams();

  void setParams(StringResoParams newParams, bool force = false);

  void setIsOn(bool on, bool force = false);

  void setFeedbackGainOn(int string, float gain);
  void setFeedbackGainOff(int string, float gain);

  void setFeedbackFreqOn(int string, float freq);
  void setFeedbackFreqOff(int string, float freq);

  void setLevelOn(int string, float freq);
  void setLevelOff(int string, float freq);
  void setLevel(int string, float lvl);

  void setStringPeriodInSamples(float period, bool force = false);
  void setStringFreq(float freq, bool force = false);

  void setInPos(int string, float pos, bool force = false);
  void setOutPos(int string, float pos, bool force = false);

  void setFreqCoarseFactor(int string, float fac, bool force=false);
  void setFreqFineFactor(int string, float fac, bool force=false);

  void setCoupling(int string, float fac, bool force=false);

  void setSamplerLevel(int string, float lvl);

  void setVelocityLevel(float lvl);
  void setVelocity(float vel);

  void setLfoFreq(int num, float freq);
  void setLfoAmp(int num, float val);
  void setLfoFine(int num, int string, bool onoff);
  void setLfoCoarse(int num, int string, bool onoff);
  void setLfoLevel(int num, int string, bool onoff);
  void setLfoInPos(int num, int string, bool onoff);
  void setLfoOutPos(int num, int string, bool onoff);

  void setLfoSamplerLPFreq(int num, bool onoff);
  void setLfoSamplerLevel(int num, bool onoff);

  juce::dsp::ProcessSpec processSpec;

  void setADSR1(juce::ADSR::Parameters adsrParams);
  void setADSRN(juce::ADSR::Parameters adsrParams);

  SimpleSampler sampler[NUMSTRINGS];

  // Noise generator
  // juce::ADSR::Parameters adsrNParams;
  void setNoiseLPFilterFreq(float freq);
  void setNoiseLPFilterFreqVelocityInfluence(float factor);
  void setNoiseHPFilterFreq(float freq);
  //void setNoiseHPFilterFreqVelocityInfluence(float factor);
  void setNoiseLevel(float lvl);
  void setNoiseLevelVelocityInfluence(float val);

private:

  // juce::AudioBuffer<float>& inputBuffer;

  void setDelaySamples(int string, bool force = false);

  void setSamplerFreq(int string);

  juce::dsp::DelayLine<float,juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> delayLine[4*NUMSTRINGS];
  juce::dsp::IIR::Filter<float> fbFilter[NUMSTRINGS];

  juce::dsp::Oscillator<float> lfo[NUMLFO];  

  juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> smoothDelaySamples[4*NUMSTRINGS],
                                                                smoothFeedbackFreq[NUMSTRINGS],
                                                                smoothFeedbackGain[NUMSTRINGS],
                                                                smoothLevel[NUMSTRINGS],
                                                                smoothCoupling[NUMSTRINGS];

  StringResoParams params;

  float velocityLevelFactor;

  float currentFeedbackGain[NUMSTRINGS],
        currentFeedbackFreq[NUMSTRINGS],
        currentLevel[NUMSTRINGS],
        currentCoupling[NUMSTRINGS];

  float previousOutput[NUMSTRINGS];

  juce::ADSR adsr1;
  juce::ADSR adsrN;

  // Noise generator
  juce::Random randomNoise;
  juce::dsp::IIR::Filter<float> noiseLPFilter;
  juce::dsp::IIR::Filter<float> noiseHPFilter;
  // float noiseLPFilterFreq  { 1000.f };
  // float noiseHPFilterFreq  { 1000.f };
  // float noiseLevel { 1.0f };
  // float noiseLPFilterFreqVelocityInfluence {0.f};
  // float noiseLPFilterFreqVelocityFactor {1.f};
  // float noiseHPFilterFreqVelocityInfluence {0.f};
  // float noiseHPFilterFreqVelocityFactor {1.f};  
  // float noiseLevelVelocityInfluence {0.f};
  // float noiseLevelVelocityFactor {1.f};


  // LFOs
  float lfoVal[NUMLFO];

  float inPosDistToBoundary[NUMSTRINGS]={0.}, outPosDistToBoundary[NUMSTRINGS]={0.};
  float fineFreqDistToBoundary[NUMSTRINGS]={0.}, coarseFreqDistToBoundary[NUMSTRINGS]={0.};

};

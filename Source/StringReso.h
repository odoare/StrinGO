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
#include "../lib/dsp/CracksGenerator.h"
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

#define NOISELPFMIN 20.f
#define NOISELPFMAX 20000.f
#define NOISEHPFMIN 20.f
#define NOISEHPFMAX 20000.F
#define CRACKSLPFMIN 20.f
#define CRACKSLPFMAX 20000.f
#define CRACKDENSITYFMIN 0
#define CRACKSDENSITYMAX 1000

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
    bool noiseLevel;
    bool noiseLPFreq;
    bool noiseHPFreq;
    bool cracksLevel;
    bool cracksDensity;
    bool cracksLPFreq;
  } LfoParams;

  typedef struct
  {
    juce::ADSR::Parameters adsrParams1, adsrParamsN, adsrParamsC;
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

    float crackLPFilterFreq  { 1000.f };
    float crackLevel { 1.0 };
    float crackLPFilterFreqVelocityInfluence {0.f};
    float crackLPFilterFreqVelocityFactor {1.f};
    float crackLevelVelocityInfluence {0.f};
    float crackLevelVelocityFactor {1.f};
    float crackDensity {10};
    
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


  // LFO parameters
  void setLfoFreq(int num, float freq);
  void setLfoAmp(int num, float val);
  void setLfoFine(int num, int string, bool onoff);
  void setLfoCoarse(int num, int string, bool onoff);
  void setLfoLevel(int num, int string, bool onoff);
  void setLfoInPos(int num, int string, bool onoff);
  void setLfoOutPos(int num, int string, bool onoff);

  void setLfoSamplerLPFreq(int num, bool onoff);
  void setLfoSamplerLevel(int num, bool onoff);

  void setLfoCrackLevel(int num, bool onoff);
  void setLfoCrackLPFreq(int num, bool onoff);
  void setLfoCrackDensity(int num, bool onoff);
  
  void setLfoNoiseLevel(int num, bool onoff);
  void setLfoNoiseLPFreq(int num, bool onoff);
  void setLfoNoiseHPFreq(int num, bool onoff);

  void updateLfos();

  juce::dsp::ProcessSpec processSpec;

  void setADSR1(juce::ADSR::Parameters adsrParams);

  SimpleSampler sampler[NUMSTRINGS];

  // Noise generator
  // juce::ADSR::Parameters adsrNParams;
  void setADSRN(juce::ADSR::Parameters adsrParams);
  void setNoiseLPFilterFreq(float freq);
  void setNoiseLPFilterFreqVelocityInfluence(float factor);
  void setNoiseHPFilterFreq(float freq);
  //void setNoiseHPFilterFreqVelocityInfluence(float factor);
  void setNoiseLevel(float lvl);
  void setNoiseLevelVelocityInfluence(float val);
  void setNoiseLPFilterCoeffs();
  void setNoiseHPFilterCoeffs();

  // Crack generator
  void setADSRC(juce::ADSR::Parameters adsrParams);
  void setCrackDensity(int d);
  void setCrackLPFilterFreq(float freq);
  void setCrackLPFilterFreqVelocityInfluence(float val);
  void setCrackLevel(float lvl);
  void setCrackLevelVelocityInfluence(float val);
  void setCrackLPFilterCoeffs();

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

  // Noise generator
  juce::Random randomNoise;
  juce::dsp::IIR::Filter<float> noiseLPFilter;
  juce::dsp::IIR::Filter<float> noiseHPFilter;
  juce::ADSR adsrN;

  // Cracks generator
  CracksGenerator cracksGenerator;
  juce::dsp::IIR::Filter<float> crackLPFilter;
  juce::ADSR adsrC;  

  // LFOs
  float lfoVal[NUMLFO];

  float inPosDistToBoundary[NUMSTRINGS]={0.}, outPosDistToBoundary[NUMSTRINGS]={0.};
  float fineFreqDistToBoundary[NUMSTRINGS]={0.}, coarseFreqDistToBoundary[NUMSTRINGS]={0.};
  float noiseLPFDistToBoundary, noiseHPFDistToBoundary, cracksLPFDistToBoundary;

  float lfoFacLevel[NUMSTRINGS]={1.f};
  float lfoFacInPos[NUMSTRINGS]={1.f};
  float lfoFacOutPos[NUMSTRINGS]={1.f};
  float lfoFacFineFreq[NUMSTRINGS]={1.f};
  float lfoFacCoarseFreq[NUMSTRINGS]={1.f};
  float lfoFacSampleLevel{1.f};
  float lfoFacSampleLPF{1.f};
  float lfoFacNoiseLevel{1.f};
  float lfoFacNoiseLPF{1.f};
  float lfoFacNoiseHPF{1.f};
  float lfoFacCrackevel{1.f};
  float lfoFacCrackLPF{1.f};
  float lfoFacCrackDensity{1.f};
};

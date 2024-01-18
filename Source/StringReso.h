/*
  ==============================================================================

    StringReso.h
    Created: 4 Jan 2024 10:20:10pm
    Author:  od

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include <iostream>

#define NUMSTRINGS 2
#define SMOOTH_TIME 0.1f
#define SEMITONE 1.059463094359295f

int suivant(int N, int i);

class StringReso
{
public:

  typedef struct
  {
    juce::ADSR::Parameters adsrParams1, adsrParamsn;
    float decay1, decayn;
    float sustain1, sustainn;
    float release1, releasen;
    float portamento;
    float smoothTime;
    float stringPeriodInSamples;
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
    bool isOn;
  } StringResoParams;

  StringReso();

  ~StringReso();

  //----------------------------------------------------------------
  void prepare(const juce::dsp::ProcessSpec spec, float minFreq);

  //----------------------------------------------------------------------------
  //void process(juce::AudioBuffer<float>& inBuffer, juce::AudioBuffer<float>& outBuffer);
  void process(juce::AudioBuffer<float>& inBuffer, juce::AudioBuffer<float>& outBuffer, int startSample, int numSamples);

  //-----------------------------------------
  void setSmoothTime(float time);
  void setPortamentoTime(float time);

  void setADSR1(juce::ADSR::Parameters adsrParams);

  //--------------------------
  StringResoParams getParams();

  void setParams(StringResoParams newParams, bool force = false);

  void setIsOn(bool on, bool force = false);

  void setFeedbackGain(int string, float gain, bool force = false);
  void setFeedbackGainOn(int string, float gain, bool force = false);
  void setFeedbackGainOff(int string, float gain, bool force = false);

  void setFeedbackFreqOn(int string, float freq, bool force = false);
  void setFeedbackFreqOff(int string, float freq, bool force = false);
  void setFeedbackFreq(int string, float freq, bool force = false);

  void setLevelOn(int string, float freq, bool force=false);
  void setLevelOff(int string, float freq, bool force=false);
  void setLevel(int string, float freq, bool force=false);

  void setStringPeriodInSamples(float period, bool force = false);
  void setStringFreq(float freq, bool force = false);

  void setInPos(int string, float pos, bool force = false);
  void setOutPos(int string, float pos, bool force = false);

  void setFreqCoarseFactor(int string, float fac, bool force=false);
  void setFreqFineFactor(int string, float fac, bool force=false);

  void setCoupling(int string, float fac, bool force=false);

  // float maxFreq;
  juce::dsp::ProcessSpec processSpec;

  juce::ADSR adsr1, adsrn;
  juce::ADSR::Parameters adsr1Params;

private:

  //void setTargetDelaySamples();

  void setDelaySamples(int string, bool force = false);

  juce::dsp::DelayLine<float,juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> delayLine[4*NUMSTRINGS];
  juce::dsp::IIR::Filter<float> fbFilter[NUMSTRINGS];

  juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> smoothDelaySamples[4*NUMSTRINGS],
                                                                smoothFeedbackFreq[NUMSTRINGS],
                                                                smoothFeedbackGain[NUMSTRINGS],
                                                                smoothLevel[NUMSTRINGS],
                                                                smoothCoupling[NUMSTRINGS];

  StringResoParams params;

  float currentFeedbackGain[NUMSTRINGS],
        currentFeedbackFreq[NUMSTRINGS],
        currentLevel[NUMSTRINGS],
        currentCoupling[NUMSTRINGS];

  float previousOutput[NUMSTRINGS];

  juce::dsp::Oscillator<float> lfo { [](float x) {return std::sin(x); }};
};

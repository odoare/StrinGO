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

class StringReso
{
public:

  typedef struct
  {
    float attack;
    float release;
    float portamento;
    float stringPeriodInSamples;
    float inPos, outPos;
    float feedbackGainOn;
    float feedbackFreqOn;
    float levelOn;
    float feedbackGainOff;
    float feedbackFreqOff;
    float levelOff;
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
  void setAttackTimeInSamples(int numSamples);
  void setReleaseTimeInSamples(int numSamples);
  void setPortamentoTimeInSamples(int numSamples);

  //--------------------------
  StringResoParams getParams();

  void setIsOn(bool on, bool force = false);

  //void setTargetIsOn(bool on);

  //void setTargetFeedbackGain(float gain);

  void setFeedbackGain(float gain, bool force = false);

  //void setTargetFeedbackGainOn(float gain);

  void setFeedbackGainOn(float gain, bool force = false);

  //void setTargetFeedbackGainOff(float gain);

  void setFeedbackGainOff(float gain, bool force = false);

  //void setTargetFeedbackFreqOn(float freq);

  void setFeedbackFreqOn(float freq, bool force = false);

  //void setTargetFeedbackFreqOff(float freq);

  void setFeedbackFreqOff(float freq, bool force = false);

  //void setTargetFeedbackFreq(float freq);

  void setFeedbackFreq(float freq, bool force = false);

  void setLevelOn(float freq, bool force);
  void setLevelOff(float freq, bool force);
  void setLevel(float freq, bool force);

  //void setTargetStringPeriodInSamples(float period);

  void setStringPeriodInSamples(float period, bool force = false);

  //void setTargetStringFreq(float freq);

  void setStringFreq(float freq, bool force = false);

  //void setTargetInPos(float pos);

  void setInPos(float pos, bool force = false);

  //void setTargetOutPos(float pos);

  void setOutPos(float pos, bool force = false);

private:

  //void setTargetDelaySamples();

  void setDelaySamples(bool force = false);

  juce::dsp::ProcessSpec processSpec;
  juce::dsp::DelayLine<float,juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> delayLine[4];
  juce::dsp::IIR::Filter<float> fbFilter;
  juce::SmoothedValue<float,juce::ValueSmoothingTypes::Linear> smoothDelaySamples[4],
                                                                smoothFeedbackFreq,
                                                                smoothFeedbackGain,
                                                                smoothLevel;

  StringResoParams params;
  float currentFeedbackGain, currentFeedbackFreq, currentLevel;
};


// ---------------------------------------------------------------------------------------------

// class StringsReso
// {

// public:

//   StringsReso(int n);

//   ~StringsReso();

//   void prepare(juce::dsp::ProcessSpec spec);

//   void process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

//   // StringReso::Params paramsToParams(Params stringsResoParams, float periodInSamples)
//   // {
//   //   StringReso::Params params;
//   //   params.smoothTime = stringsResoParams.smoothTime;
//   //   params.inPos = stringsResoParams.inPos;
//   //   params.outPos = stringsResoParams.outPos;
//   //   params.feedbackGainOn = stringsResoParams.feedbackGainOn;
//   //   params.feedbackFreqOn = stringsResoParams.feedbackFreqOn;
//   //   params.feedbackGainOff = stringsResoParams.feedbackGainOff;
//   //   params.feedbackFreqOff = stringsResoParams.feedbackFreqOff;
//   //   params.stringPeriodInSamples = periodInSamples;
//   //   return params;
//   // }

// private:
//   StringReso::StringResoParams params;
//   int numStrings;
//   std::vector<StringReso> strings;
//   std::vector<int> stringsIndices;
  
// };


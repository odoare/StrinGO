/*
  ==============================================================================

    StringReso.cpp
    Created: 4 Jan 2024 10:20:10pm
    Author:  od

  ==============================================================================
*/

#include "StringReso.h"


StringReso::StringReso()
  {
    // Initialize class parameters with some default values
    params.smoothTime = 0.1f;
    params.stringPeriodInSamples = 100.f;
    params.inPos = 0.1f;
    params.outPos = 0.9f;
    params.feedbackFreqOn = 2000.f;
    params.feedbackGainOn = 0.95;
    params.feedbackFreqOff = 1000.f;
    params.feedbackGainOff = 0.1;
    params.isOn = false;
    currentFeedbackFreq = params.feedbackFreqOff;
    currentFeedbackGain = params.feedbackGainOff;
  }

StringReso::~StringReso()
  {

  }

  //----------------------------------------------------------------
void StringReso::prepare(const juce::dsp::ProcessSpec spec, float minFreq)
  {
    processSpec = spec;
    setSmoothTimeInSamples(params.smoothTime);

    // We set not isOn before to force the updating of other parameters
    setIsOn(!params.isOn);
    setIsOn(params.isOn);
    
    float delaySamples[4];
    delaySamples[0] = params.inPos*params.stringPeriodInSamples;
    delaySamples[1] = 0.5*(params.outPos-params.inPos)*params.stringPeriodInSamples;
    delaySamples[2] = delaySamples[1];
    delaySamples[3] = (1-params.outPos)*params.stringPeriodInSamples;
    
    for (int i=0; i<4; i++)
    {
      delayLine[i].setMaximumDelayInSamples(int(processSpec.sampleRate/minFreq));
      smoothDelaySamples[i].setCurrentAndTargetValue(delaySamples[i]);
      delayLine[i].prepare(processSpec);
    }
    
    fbFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,currentFeedbackFreq);
    fbFilter.prepare(processSpec);

  }

  //----------------------------------------------------------------------------
// //   void StringReso::process(juce::AudioBuffer<float>& inBuffer, juce::AudioBuffer<float>& outBuffer)
// {
//     for (int channel = 0; channel < inBuffer.getNumChannels(); ++channel)
//     {
//       auto* inChannelData = inBuffer.getReadPointer (channel);
//       auto* outChannelData = outBuffer.getWritePointer (channel);
//       for (int sample=0; sample<inBuffer.getNumSamples(); ++sample)
//       {
//         fbFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,smoothFeedbackFreq.getNextValue());
//         float loop = fbFilter.processSample(smoothFeedbackGain.getNextValue()*delayLine[3].popSample(channel,smoothDelaySamples[3].getNextValue()));
//         float in0 = loop + 0.5*inChannelData[sample];
//         float in1 = - delayLine[0].popSample(channel,smoothDelaySamples[0].getNextValue()) + 0.5*inChannelData[sample];
//         float in2 = delayLine[1].popSample(channel,smoothDelaySamples[1].getNextValue());
//         float in3 = - delayLine[2].popSample(channel,smoothDelaySamples[2].getNextValue());

//         delayLine[0].pushSample(channel,in0);
//         delayLine[1].pushSample(channel,in1);
//         delayLine[2].pushSample(channel,in2);
//         delayLine[3].pushSample(channel,in3);

//         outChannelData[sample] = in1+in2;
//       }
//     }
//   }

void StringReso::process(juce::AudioBuffer<float>& buffer, int statSample, int numSamples)
{
    for (int channel = 0; channel < inBuffer.getNumChannels(); ++channel)
    {
      auto* channelData = buffer.getReadPointer (channel);
      //auto* outChannelData = outBuffer.getWritePointer (channel);
      for (int sample=0; sample<inBuffer.getNumSamples(); ++sample)
      {
        fbFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,smoothFeedbackFreq.getNextValue());
        float loop = fbFilter.processSample(smoothFeedbackGain.getNextValue()*delayLine[3].popSample(channel,smoothDelaySamples[3].getNextValue()));
        float in0 = loop + 0.5*channelData[sample];
        float in1 = - delayLine[0].popSample(channel,smoothDelaySamples[0].getNextValue()) + 0.5*inChannelData[sample];
        float in2 = delayLine[1].popSample(channel,smoothDelaySamples[1].getNextValue());
        float in3 = - delayLine[2].popSample(channel,smoothDelaySamples[2].getNextValue());

        delayLine[0].pushSample(channel,in0);
        delayLine[1].pushSample(channel,in1);
        delayLine[2].pushSample(channel,in2);
        delayLine[3].pushSample(channel,in3);

        outChannelData[sample] = in1+in2;
      }
    }
  }

//-----------------------------------------
void StringReso::setSmoothTimeInSamples(int numSamples)
{
if (numSamples!=params.smoothTime)
{
    params.smoothTime = numSamples;
    for (int i=0; i<4; i++)
    {
    smoothDelaySamples[i].reset(numSamples);
    }
    smoothFeedbackFreq.reset(numSamples);
    smoothFeedbackGain.reset(numSamples);
}
}

//--------------------------
StringReso::StringResoParams StringReso::getParams()
{
    std::cout << "inPos" << params.inPos << std::endl;
    std::cout << "outPos" << params.outPos << std::endl;
    std::cout << "FbFreqOff" << params.feedbackFreqOff << std::endl;
    std::cout << "FbFreqOn" << params.feedbackFreqOn << std::endl;
    std::cout << "FbGainOff" << params.feedbackGainOff << std::endl;
    std::cout << "FbGainOn" << params.feedbackGainOn << std::endl;
    return params;
}

//

void StringReso::setIsOn(bool on, bool force)
{
    if (on!=params.isOn)
    {
        params.isOn = on;
        setFeedbackGain(on ? params.feedbackGainOn : params.feedbackGainOff, force);
        setFeedbackFreq(on ? params.feedbackFreqOn : params.feedbackFreqOff, force);
    }
}

// void StringReso::setTargetIsOn(bool on)
// {
// if (on!=params.isOn)
// {
//     params.isOn = on;
//     setTargetFeedbackGain(on ? params.feedbackGainOn : params.feedbackGainOff);
//     setTargetFeedbackFreq(on ? params.feedbackFreqOn : params.feedbackFreqOff);
// }
// }

// void StringReso::setTargetFeedbackGain(float gain)
// {
// if (gain!=currentFeedbackGain)
// {
//     currentFeedbackGain = gain;
//     smoothFeedbackGain.setTargetValue(gain);
// }
// }

void StringReso::setFeedbackGain(float gain, bool force)
{
    if (gain!=currentFeedbackGain)
    {
        currentFeedbackGain = gain;
        if (force)
            smoothFeedbackGain.setCurrentAndTargetValue(gain);
        else
            smoothFeedbackGain.setTargetValue(gain);
    }
}

// void StringReso::setTargetFeedbackGainOn(float gain)
// {
// if (gain!=params.feedbackGainOn)
// {
//     params.feedbackGainOn = gain;
//     if (params.isOn) setTargetFeedbackGain(gain);
// }
// }

void StringReso::setFeedbackGainOn(float gain, bool force)
{
    if (gain!=params.feedbackGainOn)
    {
        params.feedbackGainOn = gain;
        if (params.isOn) setFeedbackGain(gain, force);
    }
}

// void StringReso::setTargetFeedbackGainOff(float gain)
// {
// if (gain!=params.feedbackGainOff)
// {
//     params.feedbackGainOff = gain;
//     if (!params.isOn) setTargetFeedbackGain(gain);
// }
// }

void StringReso::setFeedbackGainOff(float gain, bool force)
{
    if (gain!=params.feedbackGainOff)
    {
        params.feedbackGainOff = gain;
        if (!params.isOn) setFeedbackGain(gain, force);
    }
}


// void StringReso::setTargetFeedbackFreqOn(float freq)
// {
// if (freq!=params.feedbackFreqOn)
// {
//     params.feedbackFreqOn = freq;
//     if (params.isOn) setTargetFeedbackFreq(freq);
// }
// }

void StringReso::setFeedbackFreqOn(float freq, bool force)
{
    if (freq!=params.feedbackFreqOn)
    {
        params.feedbackFreqOn = freq;
        if (params.isOn) setFeedbackFreq(freq, force);
    }
}

// void StringReso::setTargetFeedbackFreqOff(float freq)
// {
// if (freq!=params.feedbackFreqOff)
// {
//     params.feedbackFreqOff = freq;
//     if (!params.isOn) setTargetFeedbackFreq(freq);
// }
// }

void StringReso::setFeedbackFreqOff(float freq, bool force)
{
    if (freq!=params.feedbackFreqOff)
    {
        params.feedbackFreqOff = freq;
        if (!params.isOn) setFeedbackFreq(freq, force);
    }
}

// void StringReso::setTargetFeedbackFreq(float freq)
// {
// if (freq!=currentFeedbackFreq)
// {
//     currentFeedbackFreq = freq;
//     smoothFeedbackFreq.setTargetValue(freq);
// }
// }

void StringReso::setFeedbackFreq(float freq, bool force)
{
    if (freq!=currentFeedbackFreq)
    {
        currentFeedbackFreq = freq;
        if (force)
            smoothFeedbackFreq.setCurrentAndTargetValue(freq);
        else
            smoothFeedbackFreq.setTargetValue(freq);
    }
}

// void StringReso::setTargetStringPeriodInSamples(float period)
// {
// if (period!=params.stringPeriodInSamples)
// {
//     params.stringPeriodInSamples = period;
//     setTargetDelaySamples();
// }
// }

void StringReso::setStringPeriodInSamples(float period, bool force)
{
    if (period!=params.stringPeriodInSamples)
    {
        params.stringPeriodInSamples = period;
        setDelaySamples(force);
    }
}

// void StringReso::setTargetStringFreq(float freq)
// {
// params.stringPeriodInSamples = processSpec.sampleRate/freq;
// setTargetDelaySamples();
// }

void StringReso::setStringFreq(float freq, bool force)
{
    params.stringPeriodInSamples = processSpec.sampleRate/freq;
    setDelaySamples(force);
    std::cout << "total delay : " << params.stringPeriodInSamples << std::endl;
    std::cout << "inPos : " << params.inPos << std::endl;
    std::cout << "outPos : " << params.outPos << std::endl;
    
    
}

// void StringReso::setTargetInPos(float pos)
// {
// if (pos!=params.inPos)
// {
//     params.inPos = pos;
//     setTargetDelaySamples();
// }
// }

void StringReso::setInPos(float pos, bool force)
{
    if (pos!=params.inPos)
    {
        params.inPos = pos;
        setDelaySamples(force);
}
}

// void StringReso::setTargetOutPos(float pos)
// {
// if (pos!=params.outPos)
// {
//     params.outPos = pos;
//     setTargetDelaySamples();
// }
// }

void StringReso::setOutPos(float pos, bool force)
{
    if (pos!=params.outPos)
    {
        params.outPos = pos;
        setDelaySamples(force);
    }
}

// Private part

// void StringReso::setTargetDelaySamples()
// {
// float delaySamples[4];
// delaySamples[0] = params.inPos*params.stringPeriodInSamples;
// delaySamples[1] = 0.5*(params.outPos-params.inPos)*params.stringPeriodInSamples;
// delaySamples[2] = delaySamples[1];
// delaySamples[3] = (1-params.outPos)*params.stringPeriodInSamples;

// for (int i=0; i<4; i++)
// {
//     smoothDelaySamples[i].setTargetValue(delaySamples[i]);
// }
// }

void StringReso::setDelaySamples(bool force)
{
    float delaySamples[4];
    delaySamples[0] = params.inPos*params.stringPeriodInSamples;
    delaySamples[1] = 0.5*(params.outPos-params.inPos)*params.stringPeriodInSamples;
    delaySamples[2] = delaySamples[1];
    delaySamples[3] = (1-params.outPos)*params.stringPeriodInSamples;

    if (force)
        for (int i=0; i<4; i++)
        {
            smoothDelaySamples[i].setCurrentAndTargetValue(delaySamples[i]);
        }
    else
        for (int i=0; i<4; i++)
        {
            smoothDelaySamples[i].setTargetValue(delaySamples[i]);
        }

    std::cout << "delay 0 : " << delaySamples[0] << std::endl;
    std::cout << "delay 1 : " << delaySamples[1] << std::endl;
    std::cout << "delay 2 : " << delaySamples[2] << std::endl;
    std::cout << "delay 3 : " << delaySamples[3] << std::endl;
}


/*
// ---------------------------------------------------------------------------------------------

class StringsReso
{
typedef struct
{
float smoothTime;
float inPos, outPos;
float feedbackGainOn, feedbackGainOff;
float feedbackFreqOn, feedbackFreqOff;
int numVoices;    
} Params;

StringsReso(int numStrings)
{

}

~StringsReso()
{

}

void prepare()
{

}

void process()
{

}

StringReso::Params paramsToParams(Params stringsResoParams, float periodInSamples)
{
StringReso::Params params;
params.smoothTime = stringsResoParams.smoothTime;
params.inPos = stringsResoParams.inPos;
params.outPos = stringsResoParams.outPos;
params.feedbackGainOn = stringsResoParams.feedbackGainOn;
params.feedbackFreqOn = stringsResoParams.feedbackFreqOn;
params.feedbackGainOff = stringsResoParams.feedbackGainOff;
params.feedbackFreqOff = stringsResoParams.feedbackFreqOff;
params.stringPeriodInSamples = periodInSamples;
return params;
}

private:
Params params;
int numStrings;
std::vector<StringReso> strings;
std::vector<int> stringsIndices;


};
*/

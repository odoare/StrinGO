/*
  ==============================================================================

    StringReso.cpp
    Created: 4 Jan 2024 10:20:10pm
    Author:  od

  ==============================================================================
*/

#include "StringReso.h"

int suivant(int N, int i)
{
  if (i>=N)
    return 0;
  else return i;
}

StringReso::StringReso()
  {
    // std::cout << "begin StringReso initialization   ";
    // Initialize class parameters with some default values
    params.adsrParams1.attack = 0.1f;
    params.adsrParams1.decay = 0.1f;
    params.adsrParams1.sustain = 1.f;
    params.adsrParams1.release = 1.f;
    params.portamento = SMOOTH_TIME;
    params.smoothTime = SMOOTH_TIME;
    params.stringPeriodInSamples = 100.f;
    params.freqCoarseFactor[0] = 0.f;
    params.freqCoarseFactor[1] = 7.f;
    params.freqFineFactor[0] = -0.05f;
    params.freqFineFactor[1] = 0.05f;
    params.inPos[0] = 0.1f;
    params.inPos[1] = 0.1f;
    params.outPos[0] = 0.9f;
    params.outPos[1] = 0.9f;
    params.feedbackGainOn[0] = 0.99f;
    params.feedbackGainOn[1] = 0.99f;
    params.feedbackFreqOn[0] = 50.f;
    params.feedbackFreqOn[1] = 50.f;
    params.levelOn[0] = 1.f;
    params.levelOn[1] = 1.f;
    params.feedbackGainOff[0] = 0.5f;
    params.feedbackGainOff[1] = 0.5f;
    params.feedbackFreqOff[0] = 49.5f;
    params.feedbackFreqOff[1] = 49.5f;
    params.levelOff[0] = 0.2f;
    params.levelOff[1] = 0.2f;
    params.coupling[0] = 0.f;//0.1f;
    params.coupling[1] = 0.f;//0.1f;
    params.isOn=false;
    params.velocityLevel = 0.f;
    
    for (int i=0; i<NUMSTRINGS; i++)
        {
            currentFeedbackFreq[i] = params.feedbackFreqOff[i];
            currentFeedbackGain[i] = params.feedbackGainOff[i];
            currentLevel[i]= params.levelOff[i];
            currentCoupling[i] = params.coupling[i];
        }
    // std::cout << "end StringReso initialization   " << std::endl;
    }

StringReso::~StringReso()
  {

  }

//----------------------------------------------------------------
void StringReso::prepare(const juce::dsp::ProcessSpec spec, float minFreq)
  {
    // std::cout << "begin StringReso::prepare   ";

    processSpec = spec;
    setParams(params, true);

    // We set not isOn before to force the updating of other parameters
    // bool son = params.isOn;
    // setIsOn(!son, true);
    setIsOn(false, true);

    adsr1.setSampleRate(spec.sampleRate);

    for (int string=0; string<NUMSTRINGS; string++)
    {
        float delaySamples[4];
        delaySamples[0] = params.inPos[string]*params.stringPeriodInSamples;
        delaySamples[1] = 0.5*(params.outPos[string]-params.inPos[string])*params.stringPeriodInSamples;
        delaySamples[2] = delaySamples[1];
        delaySamples[3] = (1-params.outPos[string])*params.stringPeriodInSamples;
        for (int i=0; i<4; i++)
        {
            delayLine[4*string+i].setMaximumDelayInSamples(int(processSpec.sampleRate/minFreq));
            smoothDelaySamples[4*string+i].setCurrentAndTargetValue(delaySamples[i]);
            delayLine[4*string+i].prepare(processSpec);
        }
        fbFilter[string].coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,currentFeedbackFreq[string]);
        fbFilter[string].prepare(processSpec);

        sampler[string].setWaveByNumber(0, true);
        sampler[string].prepare(processSpec);
        sampler[string].stringNum = string;
    }
    // std::cout << "end StringReso::prepare   " << std::endl;
  }

void StringReso::process(juce::AudioBuffer<float>& inBuffer, juce::AudioBuffer<float>& outBuffer, int startSample, int numSamples)
{
    // Input and output buffers should be mono
    outBuffer.clear();

    float fbFreq[NUMSTRINGS], fbGain[NUMSTRINGS], level[NUMSTRINGS], input[NUMSTRINGS];

    for (int channel = 0; channel < inBuffer.getNumChannels(); ++channel)
    {
        auto* inChannelData = inBuffer.getReadPointer (channel);
        auto* outChannelData = outBuffer.getWritePointer (channel);

        for (int sample=startSample; sample<inBuffer.getNumSamples()-startSample; ++sample)
        {
            float adsr1val = adsr1.getNextSample();
            for (int string=0; string<NUMSTRINGS; string++)
            {
                input[string] = inChannelData[sample] + sampler[string].processNextSample();
                float coupling = smoothCoupling[string].getNextValue();
                float fOff = juce::jmin<float>(processSpec.sampleRate/2,processSpec.sampleRate*params.feedbackFreqOff[string]/params.stringPeriodInSamples);
                float fOn = juce::jmin<float>(processSpec.sampleRate/2,processSpec.sampleRate*params.feedbackFreqOn[string]/params.stringPeriodInSamples);
                fbFreq[string] = juce::jmap<float>(adsr1val,fOff, fOn);
                fbGain[string] = juce::jmap<float>(adsr1val,params.feedbackGainOff[string], params.feedbackGainOn[string]);
                level[string] = params.level[string]*juce::jmap<float>(adsr1val,params.levelOff[string], params.levelOn[string]);

                int dec = 4*string;

                fbFilter[string].coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(processSpec.sampleRate,fbFreq[string]);
                float loop = fbFilter[string].processSample(fbGain[string]*delayLine[3+dec].popSample(channel,smoothDelaySamples[3+dec].getNextValue()));
                float in0 = loop + 0.5*input[string] + coupling*previousOutput[string];
                float in1 = - delayLine[0+dec].popSample(channel,smoothDelaySamples[0+dec].getNextValue()) + 0.5*input[string] + coupling*previousOutput[string];
                float in2 = delayLine[1+dec].popSample(channel,smoothDelaySamples[1+dec].getNextValue());
                float in3 = - delayLine[2+dec].popSample(channel,smoothDelaySamples[2+dec].getNextValue());

                delayLine[0+dec].pushSample(channel,in0);
                delayLine[1+dec].pushSample(channel,in1);
                delayLine[2+dec].pushSample(channel,in2);
                delayLine[3+dec].pushSample(channel,in3);

                previousOutput[suivant(NUMSTRINGS,string+1)] = in1+in2;

                outChannelData[sample] += velocityLevelFactor*level[string]*(in1+in2);
            }
        }
    }
}

void StringReso::setADSR1(juce::ADSR::Parameters adsrParams)
{
    adsr1.setParameters(adsrParams);
}

void StringReso::setParams(StringReso::StringResoParams newParams, bool force)
{
    // std::cout << "     begin StringReso::setParams   " << std::endl;

    adsr1.setParameters(newParams.adsrParams1);
    
    for (int string=0; string<NUMSTRINGS; string++)
    {
        setPortamentoTime(newParams.portamento);
        // setSmoothTime(params.smoothTime);
        setInPos(string, newParams.inPos[string], force);
        setOutPos(string, newParams.outPos[string], force);
        setFreqCoarseFactor(string, newParams.freqCoarseFactor[string], force);
        setFreqFineFactor(string, newParams.freqFineFactor[string], force);
        setFeedbackFreqOn(string, newParams.feedbackFreqOn[string], force);
        setFeedbackFreqOff(string, newParams.feedbackFreqOff[string], force);
        setFeedbackGainOn(string, newParams.feedbackGainOn[string], force);
        setFeedbackGainOff(string, newParams.feedbackGainOff[string], force);
        setLevelOn(string, newParams.levelOn[string], force);
        setLevelOff(string, newParams.levelOff[string], force);
        setCoupling(string, newParams.coupling[string], force);    
    }
    params = newParams;

    // std::cout << "     end StringReso::setParams   " << std::endl;
}

//-----------------------------------------
// void StringReso::setSmoothTime(float time)
// {
//     if (!juce::approximatelyEqual<float>(time,params.smoothTime))
//     {
//         std::cout << "         begin StringReso::setSmoothTimeInSamples  time =  " << time << std::endl;

//         params.smoothTime = time;

//         for (int i=0; i<NUMSTRINGS; i++)
//         {
//             smoothFeedbackFreq[i].reset(processSpec.sampleRate,time);
//             smoothFeedbackGain[i].reset(processSpec.sampleRate,time);
//             smoothLevel[i].reset(processSpec.sampleRate,time);
//             smoothCoupling[i].reset(processSpec.sampleRate,time);
//         }
//         std::cout << "         end StringReso::setSmoothTimeInSamples  " << std::endl;
//     }
// }

//-----------------------------------------
void StringReso::setPortamentoTime(float time)
{
    if (!juce::approximatelyEqual<float>(time,params.portamento))
    {
        params.portamento = time;
        for (int i=0; i<4*NUMSTRINGS; i++)
        {
            smoothDelaySamples[i].reset(processSpec.sampleRate,time);
        }
    }
}


//--------------------------
StringReso::StringResoParams StringReso::getParams()
{
    // std::cout << "inPos" << params.inPos << std::endl;
    // std::cout << "outPos" << params.outPos << std::endl;
    // std::cout << "FbFreqOff" << params.feedbackFreqOff << std::endl;
    // std::cout << "FbFreqOn" << params.feedbackFreqOn << std::endl;
    // std::cout << "FbGainOff" << params.feedbackGainOff << std::endl;
    // std::cout << "FbGainOn" << params.feedbackGainOn << std::endl;
    return params;
}

//

void StringReso::setIsOn(bool on, bool force)
{
    if (on!=params.isOn)
    {
        params.isOn = on;
        if (on)
        {
            // std::cout << "     set note on params    " << std::endl;
            adsr1.noteOn();
            for (int string=0; string<NUMSTRINGS; string++)
            {
                sampler[string].start();
            }
        }
        else
        {
            // std::cout << "     set note off params   " << std::endl;
            adsr1.noteOff();
            for (int string=0; string<NUMSTRINGS; string++)
            {
                sampler[string].stop();
            }
        }
    }
    // std::cout << "     end StringReso::setIsOn    " << std::endl;
}

void StringReso::setSamplerLevel(int string, float lvl)
{
  sampler[string].setLevel(lvl);
}

void StringReso::setFeedbackGainOn(int string, float gain, bool force)
{
    params.feedbackGainOn[string] = gain;
}

void StringReso::setFeedbackGainOff(int string, float gain, bool force)
{
    params.feedbackGainOff[string] = gain;
}

void StringReso::setFeedbackFreqOn(int string, float freq, bool force)
{
    params.feedbackFreqOn[string] = freq;
}

void StringReso::setFeedbackFreqOff(int string, float freq, bool force)
{
    params.feedbackFreqOff[string] = freq;
}

void StringReso::setLevelOn(int string, float lvl, bool force)
{
    params.levelOn[string] = lvl;
}

void StringReso::setLevelOff(int string, float lvl, bool force)
{
    params.levelOff[string] = lvl;
}   

void StringReso::setLevel(int string, float lvl, bool force)
{
    params.level[string] = lvl;
}

void StringReso::setFreqCoarseFactor(int string, float fac, bool force)
{
    if (fac!=params.freqCoarseFactor[string])
    {
        params.freqCoarseFactor[string]=fac;
        setDelaySamples(string,force);
        setSamplerFreq(string);
    }
}

void StringReso::setFreqFineFactor(int string, float fac, bool force)
{
    if (fac!=params.freqFineFactor[string])
    {
        params.freqFineFactor[string]=fac;
        setDelaySamples(string,force);
        setSamplerFreq(string);
    }
}

void StringReso::setCoupling(int string, float fac, bool force)
{
    if (fac!=currentCoupling[string])
    {
        currentCoupling[string] = fac;
        if (force)
            smoothCoupling[string].setCurrentAndTargetValue(fac);
        else
            smoothCoupling[string].setTargetValue(fac);
    }
}

// ----------------------------------------------------------------

void StringReso::setStringPeriodInSamples(float period, bool force)
{
    if (period!=params.stringPeriodInSamples)
    {
        for (int string=0; string<NUMSTRINGS; string++)
        {
            setDelaySamples(string, force);
            setSamplerFreq(string);
        }
    }
}

void StringReso::setStringFreq(float freq, bool force)
{
    params.stringPeriodInSamples = processSpec.sampleRate/freq;
    for (int string=0; string<NUMSTRINGS; string++)
    {
        setDelaySamples(string, force);
        setSamplerFreq(string);
    }
}

void StringReso::setInPos(int string, float pos, bool force)
{
    if (pos!=params.inPos[string])
    {
        params.inPos[string] = pos;
        setDelaySamples(string, force);
    }
}

void StringReso::setOutPos(int string, float pos, bool force)
{
    if (pos!=params.outPos[string])
    {
        params.outPos[string] = pos;
        setDelaySamples(string, force);
    }
}

void StringReso::setDelaySamples(int string, bool force)
{
    float basePeriod = params.stringPeriodInSamples / powf(SEMITONE,params.freqCoarseFactor[string]+params.freqFineFactor[string]) ;
    float delaySamples[4];
    delaySamples[0] = params.inPos[string]*basePeriod;
    delaySamples[1] = 0.5*(params.outPos[string]-params.inPos[string])*basePeriod;
    delaySamples[2] = delaySamples[1];
    delaySamples[3] = (1-params.outPos[string])*basePeriod;

    if (force)
        {
            for (int i=0; i<4; i++)
                {
                    smoothDelaySamples[4*string+i].setCurrentAndTargetValue(delaySamples[i]);
                }
        }
    else
        for (int i=0; i<4; i++)
        {
            smoothDelaySamples[4*string+i].setTargetValue(delaySamples[i]);
        }

}

void StringReso::setVelocityLevel(float lvl)
{
    params.velocityLevel = lvl;
}

void StringReso::setVelocity(float vel)
{
    velocityLevelFactor = juce::jmap<float>(float(vel), 1-params.velocityLevel, 1) ;
    for (int string=0;string<NUMSTRINGS;string++)
        sampler[string].setVelocity(vel);
}


void StringReso::setSamplerFreq(int string)
{
    float freq = processSpec.sampleRate * powf(SEMITONE,params.freqCoarseFactor[string]+params.freqFineFactor[string])/params.stringPeriodInSamples;
    // std::cout << "StringReso::setSamplerFreq : Setting frequency of string " << string << " to  " << freq << std::endl;
    sampler[string].setPlayingFrequency(processSpec.sampleRate * powf(SEMITONE,params.freqCoarseFactor[string]+params.freqFineFactor[string])/params.stringPeriodInSamples);
}

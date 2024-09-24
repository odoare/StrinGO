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

std::string makeStringResoLfoParam(std::string param, int numlfo, int string)
{
    std::string s = "LFO";
    s.append(std::to_string(numlfo));
    s.append(" ");
    s.append(param);
    if (string>-1) s.append(std::to_string(string));
    return s;
}

std::string makeStringResoParam(std::string param, int string)
{
    std::string s = param;
    if (string>-1) s.append(std::to_string(string));
    return s;
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
        params.stringPeriodInSamples = 100.f;
        params.isOn=false;
        params.velocityLevel = 0.f;
        for (int s=0;s<NUMSTRINGS;s++)
        {
            params.freqCoarseFactor[s] = 0.f;
            params.freqFineFactor[s] = -0.05f;
            params.inPos[s] = 0.1f;
            params.outPos[s] = 0.9f;
            params.feedbackGainOn[s] = 0.99f;
            params.feedbackFreqOn[s] = 50.f;
            params.levelOn[s] = 1.f;
            params.feedbackGainOff[s] = 0.5f;
            params.feedbackFreqOff[s] = 49.5f;
            params.levelOff[s] = 0.2f;
            params.coupling[s] = 0.f;//0.1f;
            currentFeedbackFreq[s] = params.feedbackFreqOff[s];
            currentFeedbackGain[s] = params.feedbackGainOff[s];
            currentLevel[s]= params.levelOff[s];
            currentCoupling[s] = params.coupling[s];
        }
        for (int l=0; l<NUMLFO; l++)
        {
            lfo[l].initialise([] (float x) { return std::sin (x); }, 100);
            params.lfoParams[l].freq = 0.5f;
            params.lfoParams[l].amp = 0.2f;
            for (int s=0;s<NUMSTRINGS;s++)
            {
                params.lfoParams[l].level[s] = false;
                params.lfoParams[l].fine[s] = false;
                params.lfoParams[l].inPos[s] = false;
                params.lfoParams[l].outPos[s] = false;
            }
            params.lfoParams[l].samplerFreq = false;
            params.lfoParams[l].samplerLevel = false;                
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

    for (int l=0;l<NUMLFO;l++)
    {
        lfo[l].prepare(spec);
        lfo[l].setFrequency(params.lfoParams[l].freq);
        for (int s=0;s<NUMSTRINGS;s++)
        {
            setLfoCoarse(l,s,params.lfoParams[l].coarse[s]);
            setLfoFine(l,s,params.lfoParams[l].fine[s]);
            setLfoInPos(l,s,params.lfoParams[l].inPos[s]);
            setLfoOutPos(l,s,params.lfoParams[l].outPos[s]);
        }
    }

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
        float adsr1val;
        float vallfolevel, vallfosamplerlevel, vallfosamplerfreq;
        bool needsStringUpdate, needsSamplerFreqUpdate;

        for (int sample=startSample; sample<inBuffer.getNumSamples()-startSample; ++sample)
        {
            if (channel==0)
            {
                adsr1val = adsr1.getNextSample();
                vallfosamplerlevel = 1.f;
                vallfosamplerfreq = 1.f;
                for (int l=0;l<NUMLFO;l++)
                {
                    lfoVal[l] = lfo[l].processSample(0.f);
                    //std::cout << "lfo " << i << "  freq : " << lfo[i].getFrequency() << "\n";
                    //std::cout << "lfo " << i << "  value : " << lfoVal[i] << "\n";
                    vallfosamplerlevel *= params.lfoParams[l].samplerLevel ? 1.f-(.5f+.5f*lfoVal[l])*params.lfoParams[l].amp : 1.f ;
                    vallfosamplerfreq *= params.lfoParams[l].samplerFreq ? 1.f-(.5f+.5f*lfoVal[l])*params.lfoParams[l].amp : 1.f ;
                }

                needsSamplerFreqUpdate=false;
                for (int l=0;l<NUMLFO;l++) needsSamplerFreqUpdate = needsSamplerFreqUpdate || params.lfoParams[l].samplerFreq;             
            }

            for (int string=0; string<NUMSTRINGS; string++)
            {
                if (channel==0)
                {
                    vallfolevel = 1.f;
                    for (int l=0; l<NUMLFO; l++)
                        vallfolevel *= params.lfoParams[l].level[string] ? 1.f-(.5f+.5f*lfoVal[l])*params.lfoParams[l].amp : 1.f ;
                }

                needsStringUpdate = false;                
                for (int l=0;l<NUMLFO;l++)
                {
                    needsStringUpdate = needsStringUpdate
                        || params.lfoParams[l].fine[string]
                        || params.lfoParams[l].coarse[string]
                        || params.lfoParams[l].inPos[string]
                        || params.lfoParams[l].outPos[string];
                }
                if  (sample%LFOSAMPLESUPDATE==0)
                {
                    if (needsStringUpdate) setDelaySamples(string);
                    // std::cout << "Update Delay samples... \n";
                    if (needsSamplerFreqUpdate) sampler[string].setFilterFreqLfoFactor(vallfosamplerfreq);
                }

                input[string] = inChannelData[sample] + sampler[string].processNextSample()*vallfosamplerlevel;
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

                // if (sample%LFOSAMPLESUPDATE==0 )
                // {
                //     std::cout << "levellfo 0 : " << params.lfoParams[0].level[string] << "\n";
                //     std::cout << "vallfos : " << vallfos << "\n";
                // }

                outChannelData[sample] += velocityLevelFactor
                                            *level[string]
                                            *(in1+in2)
                                            *vallfolevel;
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
    
    for (int l=0;l<NUMLFO;l++)
    {
        lfo[l].setFrequency(newParams.lfoParams[l].freq);
    }

    for (int string=0; string<NUMSTRINGS; string++)
    {
        setPortamentoTime(newParams.portamento);
        setInPos(string, newParams.inPos[string], force);
        setOutPos(string, newParams.outPos[string], force);
        setFreqCoarseFactor(string, newParams.freqCoarseFactor[string], force);
        setFreqFineFactor(string, newParams.freqFineFactor[string], force);
        setFeedbackFreqOn(string, newParams.feedbackFreqOn[string]);
        setFeedbackFreqOff(string, newParams.feedbackFreqOff[string]);
        setFeedbackGainOn(string, newParams.feedbackGainOn[string]);
        setFeedbackGainOff(string, newParams.feedbackGainOff[string]);
        setLevelOn(string, newParams.levelOn[string]);
        setLevelOff(string, newParams.levelOff[string]);
        setCoupling(string, newParams.coupling[string], force);    
    }
    params = newParams;

    // std::cout << "     end StringReso::setParams   " << std::endl;
}

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

void StringReso::setFeedbackGainOn(int string, float gain)
{
    params.feedbackGainOn[string] = gain;
}

void StringReso::setFeedbackGainOff(int string, float gain)
{
    params.feedbackGainOff[string] = gain;
}

void StringReso::setFeedbackFreqOn(int string, float freq)
{
    params.feedbackFreqOn[string] = freq;
}

void StringReso::setFeedbackFreqOff(int string, float freq)
{
    params.feedbackFreqOff[string] = freq;
}

void StringReso::setLevelOn(int string, float lvl)
{
    params.levelOn[string] = lvl;
}

void StringReso::setLevelOff(int string, float lvl)
{
    params.levelOff[string] = lvl;
}   

void StringReso::setLevel(int string, float lvl)
{
    params.level[string] = lvl;
}

void StringReso::setFreqCoarseFactor(int string, float fac, bool force)
{
    if (fac!=params.freqCoarseFactor[string])
    {
        params.freqCoarseFactor[string]=fac;
        coarseFreqDistToBoundary[string]=juce::jmin<float>(fac-COARSEMIN,COARSEMAX-fac);        
        setDelaySamples(string,force);
        setSamplerFreq(string);
        // std::cout << "coarse factor set to " << fac << " for string " << string << "\n";
        // std::cout << "coarseFreqDistToBoundary " << string << " : " << coarseFreqDistToBoundary[string] << "\n";
    }
}

void StringReso::setFreqFineFactor(int string, float fac, bool force)
{
    if (fac!=params.freqFineFactor[string])
    {
        params.freqFineFactor[string]=fac;
        fineFreqDistToBoundary[string]=juce::jmin<float>(fac-FINEMIN,FINEMAX-fac);        
        setDelaySamples(string,force);
        setSamplerFreq(string);

        // std::cout << "fineFreqDistToBoundary " << string << " : " << fineFreqDistToBoundary[string] << "\n";

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
        inPosDistToBoundary[string] = juce::jmin<float>(pos-INPOSMIN,INPOSMAX-pos);        
        setDelaySamples(string, force);
    }
}

void StringReso::setOutPos(int string, float pos, bool force)
{
    if (pos!=params.outPos[string])
    {
        params.outPos[string] = pos;
        outPosDistToBoundary[string] = juce::jmin<float>(pos-OUTPOSMIN,OUTPOSMAX-pos);        
        setDelaySamples(string, force);
    }
}

void StringReso::setDelaySamples(int string, bool force)
{
    float flfofac=1.f, clfofac=1.f, inlfofac=1.f, outlfofac=1.f;

    for (int l=0;l<NUMLFO;l++)
    {
        float vv = 1.f + params.lfoParams[l].amp*lfoVal[l] ;
        flfofac *= params.lfoParams[l].fine[string] ?  vv : 1.f; 
        clfofac *= params.lfoParams[l].coarse[string] ?  vv : 1.f; 
        inlfofac *= params.lfoParams[l].inPos[string] ? vv : 1.f;
        outlfofac *= params.lfoParams[l].outPos[string] ? vv : 1.f;
    }
    float fff = params.freqFineFactor[string] - fineFreqDistToBoundary[string] * (1.f-flfofac);
    float fcf = params.freqCoarseFactor[string] - coarseFreqDistToBoundary[string] * (1.f-clfofac);
    float inP = params.inPos[string] - inPosDistToBoundary[string] * (1.f-inlfofac);
    float outP = params.outPos[string] - outPosDistToBoundary[string] * (1.f-outlfofac);

    // std::cout << "fineFreqDistToBoundary " << string << " : " << fineFreqDistToBoundary[string] << "\n";
    // std::cout << "flfofac : " << flfofac << "\n";
    // std::cout << "fff : " << fff << "\n";

    float basePeriod = params.stringPeriodInSamples / powf(SEMITONE,fff+fcf);
    float delaySamples[4];
    delaySamples[0] = inP*basePeriod;
    delaySamples[1] = 0.5*(outP-inP)*basePeriod;
    delaySamples[2] = delaySamples[1];
    delaySamples[3] = (1-outP)*basePeriod;

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

void StringReso::setLfoFreq(int num, float freq)
{
    params.lfoParams[num].freq = freq;
    lfo[num].setFrequency(freq);
    //std::cout << "lfo Freq " << num << " : " << params.lfoParams[num].freq << "\n";
}

void StringReso::setLfoAmp(int num, float val)
{
    params.lfoParams[num].amp = val;
    // std::cout << "lfo Amp " << num << " : " << params.lfoParams[num].amp << "\n";
}

void StringReso::setLfoFine(int num, int string, bool onoff)
{
    params.lfoParams[num].fine[string] = onoff;
    // std::cout << "LFO fine " << num << " - string " << string << " : " << onoff << "\n"; 
}

void StringReso::setLfoCoarse(int num, int string, bool onoff)
{
    params.lfoParams[num].coarse[string] = onoff;
}

void StringReso::setLfoLevel(int num, int string, bool onoff)
{
    params.lfoParams[num].level[string] = onoff;
    // std::cout << "LFO level " << num << " - string " << string << " : " << onoff << "\n"; 
}
void StringReso::setLfoInPos(int num, int string, bool onoff)
{
    params.lfoParams[num].inPos[string] = onoff;
}
void StringReso::setLfoOutPos(int num, int string, bool onoff)
{
    params.lfoParams[num].outPos[string] = onoff;
}

void StringReso::setLfoSamplerLPFreq(int num, bool onoff)
{
    params.lfoParams[num].samplerFreq = onoff;
}

void StringReso::setLfoSamplerLevel(int num, bool onoff)
{
    params.lfoParams[num].samplerLevel = onoff;
}

void StringReso::setSamplerFreq(int string)
{
    float freq = processSpec.sampleRate * powf(SEMITONE,params.freqCoarseFactor[string]+params.freqFineFactor[string])/params.stringPeriodInSamples;
    // std::cout << "StringReso::setSamplerFreq : Setting frequency of string " << string << " to  " << freq << std::endl;
    sampler[string].setPlayingFrequency(processSpec.sampleRate * powf(SEMITONE,params.freqCoarseFactor[string]+params.freqFineFactor[string])/params.stringPeriodInSamples);
}

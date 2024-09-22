/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MySynthAudioProcessor::MySynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

MySynthAudioProcessor::~MySynthAudioProcessor()
{
}

//==============================================================================
const juce::String MySynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MySynthAudioProcessor::acceptsMidi() const
{
   return true;
}

bool MySynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MySynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MySynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MySynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MySynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MySynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MySynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void MySynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MySynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    processSampleRate = sampleRate;
    processBlockLength = samplesPerBlock;

    smoothDirectOut.reset(0.01*sampleRate);
    smoothDirectOut.setCurrentAndTargetValue(0.f);

    smoothOutputGain.reset(0.01*sampleRate);
    smoothOutputGain.setCurrentAndTargetValue(0.f);

    smoothOutputLevel.reset(0.01*sampleRate);
    smoothOutputLevel.setCurrentAndTargetValue(0.f);

    sharedInputBuffer.setSize(getTotalNumInputChannels(), samplesPerBlock, false, true, false);
    
    for (int i=0; i<synth.getNumVoices(); ++i)
    {
       if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
       {
        voice->prepareToPlay (&sharedInputBuffer, sampleRate, samplesPerBlock, getTotalNumInputChannels());
       }
    }
}

void MySynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

// #ifndef JucePlugin_PreferredChannelConfigurations
bool MySynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
//   #if JucePlugin_IsMidiEffect
//     juce::ignoreUnused (layouts);
//     return true;
//   #else
//     // This is the place where you check if the layout is supported.
//     // In this template code we only support mono or stereo.
//     // Some plugin hosts, such as certain GarageBand versions, will only
//     // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     || layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono())
        return false;

//     // This checks if the input layout matches the output layout
//    #if ! JucePlugin_IsSynth
//     if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
//         return false;
//    #endif

    return true;
//   #endif
}
// #endif

void MySynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // std::cout << "In Process block ... ";
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Voice number updating
    auto nv = apvts.getRawParameterValue("Voices")->load();
    auto anv = synth.getNumVoices();
    if (nv!=anv)
    {
        if (anv>nv)
            for (int i=0; i<anv-nv; i++)
            {
                synth.removeVoice(anv-i-1);
                std::cout << "Voice " << anv-i << " removed" << std::endl;
            }
                
        if (anv<nv)
            {
                for (int i=0; i<nv-anv; i++)
                {
                    synth.addVoice(new SynthVoice());

                    // Each new allocated voice has to be prepared
                    if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(anv+i)))
                        {
                            voice->prepareToPlay (&sharedInputBuffer, processSampleRate,processBlockLength, getTotalNumInputChannels());
                            for (int string=0; string<NUMSTRINGS; string++)
                                    voice->stringReso.sampler[string].setWaveByNumber(apvts.getRawParameterValue("Attack Sample")->load(),true);
                        }
                }
            }
    }

    sharedInputBuffer.makeCopyOf<float>(buffer,true);
    buffer.clear();

    for (int i=0; i<synth.getNumVoices(); ++i)
    {
       if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
       {
        for (int string=0; string<NUMSTRINGS; string++)
        {
            voice->stringReso.setFeedbackGainOn(string,apvts.getRawParameterValue("Fb Gain On")->load());
            voice->stringReso.setFeedbackFreqOn(string,apvts.getRawParameterValue("Fb Freq On")->load());
            voice->stringReso.setLevelOn(string,juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Level On")->load()));
            voice->stringReso.setFeedbackGainOff(string,apvts.getRawParameterValue("Fb Gain Off")->load());
            voice->stringReso.setFeedbackFreqOff(string,apvts.getRawParameterValue("Fb Freq Off")->load());
            voice->stringReso.setLevelOff(string,juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Level Off")->load()));
            voice->stringReso.setInPos(string,apvts.getRawParameterValue("In Pos")->load());
            voice->stringReso.setOutPos(string,apvts.getRawParameterValue("Out Pos")->load());   
            voice->stringReso.setSamplerLevel(string, juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Sample Level")->load()));
            voice->stringReso.sampler[string].setWaveByNumber(apvts.getRawParameterValue("Attack Sample")->load());
            voice->stringReso.sampler[string].setFilterFreqFactor(apvts.getRawParameterValue("Sample LP Freq")->load());
            voice->stringReso.sampler[string].setFilterFreqVelocityInfluence(apvts.getRawParameterValue("Velocity Sample Freq")->load());
            voice->stringReso.sampler[string].setLevelVelocityInfluence(apvts.getRawParameterValue("Velocity Sample Level")->load());
            voice->stringReso.sampler[string].setLooping(apvts.getRawParameterValue("Loop Sample")->load());
            voice->stringReso.sampler[string].setAttack(apvts.getRawParameterValue("AS")->load());
            voice->stringReso.sampler[string].setDecay(apvts.getRawParameterValue("DS")->load());
            voice->stringReso.sampler[string].setSustain(apvts.getRawParameterValue("SS")->load());
            voice->stringReso.sampler[string].setRelease(apvts.getRawParameterValue("RS")->load());
        }
        
        voice->stringReso.setFreqCoarseFactor(0,apvts.getRawParameterValue("Coarse1")->load());
        voice->stringReso.setFreqFineFactor(0,apvts.getRawParameterValue("Fine1")->load());
        voice->stringReso.setFreqCoarseFactor(1,apvts.getRawParameterValue("Coarse2")->load());
        voice->stringReso.setFreqFineFactor(1,apvts.getRawParameterValue("Fine2")->load());
        voice->stringReso.setPortamentoTime(apvts.getRawParameterValue("Porta Time")->load());

        voice->stringReso.setLevel(0,juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Level1")->load()));
        voice->stringReso.setLevel(1,juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Level2")->load()));
        
        float c = apvts.getRawParameterValue("Coupling")->load();
        if (c>0.f)
        {
            voice->stringReso.setCoupling(0,0.f);
            voice->stringReso.setCoupling(1,c);
        }
        else
        {
            voice->stringReso.setCoupling(0,c);
            voice->stringReso.setCoupling(1,0);
        }

        voice->adsrNParams.attack = apvts.getRawParameterValue("AN")->load();
        voice->adsrNParams.decay = apvts.getRawParameterValue("DN")->load();
        voice->adsrNParams.sustain = apvts.getRawParameterValue("SN")->load();
        voice->adsrNParams.release = apvts.getRawParameterValue("RN")->load();
        voice->adsrN.setParameters(voice->adsrNParams);

        voice->adsrCParams.attack = apvts.getRawParameterValue("AC")->load();
        voice->adsrCParams.decay = apvts.getRawParameterValue("DC")->load();
        voice->adsrCParams.sustain = apvts.getRawParameterValue("SC")->load();
        voice->adsrCParams.release = apvts.getRawParameterValue("RC")->load();
        voice->adsrC.setParameters(voice->adsrCParams);

        voice->stringReso.adsr1Params.attack = apvts.getRawParameterValue("A")->load();
        voice->stringReso.adsr1Params.decay = apvts.getRawParameterValue("D")->load();
        voice->stringReso.adsr1Params.sustain = apvts.getRawParameterValue("S")->load();
        voice->stringReso.adsr1Params.release = apvts.getRawParameterValue("R")->load();
        voice->stringReso.adsr1.setParameters(voice->stringReso.adsr1Params);

        voice->setNoiseLPFilterFreq(apvts.getRawParameterValue("Noise LP Freq")->load());
        voice->setNoiseHPFilterFreq(apvts.getRawParameterValue("Noise HP Freq")->load());
        voice->setNoiseLevel(juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Noise Level")->load()));
        voice->setCrackDensity(int(apvts.getRawParameterValue("Crack Density")->load()));
        voice->setCrackLPFilterFreq(apvts.getRawParameterValue("Crack LP Freq")->load());
        voice->setCrackLevel(juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Crack Level")->load()));

        voice->setNoiseLPFilterFreqVelocityInfluence(apvts.getRawParameterValue("Velocity Noise Freq")->load());
        voice->setCrackLPFilterFreqVelocityInfluence(apvts.getRawParameterValue("Velocity Crack Freq")->load());
        voice->setNoiseLevelVelocityInfluence(apvts.getRawParameterValue("Velocity Noise Level")->load());
        voice->setCrackLevelVelocityInfluence(apvts.getRawParameterValue("Velocity Crack Level")->load());

        voice->smoothInputGain.setTargetValue(juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Input Gain")->load()));
       } 
    }

    smoothDirectOut.setTargetValue(juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Direct Out")->load()));
    smoothOutputGain.setTargetValue(juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Output Gain")->load()));
    smoothOutputLevel.setTargetValue(juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Output Level")->load()));

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // TANH saturation of the output buffer
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        auto* channelDataIn = sharedInputBuffer.getWritePointer (channel);
        for (int sample=0; sample<buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = smoothDirectOut.getNextValue()*channelDataIn[sample] + smoothOutputLevel.getNextValue() * tanh(smoothOutputGain.getNextValue() * channelData[sample]);
        }
    }
}

//==============================================================================
bool MySynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MySynthAudioProcessor::createEditor()
{
    return new MySynthAudioProcessorEditor (*this);
    // return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void MySynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void MySynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data,sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MySynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout MySynthAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("A","A",juce::NormalisableRange<float>(0.000f,10.f,1e-3f,0.4f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("D","D",juce::NormalisableRange<float>(0.000f,10.f,1e-3f,0.4f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("S","S",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,0.4f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("R","R",juce::NormalisableRange<float>(0.000f,10.f,1e-3f,0.4f),0.5f));

    layout.add(std::make_unique<juce::AudioParameterInt>("Voices","Voices",1,6,1));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Porta Time","Porta Time",juce::NormalisableRange<float>(0.0001f,1.f,1e-3f,1.f),0.01f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Level1","Level1",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Coarse1","Coarse1",juce::NormalisableRange<float>(-12.f,12.f,1.f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Fine1","Fine1",juce::NormalisableRange<float>(-1.f,1.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Level2","Level2",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Coarse2","Coarse2",juce::NormalisableRange<float>(-12.f,12.f,1.f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Fine2","Fine2",juce::NormalisableRange<float>(-1.f,1.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("In Pos","In Pos",juce::NormalisableRange<float>(0.05f,0.45f,1e-3f,1.f),0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Out Pos","Out Pos",juce::NormalisableRange<float>(0.55f,0.95f,1e-3f,1.f),0.9f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("In Pos 2","In Pos 2",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,1.f),1.f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Out Pos 2","Out Pos 2",juce::NormalisableRange<float>(0.001f,3.f,1e-3f,1.f),0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Freq On","Fb Freq On",juce::NormalisableRange<float>(1.f,100.f,1e-1f,1.f),80.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Gain On","Fb Gain On",juce::NormalisableRange<float>(0.f,1.f,1e-3f,5.f),0.99f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Level On","Level On",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),-3.f));

    // layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Freq on 2","Fb Freq on 2",juce::NormalisableRange<float>(0.1f,100.f,1e-1f,1.f),40.f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Gain on 2","Fb Gain on 2",juce::NormalisableRange<float>(0.f,1.f,1e-3f,1.f),0.95f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Level on 2","Level on 2",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),-3.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Freq Off","Fb Freq Off",juce::NormalisableRange<float>(1.f,100.f,1e-1f,1.f),20.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Gain Off","Fb Gain Off",juce::NormalisableRange<float>(0.f,1.f,1e-3f,5.f),0.9f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Level Off","Level Off",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),-3.f));

    // layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Freq off 2","Fb Freq off 2",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,1.f),1.f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Gain off 2","Fb Gain off 2",juce::NormalisableRange<float>(0.001f,3.f,1e-3f,1.f),0.5f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Level off 2","Level off 2",juce::NormalisableRange<float>(0.001f,3.f,1e-3f,1.f),0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Coupling","Coupling",juce::NormalisableRange<float>(-0.5f,0.5f,1e-3f,1.f),0.0f));

    layout.add(std::make_unique<juce::AudioParameterChoice>("Attack Sample","Attack Sample",juce::StringArray WAVECHOICES, 0));
    layout.add(std::make_unique<juce::AudioParameterBool>("Loop Sample","Loop Sample", false));
    layout.add(std::make_unique<juce::AudioParameterFloat>("AS","AS",juce::NormalisableRange<float>(0.000f,10.f,1e-3f,0.4f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DS","DS",juce::NormalisableRange<float>(0.000f,10.f,1e-3f,0.4f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("SS","SS",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,0.4f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("RS","RS",juce::NormalisableRange<float>(0.000f,10.f,1e-3f,0.4f),0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Sample LP Freq","Sample LP Freq",juce::NormalisableRange<float>(0.75f,20.f,1e-2f,1.f),20.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Sample Level","Sample Level",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),-3.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Noise LP Freq","Noise LP Freq",juce::NormalisableRange<float>(20.f,20000.f,1.f,0.3f),20000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Noise HP Freq","Noise HP Freq",juce::NormalisableRange<float>(20.f,20000.f,1.f,0.3f),20.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Noise Level","Noise Level",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),-24.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("AN","AN",juce::NormalisableRange<float>(0.000f,10.f,1e-3f,0.4f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DN","DN",juce::NormalisableRange<float>(0.0001f,5.f,1e-3f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("SN","SN",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,1.f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("RN","RN",juce::NormalisableRange<float>(0.0001f,10.f,1e-3f,1.f),0.1f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Crack Density","Crack Density",juce::NormalisableRange<float>(0.f,10000.f,1.f,1.f),60.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Crack LP Freq","Crack LP Freq",juce::NormalisableRange<float>(20.f,20000.f,1.f,0.3f),20000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Crack Level","Crack Level",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),-6.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("AC","AC",juce::NormalisableRange<float>(0.000f,10.f,1e-3f,0.4f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DC","DC",juce::NormalisableRange<float>(0.000f,10.f,1e-3f,0.4f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("SC","SC",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,0.4f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("RC","RC",juce::NormalisableRange<float>(0.000f,10.f,1e-3f,0.4f),0.1f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Velocity Sample Level","Velocity Sample Level",juce::NormalisableRange<float>(0.f,1.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Velocity Sample Freq","Velocity Sample Freq",juce::NormalisableRange<float>(0.f,1.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Velocity Noise Level","Velocity Noise Level",juce::NormalisableRange<float>(0.f,1.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Velocity Noise Freq","Velocity Noise Freq",juce::NormalisableRange<float>(0.f,1.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Velocity Crack Level","Velocity Crack Level",juce::NormalisableRange<float>(0.f,1.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Velocity Crack Freq","Velocity Crack Freq",juce::NormalisableRange<float>(0.f,1.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Output Gain","Output Gain",juce::NormalisableRange<float>(-60.f,12.f,1e-2f,1.f),-3.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Output Level","Output Level",juce::NormalisableRange<float>(-60.f,12.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Input Gain","Input Gain",juce::NormalisableRange<float>(-60.f,12.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Direct Out","Direct Out",juce::NormalisableRange<float>(-60.f,12.f,1e-2f,1.f),-60.f));

    return layout;
}
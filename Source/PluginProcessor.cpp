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
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
    factoryPresets = FactoryPresets::getAvailablePresets();
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
    // Return number of factory presets, or 1 if there are none.
    return factoryPresets.size() + 1;
}

int MySynthAudioProcessor::getCurrentProgram()
{
    return currentProgram;
}

void MySynthAudioProcessor::setCurrentProgram (int index)
{
    if (! juce::isPositiveAndBelow (index, getNumPrograms()))
        return;

    currentProgram = index;

    if (index < factoryPresets.size())
    {
        // It's a factory preset, so load it from the XML data.
        juce::ScopedValueSetter<bool> loading (isLoadingPreset, true);

        const auto& preset = factoryPresets.getReference(index);
        auto xml = juce::XmlDocument::parse(juce::String::fromUTF8(preset.data, preset.dataSize));

        if (xml != nullptr && xml->hasTagName(apvts.state.getType()))
        {
            apvts.replaceState(juce::ValueTree::fromXml(*xml));
        }
        else
        {
            jassertfalse; // Preset data is malformed!
        }
    }
    // If the index is for the "User Preset", we do nothing, as its state
    // is managed by the host and has already been loaded via setStateInformation.
}

const juce::String MySynthAudioProcessor::getProgramName (int index)
{
    if (juce::isPositiveAndBelow (index, factoryPresets.size()))
        return factoryPresets.getUnchecked(index).name;

    if (index == factoryPresets.size())
        return "User Preset";

    return {};
}

void MySynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MySynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Load the initial preset once everything is ready.
    if (!isInitialPresetLoaded && factoryPresets.size() > 0)
    {
        setCurrentProgram(0);
        isInitialPresetLoaded = true;
    }

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
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()
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

    // for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //     buffer.clear (i, 0, buffer.getNumSamples());

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

            voice->stringReso.setLevel(string,juce::Decibels::decibelsToGain(apvts.getRawParameterValue(makeStringResoParam("Level",string+1))->load()));
            voice->stringReso.setPan(string,apvts.getRawParameterValue(makeStringResoParam("Pan",string+1))->load());
            voice->stringReso.setFreqCoarseFactor(string,apvts.getRawParameterValue(makeStringResoParam("Coarse",string+1))->load());
            voice->stringReso.setFreqFineFactor(string,apvts.getRawParameterValue(makeStringResoParam("Fine",string+1))->load());

        }
        for (int l=0; l<NUMLFO; l++)
        {
            voice->stringReso.setLfoFreq(l,apvts.getRawParameterValue(makeStringResoLfoParam("Freq",l+1,-1))->load());
            voice->stringReso.setLfoAmp(l,apvts.getRawParameterValue(makeStringResoLfoParam("Amp",l+1,-1))->load());

            voice->stringReso.setLfoSamplerLevel(l,apvts.getRawParameterValue(makeStringResoLfoParam("SamplerLevel",l+1,-1))->load());
            voice->stringReso.setLfoSamplerLPFreq(l,apvts.getRawParameterValue(makeStringResoLfoParam("SamplerLPFreq",l+1,-1))->load());

            voice->stringReso.setLfoNoiseLevel(l,apvts.getRawParameterValue(makeStringResoLfoParam("NoiseLevel",l+1,-1))->load());
            voice->stringReso.setLfoNoiseLPFreq(l,apvts.getRawParameterValue(makeStringResoLfoParam("NoiseLPFreq",l+1,-1))->load());
            voice->stringReso.setLfoNoiseHPFreq(l,apvts.getRawParameterValue(makeStringResoLfoParam("NoiseHPFreq",l+1,-1))->load());

            voice->stringReso.setLfoCrackLevel(l,apvts.getRawParameterValue(makeStringResoLfoParam("CrackLevel",l+1,-1))->load());
            voice->stringReso.setLfoCrackLPFreq(l,apvts.getRawParameterValue(makeStringResoLfoParam("CrackLPFreq",l+1,-1))->load());
            voice->stringReso.setLfoCrackDensity(l,apvts.getRawParameterValue(makeStringResoLfoParam("CrackDensity",l+1,-1))->load());

            for (int string=0; string<NUMSTRINGS; string++)
            {
                voice->stringReso.setLfoFine(l,string,apvts.getRawParameterValue(makeStringResoLfoParam("Fine",l+1,string+1))->load());
                voice->stringReso.setLfoCoarse(l,string,apvts.getRawParameterValue(makeStringResoLfoParam("Coarse",l+1,string+1))->load());
                voice->stringReso.setLfoLevel(l,string,apvts.getRawParameterValue(makeStringResoLfoParam("Level",l+1,string+1))->load());
                voice->stringReso.setLfoPan(l,string,apvts.getRawParameterValue(makeStringResoLfoParam("Pan",l+1,string+1))->load());
                voice->stringReso.setLfoInPos(l,string,apvts.getRawParameterValue(makeStringResoLfoParam("InPos",l+1,string+1))->load());
                voice->stringReso.setLfoOutPos(l,string,apvts.getRawParameterValue(makeStringResoLfoParam("OutPos",l+1,string+1))->load());
            }
        }
        
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

        juce::ADSR::Parameters adsrp;

        adsrp.attack = apvts.getRawParameterValue("AN")->load();
        adsrp.decay = apvts.getRawParameterValue("DN")->load();
        adsrp.sustain = apvts.getRawParameterValue("SN")->load();
        adsrp.release = apvts.getRawParameterValue("RN")->load();
        voice->stringReso.setADSRN(adsrp);

        adsrp.attack = apvts.getRawParameterValue("AC")->load();
        adsrp.decay = apvts.getRawParameterValue("DC")->load();
        adsrp.sustain = apvts.getRawParameterValue("SC")->load();
        adsrp.release = apvts.getRawParameterValue("RC")->load();
        voice->stringReso.setADSRC(adsrp);

        adsrp.attack = apvts.getRawParameterValue("A")->load();
        adsrp.decay = apvts.getRawParameterValue("D")->load();
        adsrp.sustain = apvts.getRawParameterValue("S")->load();
        adsrp.release = apvts.getRawParameterValue("R")->load();
        voice->stringReso.setADSR1(adsrp);

        voice->stringReso.setNoiseLPFilterFreq(apvts.getRawParameterValue("Noise LP Freq")->load());
        voice->stringReso.setNoiseHPFilterFreq(apvts.getRawParameterValue("Noise HP Freq")->load());
        voice->stringReso.setNoiseLevel(juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Noise Level")->load()));
        voice->stringReso.setCrackDensity(int(apvts.getRawParameterValue("Crack Density")->load()));
        voice->stringReso.setCrackLPFilterFreq(apvts.getRawParameterValue("Crack LP Freq")->load());
        voice->stringReso.setCrackLevel(juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Crack Level")->load()));

        voice->stringReso.setNoiseLPFilterFreqVelocityInfluence(apvts.getRawParameterValue("Velocity Noise Freq")->load());
        voice->stringReso.setCrackLPFilterFreqVelocityInfluence(apvts.getRawParameterValue("Velocity Crack Freq")->load());
        voice->stringReso.setNoiseLevelVelocityInfluence(apvts.getRawParameterValue("Velocity Noise Level")->load());
        voice->stringReso.setCrackLevelVelocityInfluence(apvts.getRawParameterValue("Velocity Crack Level")->load());

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
        auto* channelDataIn = sharedInputBuffer.getReadPointer (0);
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
    // Create a temporary ValueTree to hold the state
    juce::ValueTree state = apvts.copyState();
    // Store the current program index in the state
    state.setProperty ("currentProgram", currentProgram, nullptr);

    // Convert the ValueTree to XML and write it to the memory block
    juce::MemoryOutputStream mos(destData, true);
    state.writeToStream(mos);
}

void MySynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Create a ValueTree from the binary data
    juce::ValueTree tree = juce::ValueTree::readFromData (data, sizeInBytes);

    if (tree.isValid())
    {
        // Restore the parameters
        apvts.replaceState (tree);

        // Restore the current program index
        int programIndex = tree.getProperty ("currentProgram", 0);
        // We call setCurrentProgram to ensure the correct preset is loaded if it's a factory one,
        // or the index is correctly set for a user preset.
        if (currentProgram != programIndex)
        {
            setCurrentProgram (programIndex);
        }
    }
}


void MySynthAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    // Any parameter change makes the preset "dirty" (a user preset).
    // We check the isLoadingPreset flag to avoid this when loading a preset.
    if (!isLoadingPreset)
    {
        if (currentProgram < factoryPresets.size()) {
            currentProgram = factoryPresets.size();
        }
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
    layout.add(std::make_unique<juce::AudioParameterFloat>("Pan1","Pan1",juce::NormalisableRange<float>(-1.f,1.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Coarse1","Coarse1",juce::NormalisableRange<float>(-12.f,12.f,1.f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Fine1","Fine1",juce::NormalisableRange<float>(-1.f,1.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Level2","Level2",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Pan2","Pan2",juce::NormalisableRange<float>(-1.f,1.f,1e-2f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Coarse2","Coarse2",juce::NormalisableRange<float>(-12.f,12.f,1.f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Fine2","Fine2",juce::NormalisableRange<float>(-1.f,1.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("In Pos","In Pos",juce::NormalisableRange<float>(INPOSMIN,INPOSMAX,1e-3f,1.f),0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Out Pos","Out Pos",juce::NormalisableRange<float>(OUTPOSMIN,OUTPOSMAX,1e-3f,1.f),0.9f));
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

    layout.add(std::make_unique<juce::AudioParameterFloat>("Coupling","Coupling",juce::NormalisableRange<float>(-MAXCOUPLING,MAXCOUPLING,1e-3f,1.f),0.0f));

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

    // LFO1 Parameters

    layout.add(std::make_unique<juce::AudioParameterFloat>("LFO1 Freq","LFO1 Freq",juce::NormalisableRange<float>(0.01f,20.f,1e-2f,0.5f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("LFO1 Amp","LFO1 Amp",juce::NormalisableRange<float>(0.f,1.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 Level1","LFO1 Level1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 Pan1","LFO1 Pan1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 Fine1","LFO1 Fine1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 Coarse1","LFO1 Coarse1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 InPos1","LFO1 InPos1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 OutPos1","LFO1 OutPos1", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 Level2","LFO1 Level2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 Pan2","LFO1 Pan2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 Fine2","LFO1 Fine2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 Coarse2","LFO1 Coarse2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 InPos2","LFO1 InPos2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 OutPos2","LFO1 OutPos2", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 SamplerLevel","LFO1 SamplerLevel", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 SamplerLPFreq","LFO1 SamplerLPFreq", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 NoiseLevel","LFO1 NoiseLevel", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 NoiseLPFreq","LFO1 NoiseLPFreq", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 NoiseHPFreq","LFO1 NoiseHPFreq", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 CrackLevel","LFO1 CrackLevel", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 CrackLPFreq","LFO1 CrackLPFreq", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO1 CrackDensity","LFO1 CrackDensity", false));

    // LFO2 Parameters

    layout.add(std::make_unique<juce::AudioParameterFloat>("LFO2 Freq","LFO2 Freq",juce::NormalisableRange<float>(0.01f,20.f,1e-2f,0.5f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("LFO2 Amp","LFO2 Amp",juce::NormalisableRange<float>(0.f,1.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 Level1","LFO2 Level1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 Pan1","LFO2 Pan1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 Fine1","LFO2 Fine1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 Coarse1","LFO2 Coarse1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 InPos1","LFO2 InPos1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 OutPos1","LFO2 OutPos1", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 Level2","LFO2 Level2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 Pan2","LFO1 Pan2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 Fine2","LFO2 Fine2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 Coarse2","LFO2 Coarse2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 InPos2","LFO2 InPos2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 OutPos2","LFO2 OutPos2", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 SamplerLevel","LFO2 SamplerLevel", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 SamplerLPFreq","LFO2 SamplerLPFreq", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 NoiseLevel","LFO2 NoiseLevel", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 NoiseLPFreq","LFO2 NoiseLPFreq", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 NoiseHPFreq","LFO2 NoiseHPFreq", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 CrackLevel","LFO2 CrackLevel", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 CrackLPFreq","LFO2 CrackLPFreq", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO2 CrackDensity","LFO2 CrackDensity", false));

    // LFO3 Parameters

    layout.add(std::make_unique<juce::AudioParameterFloat>("LFO3 Freq","LFO3 Freq",juce::NormalisableRange<float>(0.01f,20.f,1e-2f,0.5f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("LFO3 Amp","LFO3 Amp",juce::NormalisableRange<float>(0.f,1.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 Level1","LFO3 Level1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 Pan1","LFO3 Pan1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 Fine1","LFO3 Fine1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 Coarse1","LFO3 Coarse1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 InPos1","LFO3 InPos1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 OutPos1","LFO3 OutPos1", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 Level2","LFO3 Level2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 Pan2","LFO3 Pan2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 Fine2","LFO3 Fine2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 Coarse2","LFO3 Coarse2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 InPos2","LFO3 InPos2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 OutPos2","LFO3 OutPos2", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 SamplerLevel","LFO3 SamplerLevel", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 SamplerLPFreq","LFO3 SamplerLPFreq", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 NoiseLevel","LFO3 NoiseLevel", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 NoiseLPFreq","LFO3 NoiseLPFreq", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 NoiseHPFreq","LFO3 NoiseHPFreq", false));

    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 CrackLevel","LFO3 CrackLevel", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 CrackLPFreq","LFO3 CrackLPFreq", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("LFO3 CrackDensity","LFO3 CrackDensity", false));

    return layout;
}

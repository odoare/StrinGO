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
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
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
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
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

    for (int i=0; i<synth.getNumVoices(); ++i)
    {
       if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
       {
        voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumInputChannels());
       }
    }
}

void MySynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MySynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MySynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // std::cout << "In Process block ... ";
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //     buffer.clear (i, 0, buffer.getNumSamples());

    for (int i=0; i<synth.getNumVoices(); ++i)
    {
       if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
       {
        for (int string=0; string<NUMSTRINGS; string++)
        {
            voice->stringReso.setFeedbackGainOn(string,apvts.getRawParameterValue("Fb Gain on 1")->load());
            voice->stringReso.setFeedbackFreqOn(string,apvts.getRawParameterValue("Fb Freq on 1")->load());
            voice->stringReso.setLevelOn(string,juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Level on 1")->load()));
            voice->stringReso.setFeedbackGainOff(string,apvts.getRawParameterValue("Fb Gain off 1")->load());
            voice->stringReso.setFeedbackFreqOff(string,apvts.getRawParameterValue("Fb Freq off 1")->load());
            voice->stringReso.setLevelOff(string,juce::Decibels::decibelsToGain(apvts.getRawParameterValue("Level off 1")->load()));
            voice->stringReso.setInPos(string,apvts.getRawParameterValue("In Pos 1")->load());
            voice->stringReso.setOutPos(string,apvts.getRawParameterValue("Out Pos 1")->load());   
            voice->stringReso.setCoupling(string,apvts.getRawParameterValue("Coupling")->load());         
        }
        voice->stringReso.setFreqCoarseFactor(0,apvts.getRawParameterValue("Freq Coarse 1")->load());
        voice->stringReso.setFreqFineFactor(0,apvts.getRawParameterValue("Freq Fine 1")->load());
        voice->stringReso.setFreqCoarseFactor(1,apvts.getRawParameterValue("Freq Coarse 2")->load());
        voice->stringReso.setFreqFineFactor(1,apvts.getRawParameterValue("Freq Fine 2")->load());

        voice->stringReso.setSmoothTime(apvts.getRawParameterValue("Smooth time")->load());
        voice->stringReso.setPortamentoTime(apvts.getRawParameterValue("Porta time")->load());

        voice->adsr2Params.attack = apvts.getRawParameterValue("AN")->load();
        voice->adsr2Params.decay = apvts.getRawParameterValue("DN")->load();
        voice->adsr2Params.sustain = apvts.getRawParameterValue("SN")->load();
        voice->adsr2Params.release = apvts.getRawParameterValue("RN")->load();
        voice->adsr2.setParameters(voice->adsr2Params);

        voice->stringReso.adsr1Params.attack = apvts.getRawParameterValue("A1")->load();
        voice->stringReso.adsr1Params.decay = apvts.getRawParameterValue("D1")->load();
        voice->stringReso.adsr1Params.sustain = apvts.getRawParameterValue("S1")->load();
        voice->stringReso.adsr1Params.release = apvts.getRawParameterValue("R1")->load();
        voice->stringReso.adsr1.setParameters(voice->stringReso.adsr1Params);

        voice->setNoiseFilterFreq(apvts.getRawParameterValue("Noise Freq")->load());

       } 
    }

    //buffer.clear();

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool MySynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MySynthAudioProcessor::createEditor()
{
    // return new MySynthAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
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
    layout.add(std::make_unique<juce::AudioParameterChoice>("OSC","OSC",juce::StringArray{"Sine","Saw","Square"}, 0));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("AN","AN",juce::NormalisableRange<float>(0.001f,2.f,1e-3f,1.f),0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DN","DN",juce::NormalisableRange<float>(0.001f,2.f,1e-3f,1.f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("SN","SN",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,1.f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("RN","RN",juce::NormalisableRange<float>(0.001f,10.f,1e-3f,1.f),0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("A1","A1",juce::NormalisableRange<float>(0.001f,2.f,1e-3f,1.f),0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("D1","D1",juce::NormalisableRange<float>(0.001f,2.f,1e-3f,1.f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("S1","S1",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,1.f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("R1","R1",juce::NormalisableRange<float>(0.001f,10.f,1e-3f,1.f),0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Porta time","Porta time",juce::NormalisableRange<float>(0.001f,1.f,1e-3f,1.f),0.1f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Smooth time","Smooth time",juce::NormalisableRange<float>(0.001f,1.f,1e-3f,1.f),1.f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Coupling","Coupling",juce::NormalisableRange<float>(0.001f,1.f,1e-3f,1.f),1.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Freq Coarse 1","Freq Coarse 1",juce::NormalisableRange<float>(-12.f,12.f,1.f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Freq Fine 1","Freq Fine 1",juce::NormalisableRange<float>(-1.f,1.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Freq Coarse 2","Freq Coarse 2",juce::NormalisableRange<float>(-12.f,12.f,1.f,1.f),0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Freq Fine 2","Freq Fine 2",juce::NormalisableRange<float>(-1.f,1.f,1e-2f,1.f),0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("In Pos 1","In Pos 1",juce::NormalisableRange<float>(0.05f,0.45f,1e-3f,1.f),0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Out Pos 1","Out Pos 1",juce::NormalisableRange<float>(0.55f,0.95f,1e-3f,1.f),0.9f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("In Pos 2","In Pos 2",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,1.f),1.f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Out Pos 2","Out Pos 2",juce::NormalisableRange<float>(0.001f,3.f,1e-3f,1.f),0.5f));


    layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Freq on 1","Fb Freq on 1",juce::NormalisableRange<float>(0.1f,100.f,1e-1f,1.f),40.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Gain on 1","Fb Gain on 1",juce::NormalisableRange<float>(0.f,1.f,1e-3f,1.f),0.95f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Level on 1","Level on 1",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),-3.f));

    // layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Freq on 2","Fb Freq on 2",juce::NormalisableRange<float>(0.1f,100.f,1e-1f,1.f),40.f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Gain on 2","Fb Gain on 2",juce::NormalisableRange<float>(0.f,1.f,1e-3f,1.f),0.95f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Level on 2","Level on 2",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),-3.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Freq off 1","Fb Freq off 1",juce::NormalisableRange<float>(0.1f,100.f,1e-1f,1.f),40.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Gain off 1","Fb Gain off 1",juce::NormalisableRange<float>(0.f,1.f,1e-3f,1.f),0.95f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Level off 1","Level off 1",juce::NormalisableRange<float>(-90.f,0.f,1e-2f,1.f),-3.f));

    // layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Freq off 2","Fb Freq off 2",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,1.f),1.f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Fb Gain off 2","Fb Gain off 2",juce::NormalisableRange<float>(0.001f,3.f,1e-3f,1.f),0.5f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("Level off 2","Level off 2",juce::NormalisableRange<float>(0.001f,3.f,1e-3f,1.f),0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Noise Freq","Noise Freq",juce::NormalisableRange<float>(20.f,20000.f,1.f,1.f),2000.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Coupling","Coupling",juce::NormalisableRange<float>(0.f,1.f,1e-3f,1.f),0.05f));


    return layout;

}

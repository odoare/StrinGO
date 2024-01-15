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
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //     buffer.clear (i, 0, buffer.getNumSamples());

    for (int i=0; i<synth.getNumVoices(); ++i)
    {
       if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(synth.getVoice(i)))
       {
        // Stuff to do...
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MySynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
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
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("A1","A1",juce::NormalisableRange<float>(0.001f,1.f,1e-3f,1.f),0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("D1","D1",juce::NormalisableRange<float>(0.001f,1.f,1e-3f,1.f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("S1","S1",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,1.f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("R1","R1",juce::NormalisableRange<float>(0.001f,3.f,1e-3f,1.f),0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("A2","A2",juce::NormalisableRange<float>(0.001f,1.f,1e-3f,1.f),0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("D2","D2",juce::NormalisableRange<float>(0.001f,1.f,1e-3f,1.f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("S2","S2",juce::NormalisableRange<float>(0.0f,1.f,1e-3f,1.f),1.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("R2","R2",juce::NormalisableRange<float>(0.001f,3.f,1e-3f,1.f),0.5f));



    return layout;

}

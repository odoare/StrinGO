/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MySynthAudioProcessorEditor::MySynthAudioProcessorEditor (MySynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    logo = juce::ImageCache::getFromMemory(BinaryData::logo686_png, BinaryData::logo686_pngSize);

    addKnob(inGain, inGainLabel, MASTERCOLOR,juce::Colours::black);
    inGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Input Gain",inGain);

    addKnob(directOut, directOutLabel, MASTERCOLOR,juce::Colours::black);
    directOutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Direct Out",directOut);

    addVSlider(gain, gainLabel, MASTERCOLOR,juce::Colours::black);
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Output Gain",gain);

    addVSlider(level, levelLabel, MASTERCOLOR,juce::Colours::black);
    levelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Output Level",level);

    addKnob(voices, voicesLabel, MASTERCOLOR,juce::Colours::black);
    voicesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Voices",voices);

    addKnob(porta, portaLabel, MASTERCOLOR,juce::Colours::black);
    portaAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Porta Time",porta);

    // Waveguides
    addKnob(stringsFBGainOn, stringsFBGainOnLabel, STRINGCOLOR, juce::Colours::black);
    stringsFBGainOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Gain On",stringsFBGainOn);
    addKnob(stringsFBFreqOn, stringsFBFreqOnLabel, STRINGCOLOR,juce::Colours::black);
    stringsFBFreqOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Freq On",stringsFBFreqOn);
    addKnob(stringsLevelOn, stringsLevelOnLabel, STRINGCOLOR,juce::Colours::black);
    stringsLevelOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level On",stringsLevelOn);

    addKnob(stringsFBGainOff, stringsFBGainOffLabel, STRINGCOLOR,juce::Colours::black);
    stringsFBGainOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Gain Off",stringsFBGainOff);
    addKnob(stringsFBFreqOff, stringsFBFreqOffLabel, STRINGCOLOR,juce::Colours::black);
    stringsFBFreqOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Freq Off",stringsFBFreqOff);
    addKnob(stringsLevelOff, stringsLevelOffLabel, STRINGCOLOR,juce::Colours::black);
    stringsLevelOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level Off",stringsLevelOff);

    addHSlider(stringsInPos, stringsInPosLabel, STRINGCOLOR,juce::Colours::black);
    stringsInPosAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"In Pos",stringsInPos);

    addHSlider(stringsOutPos, stringsOutPosLabel, STRINGCOLOR,juce::Colours::black);
    stringsOutPosAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Out Pos",stringsOutPos);

    addHSlider(stringsCoupling, stringsCouplingLabel, STRINGCOLOR,juce::Colours::black);
    stringsCouplingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Coupling",stringsCoupling);

    addKnob(string1Level, string1LevelLabel, STRINGCOLOR,juce::Colours::black);
    string1LevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level1",string1Level);
    addKnob(string1FreqCoarse, string1FreqCoarseLabel, STRINGCOLOR,juce::Colours::black);
    string1FreqCoarseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Coarse1",string1FreqCoarse);
    addKnob(string1FreqFine, string1FreqFineLabel, STRINGCOLOR,juce::Colours::black);
    string1FreqFineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fine1",string1FreqFine);
       
    addKnob(string2Level, string2LevelLabel, STRINGCOLOR,juce::Colours::black);
    string2LevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level2",string2Level);
    addKnob(string2FreqCoarse, string2FreqCoarseLabel, STRINGCOLOR,juce::Colours::black);
    string2FreqCoarseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Coarse2",string2FreqCoarse);
    addKnob(string2FreqFine, string2FreqFineLabel, STRINGCOLOR,juce::Colours::black);
    string2FreqFineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fine2",string2FreqFine);

    // String waveguide enveloppe
    addKnob(stringsAttack, stringsAttackLabel, STRINGCOLOR,juce::Colours::black);
    stringsAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"A",stringsAttack);
    addKnob(stringsDecay, stringsDecayLabel, STRINGCOLOR,juce::Colours::black);
    stringsDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"D",stringsDecay);
    addKnob(stringsSustain, stringsSustainLabel, STRINGCOLOR,juce::Colours::black);
    stringsSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"S",stringsSustain);
    addKnob(stringsRelease, stringsReleaseLabel, STRINGCOLOR,juce::Colours::black);
    stringsReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"R",stringsRelease);
    
    // Sample
    addKnob(samplerAttack, samplerAttackLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AS",samplerAttack);
    addKnob(samplerDecay, samplerDecayLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DS",samplerDecay);
    addKnob(samplerSustain, samplerSustainLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SS",samplerSustain);
    addKnob(samplerRelease, samplerReleaseLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RS",samplerRelease);
    addKnob(samplerLPFreq, samplerLPFreqLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerLPFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Sample LP Freq",samplerLPFreq);
    addKnob(samplerLevel, samplerLevelLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Sample Level",samplerLevel);

    // // Sample wave combo box
    juce::StringArray choices;
    choices.addArray(WAVECHOICES);
    waveComboBox.addItemList(choices,1);
    waveComboBoxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts,"Attack Sample",waveComboBox);
    waveLabel.attachToComponent(&waveComboBox,false);
    addAndMakeVisible(waveComboBox);
    addAndMakeVisible(waveLabel);

    addAndMakeVisible(loopButton);
    loopButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"Loop Sample",loopButton);
    addAndMakeVisible(loopLabel);
    loopLabel.attachToComponent(&loopButton,true);

    // Noise enveloppe
    addKnob(noiseAttack, noiseAttackLabel, NOISECOLOR,juce::Colours::black);
    noiseAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AN",noiseAttack);
    addKnob(noiseDecay, noiseDecayLabel, NOISECOLOR,juce::Colours::black);
    noiseDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DN",noiseDecay);
    addKnob(noiseSustain, noiseSustainLabel, NOISECOLOR,juce::Colours::black);
    noiseSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SN",noiseSustain);
    addKnob(noiseRelease, noiseReleaseLabel, NOISECOLOR,juce::Colours::black);
    noiseReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RN",noiseRelease);
    addKnob(noiseLPFreq, noiseLPFreqLabel, NOISECOLOR,juce::Colours::black);
    noiseLPFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Noise LP Freq",noiseLPFreq);
    addKnob(noiseHPFreq, noiseHPFreqLabel, NOISECOLOR,juce::Colours::black);
    noiseHPFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Noise HP Freq",noiseHPFreq);
    addKnob(noiseLevel, noiseLevelLabel, NOISECOLOR,juce::Colours::black);
    noiseLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Noise Level",noiseLevel);

    // crack enveloppe
    addKnob(crackAttack, crackAttackLabel, CRACKSCOLOR,juce::Colours::black);
    crackAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AC",crackAttack);
    addKnob(crackDecay, crackDecayLabel, CRACKSCOLOR,juce::Colours::black);
    crackDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DC",crackDecay);
    addKnob(crackSustain, crackSustainLabel, CRACKSCOLOR,juce::Colours::black);
    crackSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SC",crackSustain);
    addKnob(crackRelease, crackReleaseLabel, CRACKSCOLOR,juce::Colours::black);
    crackReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RC",crackRelease);
    addKnob(crackDensity, crackDensityLabel, CRACKSCOLOR,juce::Colours::black);
    crackDensityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Density",crackDensity);
    addKnob(crackLPFreq, crackLPFreqLabel, CRACKSCOLOR,juce::Colours::black);
    crackLPFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack LP Freq",crackLPFreq);
    addKnob(crackLevel, crackLevelLabel, CRACKSCOLOR,juce::Colours::black);
    crackLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Level",crackLevel);

    addKnob(velocitySampleLevel, velocitySampleLevelLabel, SAMPLERCOLOR,juce::Colours::black);
    velocitySampleLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Sample Level",velocitySampleLevel);
    addKnob(velocitySampleFreq, velocitySampleFreqLabel, SAMPLERCOLOR,juce::Colours::black);
    velocitySampleFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Sample Freq",velocitySampleFreq);

    addKnob(velocityNoiseLevel, velocityNoiseLevelLabel, NOISECOLOR,juce::Colours::black);
    velocityNoiseLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Noise Level",velocityNoiseLevel);
    addKnob(velocityNoiseFreq, velocityNoiseFreqLabel, NOISECOLOR,juce::Colours::black);
    velocityNoiseFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Noise Freq",velocityNoiseFreq);

    addKnob(velocityCrackLevel, velocityCrackLevelLabel, CRACKSCOLOR,juce::Colours::black);
    velocityCrackLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Crack Level",velocityCrackLevel);
    addKnob(velocityCrackFreq, velocityCrackFreqLabel, CRACKSCOLOR,juce::Colours::black);
    velocityCrackFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Crack Freq",velocityCrackFreq);

    // LFO 1
    addKnob(lfo1Amp, lfo1AmpLabel, LFO1COLOR,juce::Colours::black);
    lfo1AmpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"LFO1 Amp",lfo1Amp);
    addKnob(lfo1Freq, lfo1FreqLabel, LFO1COLOR,juce::Colours::black);
    lfo1FreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"LFO1 Freq",lfo1Freq);

    addAndMakeVisible(lfo1SamplerLevelButton);
    lfo1SamplerLevelButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 SamplerLevel",lfo1SamplerLevelButton);
    addAndMakeVisible(lfo1SamplerLevelLabel);
    lfo1SamplerLevelLabel.attachToComponent(&lfo1SamplerLevelButton,false);
    lfo1SamplerLevelButton.setLookAndFeel(&buttonLookAndFeel);
    lfo1SamplerLevelButton.setClickingTogglesState(true);
    lfo1SamplerLevelButton.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1SamplerLPFreqButton);
    lfo1SamplerLPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 SamplerLPFreq",lfo1SamplerLPFreqButton);
    addAndMakeVisible(lfo1SamplerLPFreqLabel);
    lfo1SamplerLPFreqLabel.attachToComponent(&lfo1SamplerLPFreqButton,false);
    lfo1SamplerLPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo1SamplerLPFreqButton.setClickingTogglesState(true);
    lfo1SamplerLPFreqButton.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1Level1Button);
    lfo1Level1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 Level1",lfo1Level1Button);
    addAndMakeVisible(lfo1Level1Label);
    lfo1Level1Label.attachToComponent(&lfo1Level1Button,false);
    lfo1Level1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo1Level1Button.setClickingTogglesState(true);
    lfo1Level1Button.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1Coarse1Button);
    lfo1Coarse1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 Coarse1",lfo1Coarse1Button);
    addAndMakeVisible(lfo1Coarse1Label);
    lfo1Coarse1Label.attachToComponent(&lfo1Coarse1Button,false);
    lfo1Coarse1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo1Coarse1Button.setClickingTogglesState(true);
    lfo1Coarse1Button.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1Fine1Button);
    lfo1Fine1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 Fine1",lfo1Fine1Button);
    addAndMakeVisible(lfo1Fine1Label);
    lfo1Fine1Label.attachToComponent(&lfo1Fine1Button,false);
    lfo1Fine1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo1Fine1Button.setClickingTogglesState(true);
    lfo1Fine1Button.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1Level2Button);
    lfo1Level2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 Level2",lfo1Level2Button);
    addAndMakeVisible(lfo1Level2Label);
    lfo1Level2Label.attachToComponent(&lfo1Level2Button,false);
    lfo1Level2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo1Level2Button.setClickingTogglesState(true);
    lfo1Level2Button.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1Coarse2Button);
    lfo1Coarse2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 Coarse2",lfo1Coarse2Button);
    addAndMakeVisible(lfo1Coarse2Label);
    lfo1Coarse2Label.attachToComponent(&lfo1Coarse2Button,false);
    lfo1Coarse2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo1Coarse2Button.setClickingTogglesState(true);
    lfo1Coarse2Button.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1Fine2Button);
    lfo1Fine2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 Fine2",lfo1Fine2Button);
    addAndMakeVisible(lfo1Fine2Label);
    lfo1Fine2Label.attachToComponent(&lfo1Fine2Button,false);
    lfo1Fine2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo1Fine2Button.setClickingTogglesState(true);
    lfo1Fine2Button.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1InPos1Button);
    lfo1InPos1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 InPos1",lfo1InPos1Button);
    addAndMakeVisible(lfo1InPos1Label);
    lfo1InPos1Label.attachToComponent(&lfo1InPos1Button,false);
    lfo1InPos1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo1InPos1Button.setClickingTogglesState(true);
    lfo1InPos1Button.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1OutPos1Button);
    lfo1OutPos1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 OutPos1",lfo1OutPos1Button);
    addAndMakeVisible(lfo1OutPos1Label);
    lfo1OutPos1Label.attachToComponent(&lfo1OutPos1Button,false);
    lfo1OutPos1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo1OutPos1Button.setClickingTogglesState(true);
    lfo1OutPos1Button.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1InPos2Button);
    lfo1InPos2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 InPos2",lfo1InPos2Button);
    addAndMakeVisible(lfo1InPos2Label);
    lfo1InPos2Label.attachToComponent(&lfo1InPos2Button,false);
    lfo1InPos2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo1InPos2Button.setClickingTogglesState(true);
    lfo1InPos2Button.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1OutPos2Button);
    lfo1OutPos2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 OutPos2",lfo1OutPos2Button);
    addAndMakeVisible(lfo1OutPos2Label);
    lfo1OutPos2Label.attachToComponent(&lfo1OutPos2Button,false);
    lfo1OutPos2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo1OutPos2Button.setClickingTogglesState(true);
    lfo1OutPos2Button.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1NoiseLevelButton);
    lfo1NoiseLevelButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 NoiseLevel",lfo1NoiseLevelButton);
    addAndMakeVisible(lfo1NoiseLevelLabel);
    lfo1NoiseLevelLabel.attachToComponent(&lfo1NoiseLevelButton,false);
    lfo1NoiseLevelButton.setLookAndFeel(&buttonLookAndFeel);
    lfo1NoiseLevelButton.setClickingTogglesState(true);
    lfo1NoiseLevelButton.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1NoiseLPFreqButton);
    lfo1NoiseLPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 NoiseLPFreq",lfo1NoiseLPFreqButton);
    addAndMakeVisible(lfo1NoiseLPFreqLabel);
    lfo1NoiseLPFreqLabel.attachToComponent(&lfo1NoiseLPFreqButton,false);
    lfo1NoiseLPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo1NoiseLPFreqButton.setClickingTogglesState(true);
    lfo1NoiseLPFreqButton.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1NoiseHPFreqButton);
    lfo1NoiseHPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 NoiseHPFreq",lfo1NoiseHPFreqButton);
    addAndMakeVisible(lfo1NoiseHPFreqLabel);
    lfo1NoiseHPFreqLabel.attachToComponent(&lfo1NoiseHPFreqButton,false);
    lfo1NoiseHPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo1NoiseHPFreqButton.setClickingTogglesState(true);
    lfo1NoiseHPFreqButton.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1CrackLevelButton);
    lfo1CrackLevelButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 CrackLevel",lfo1CrackLevelButton);
    addAndMakeVisible(lfo1CrackLevelLabel);
    lfo1CrackLevelLabel.attachToComponent(&lfo1CrackLevelButton,false);
    lfo1CrackLevelButton.setLookAndFeel(&buttonLookAndFeel);
    lfo1CrackLevelButton.setClickingTogglesState(true);
    lfo1CrackLevelButton.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1CrackLPFreqButton);
    lfo1CrackLPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 CrackLPFreq",lfo1CrackLPFreqButton);
    addAndMakeVisible(lfo1CrackLPFreqLabel);
    lfo1CrackLPFreqLabel.attachToComponent(&lfo1CrackLPFreqButton,false);
    lfo1CrackLPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo1CrackLPFreqButton.setClickingTogglesState(true);
    lfo1CrackLPFreqButton.setColour(0,LFO1COLOR);

    addAndMakeVisible(lfo1CrackDensityButton);
    lfo1CrackDensityButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO1 CrackDensity",lfo1CrackDensityButton);
    addAndMakeVisible(lfo1CrackDensityLabel);
    lfo1CrackDensityLabel.attachToComponent(&lfo1CrackDensityButton,false);
    lfo1CrackDensityButton.setLookAndFeel(&buttonLookAndFeel);
    lfo1CrackDensityButton.setClickingTogglesState(true);
    lfo1CrackDensityButton.setColour(0,LFO1COLOR);


    // --------------------------------------------------------------------
    // LFO 2
    // --------------------------------------------------------------------

    addKnob(lfo2Amp, lfo2AmpLabel, LFO2COLOR,juce::Colours::black);
    lfo2AmpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"LFO2 Amp",lfo2Amp);
    addKnob(lfo2Freq, lfo2FreqLabel, LFO2COLOR,juce::Colours::black);
    lfo2FreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"LFO2 Freq",lfo2Freq);

    addAndMakeVisible(lfo2SamplerLevelButton);
    lfo2SamplerLevelButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 SamplerLevel",lfo2SamplerLevelButton);
    addAndMakeVisible(lfo2SamplerLevelLabel);
    lfo2SamplerLevelLabel.attachToComponent(&lfo2SamplerLevelButton,false);
    lfo2SamplerLevelButton.setLookAndFeel(&buttonLookAndFeel);
    lfo2SamplerLevelButton.setClickingTogglesState(true);
    lfo2SamplerLevelButton.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2SamplerLPFreqButton);
    lfo2SamplerLPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 SamplerLPFreq",lfo2SamplerLPFreqButton);
    addAndMakeVisible(lfo2SamplerLPFreqLabel);
    lfo2SamplerLPFreqLabel.attachToComponent(&lfo2SamplerLPFreqButton,false);
    lfo2SamplerLPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo2SamplerLPFreqButton.setClickingTogglesState(true);
    lfo2SamplerLPFreqButton.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2Level1Button);
    lfo2Level1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 Level1",lfo2Level1Button);
    addAndMakeVisible(lfo2Level1Label);
    lfo2Level1Label.attachToComponent(&lfo2Level1Button,false);
    lfo2Level1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo2Level1Button.setClickingTogglesState(true);
    lfo2Level1Button.setColour(0,LFO2COLOR);
    
    addAndMakeVisible(lfo2Coarse1Button);
    lfo2Coarse1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 Coarse1",lfo2Coarse1Button);
    addAndMakeVisible(lfo2Coarse1Label);
    lfo2Coarse1Label.attachToComponent(&lfo2Coarse1Button,false);
    lfo2Coarse1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo2Coarse1Button.setClickingTogglesState(true);
    lfo2Coarse1Button.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2Fine1Button);
    lfo2Fine1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 Fine1",lfo2Fine1Button);
    addAndMakeVisible(lfo2Fine1Label);
    lfo2Fine1Label.attachToComponent(&lfo2Fine1Button,false);
    lfo2Fine1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo2Fine1Button.setClickingTogglesState(true);
    lfo2Fine1Button.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2Level2Button);
    lfo2Level2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 Level2",lfo2Level2Button);
    addAndMakeVisible(lfo2Level2Label);
    lfo2Level2Label.attachToComponent(&lfo2Level2Button,false);
    lfo2Level2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo2Level2Button.setClickingTogglesState(true);
    lfo2Level2Button.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2Coarse2Button);
    lfo2Coarse2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 Coarse2",lfo2Coarse2Button);
    addAndMakeVisible(lfo2Coarse2Label);
    lfo2Coarse2Label.attachToComponent(&lfo2Coarse2Button,false);
    lfo2Coarse2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo2Coarse2Button.setClickingTogglesState(true);
    lfo2Coarse2Button.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2Fine2Button);
    lfo2Fine2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 Fine2",lfo2Fine2Button);
    addAndMakeVisible(lfo2Fine2Label);
    lfo2Fine2Label.attachToComponent(&lfo2Fine2Button,false);
    lfo2Fine2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo2Fine2Button.setClickingTogglesState(true);
    lfo2Fine2Button.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2InPos1Button);
    lfo2InPos1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 InPos1",lfo2InPos1Button);
    addAndMakeVisible(lfo2InPos1Label);
    lfo2InPos1Label.attachToComponent(&lfo2InPos1Button,false);
    lfo2InPos1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo2InPos1Button.setClickingTogglesState(true);
    lfo2InPos1Button.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2OutPos1Button);
    lfo2OutPos1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 OutPos1",lfo2OutPos1Button);
    addAndMakeVisible(lfo2OutPos1Label);
    lfo2OutPos1Label.attachToComponent(&lfo2OutPos1Button,false);
    lfo2OutPos1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo2OutPos1Button.setClickingTogglesState(true);
    lfo2OutPos1Button.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2InPos2Button);
    lfo2InPos2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 InPos2",lfo2InPos2Button);
    addAndMakeVisible(lfo2InPos2Label);
    lfo2InPos2Label.attachToComponent(&lfo2InPos2Button,false);
    lfo2InPos2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo2InPos2Button.setClickingTogglesState(true);
    lfo2InPos2Button.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2OutPos2Button);
    lfo2OutPos2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 OutPos2",lfo2OutPos2Button);
    addAndMakeVisible(lfo2OutPos2Label);
    lfo2OutPos2Label.attachToComponent(&lfo2OutPos2Button,false);
    lfo2OutPos2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo2OutPos2Button.setClickingTogglesState(true);
    lfo2OutPos2Button.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2NoiseLevelButton);
    lfo2NoiseLevelButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 NoiseLevel",lfo2NoiseLevelButton);
    addAndMakeVisible(lfo2NoiseLevelLabel);
    lfo2NoiseLevelLabel.attachToComponent(&lfo2NoiseLevelButton,false);
    lfo2NoiseLevelButton.setLookAndFeel(&buttonLookAndFeel);
    lfo2NoiseLevelButton.setClickingTogglesState(true);
    lfo2NoiseLevelButton.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2NoiseLPFreqButton);
    lfo2NoiseLPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 NoiseLPFreq",lfo2NoiseLPFreqButton);
    addAndMakeVisible(lfo2NoiseLPFreqLabel);
    lfo2NoiseLPFreqLabel.attachToComponent(&lfo2NoiseLPFreqButton,false);
    lfo2NoiseLPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo2NoiseLPFreqButton.setClickingTogglesState(true);
    lfo2NoiseLPFreqButton.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2NoiseHPFreqButton);
    lfo2NoiseHPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 NoiseHPFreq",lfo2NoiseHPFreqButton);
    addAndMakeVisible(lfo2NoiseHPFreqLabel);
    lfo2NoiseHPFreqLabel.attachToComponent(&lfo2NoiseHPFreqButton,false);
    lfo2NoiseHPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo2NoiseHPFreqButton.setClickingTogglesState(true);
    lfo2NoiseHPFreqButton.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2CrackLevelButton);
    lfo2CrackLevelButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 CrackLevel",lfo2CrackLevelButton);
    addAndMakeVisible(lfo2CrackLevelLabel);
    lfo2CrackLevelLabel.attachToComponent(&lfo2CrackLevelButton,false);
    lfo2CrackLevelButton.setLookAndFeel(&buttonLookAndFeel);
    lfo2CrackLevelButton.setClickingTogglesState(true);
    lfo2CrackLevelButton.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2CrackLPFreqButton);
    lfo2CrackLPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 CrackLPFreq",lfo2CrackLPFreqButton);
    addAndMakeVisible(lfo2CrackLPFreqLabel);
    lfo2CrackLPFreqLabel.attachToComponent(&lfo2CrackLPFreqButton,false);
    lfo2CrackLPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo2CrackLPFreqButton.setClickingTogglesState(true);
    lfo2CrackLPFreqButton.setColour(0,LFO2COLOR);

    addAndMakeVisible(lfo2CrackDensityButton);
    lfo2CrackDensityButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO2 CrackDensity",lfo2CrackDensityButton);
    addAndMakeVisible(lfo2CrackDensityLabel);
    lfo2CrackDensityLabel.attachToComponent(&lfo2CrackDensityButton,false);
    lfo2CrackDensityButton.setLookAndFeel(&buttonLookAndFeel);
    lfo2CrackDensityButton.setClickingTogglesState(true);
    lfo2CrackDensityButton.setColour(0,LFO2COLOR);

    // --------------------------------------------------------------------
    // LFO 3
    // --------------------------------------------------------------------
    
    addKnob(lfo3Amp, lfo3AmpLabel, LFO3COLOR,juce::Colours::black);
    lfo3AmpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"LFO3 Amp",lfo3Amp);
    addKnob(lfo3Freq, lfo3FreqLabel, LFO3COLOR,juce::Colours::black);
    lfo3FreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"LFO3 Freq",lfo3Freq);

    addAndMakeVisible(lfo3SamplerLevelButton);
    lfo3SamplerLevelButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 SamplerLevel",lfo3SamplerLevelButton);
    addAndMakeVisible(lfo3SamplerLevelLabel);
    lfo3SamplerLevelLabel.attachToComponent(&lfo3SamplerLevelButton,false);
    lfo3SamplerLevelButton.setLookAndFeel(&buttonLookAndFeel);
    lfo3SamplerLevelButton.setClickingTogglesState(true);
    lfo3SamplerLevelButton.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3SamplerLPFreqButton);
    lfo3SamplerLPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 SamplerLPFreq",lfo3SamplerLPFreqButton);
    addAndMakeVisible(lfo3SamplerLPFreqLabel);
    lfo3SamplerLPFreqLabel.attachToComponent(&lfo3SamplerLPFreqButton,false);
    lfo3SamplerLPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo3SamplerLPFreqButton.setClickingTogglesState(true);
    lfo3SamplerLPFreqButton.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3Level1Button);
    lfo3Level1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 Level1",lfo3Level1Button);
    addAndMakeVisible(lfo3Level1Label);
    lfo3Level1Label.attachToComponent(&lfo3Level1Button,false);
    lfo3Level1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo3Level1Button.setClickingTogglesState(true);
    lfo3Level1Button.setColour(0,LFO3COLOR);
    
    addAndMakeVisible(lfo3Coarse1Button);
    lfo3Coarse1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 Coarse1",lfo3Coarse1Button);
    addAndMakeVisible(lfo3Coarse1Label);
    lfo3Coarse1Label.attachToComponent(&lfo3Coarse1Button,false);
    lfo3Coarse1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo3Coarse1Button.setClickingTogglesState(true);
    lfo3Coarse1Button.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3Fine1Button);
    lfo3Fine1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 Fine1",lfo3Fine1Button);
    addAndMakeVisible(lfo3Fine1Label);
    lfo3Fine1Label.attachToComponent(&lfo3Fine1Button,false);
    lfo3Fine1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo3Fine1Button.setClickingTogglesState(true);
    lfo3Fine1Button.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3Level2Button);
    lfo3Level2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 Level2",lfo3Level2Button);
    addAndMakeVisible(lfo3Level2Label);
    lfo3Level2Label.attachToComponent(&lfo3Level2Button,false);
    lfo3Level2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo3Level2Button.setClickingTogglesState(true);
    lfo3Level2Button.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3Coarse2Button);
    lfo3Coarse2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 Coarse2",lfo3Coarse2Button);
    addAndMakeVisible(lfo3Coarse2Label);
    lfo3Coarse2Label.attachToComponent(&lfo3Coarse2Button,false);
    lfo3Coarse2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo3Coarse2Button.setClickingTogglesState(true);
    lfo3Coarse2Button.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3Fine2Button);
    lfo3Fine2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 Fine2",lfo3Fine2Button);
    addAndMakeVisible(lfo3Fine2Label);
    lfo3Fine2Label.attachToComponent(&lfo3Fine2Button,false);
    lfo3Fine2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo3Fine2Button.setClickingTogglesState(true);
    lfo3Fine2Button.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3InPos1Button);
    lfo3InPos1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 InPos1",lfo3InPos1Button);
    addAndMakeVisible(lfo3InPos1Label);
    lfo3InPos1Label.attachToComponent(&lfo3InPos1Button,false);
    lfo3InPos1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo3InPos1Button.setClickingTogglesState(true);
    lfo3InPos1Button.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3OutPos1Button);
    lfo3OutPos1ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 OutPos1",lfo3OutPos1Button);
    addAndMakeVisible(lfo3OutPos1Label);
    lfo3OutPos1Label.attachToComponent(&lfo3OutPos1Button,false);
    lfo3OutPos1Button.setLookAndFeel(&buttonLookAndFeel);
    lfo3OutPos1Button.setClickingTogglesState(true);
    lfo3OutPos1Button.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3InPos2Button);
    lfo3InPos2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 InPos2",lfo3InPos2Button);
    addAndMakeVisible(lfo3InPos2Label);
    lfo3InPos2Label.attachToComponent(&lfo3InPos2Button,false);
    lfo3InPos2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo3InPos2Button.setClickingTogglesState(true);
    lfo3InPos2Button.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3OutPos2Button);
    lfo3OutPos2ButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 OutPos2",lfo3OutPos2Button);
    addAndMakeVisible(lfo3OutPos2Label);
    lfo3OutPos2Label.attachToComponent(&lfo3OutPos2Button,false);
    lfo3OutPos2Button.setLookAndFeel(&buttonLookAndFeel);
    lfo3OutPos2Button.setClickingTogglesState(true);
    lfo3OutPos2Button.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3NoiseLevelButton);
    lfo3NoiseLevelButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 NoiseLevel",lfo3NoiseLevelButton);
    addAndMakeVisible(lfo3NoiseLevelLabel);
    lfo3NoiseLevelLabel.attachToComponent(&lfo3NoiseLevelButton,false);
    lfo3NoiseLevelButton.setLookAndFeel(&buttonLookAndFeel);
    lfo3NoiseLevelButton.setClickingTogglesState(true);
    lfo3NoiseLevelButton.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3NoiseLPFreqButton);
    lfo3NoiseLPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 NoiseLPFreq",lfo3NoiseLPFreqButton);
    addAndMakeVisible(lfo3NoiseLPFreqLabel);
    lfo3NoiseLPFreqLabel.attachToComponent(&lfo3NoiseLPFreqButton,false);
    lfo3NoiseLPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo3NoiseLPFreqButton.setClickingTogglesState(true);
    lfo3NoiseLPFreqButton.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3NoiseHPFreqButton);
    lfo3NoiseHPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 NoiseHPFreq",lfo3NoiseHPFreqButton);
    addAndMakeVisible(lfo3NoiseHPFreqLabel);
    lfo3NoiseHPFreqLabel.attachToComponent(&lfo3NoiseHPFreqButton,false);
    lfo3NoiseHPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo3NoiseHPFreqButton.setClickingTogglesState(true);
    lfo3NoiseHPFreqButton.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3CrackLevelButton);
    lfo3CrackLevelButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 CrackLevel",lfo3CrackLevelButton);
    addAndMakeVisible(lfo3CrackLevelLabel);
    lfo3CrackLevelLabel.attachToComponent(&lfo3CrackLevelButton,false);
    lfo3CrackLevelButton.setLookAndFeel(&buttonLookAndFeel);
    lfo3CrackLevelButton.setClickingTogglesState(true);
    lfo3CrackLevelButton.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3CrackLPFreqButton);
    lfo3CrackLPFreqButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 CrackLPFreq",lfo3CrackLPFreqButton);
    addAndMakeVisible(lfo3CrackLPFreqLabel);
    lfo3CrackLPFreqLabel.attachToComponent(&lfo3CrackLPFreqButton,false);
    lfo3CrackLPFreqButton.setLookAndFeel(&buttonLookAndFeel);
    lfo3CrackLPFreqButton.setClickingTogglesState(true);
    lfo3CrackLPFreqButton.setColour(0,LFO3COLOR);

    addAndMakeVisible(lfo3CrackDensityButton);
    lfo3CrackDensityButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"LFO3 CrackDensity",lfo3CrackDensityButton);
    addAndMakeVisible(lfo3CrackDensityLabel);
    lfo3CrackDensityLabel.attachToComponent(&lfo3CrackDensityButton,false);
    lfo3CrackDensityButton.setLookAndFeel(&buttonLookAndFeel);
    lfo3CrackDensityButton.setClickingTogglesState(true);
    lfo3CrackDensityButton.setColour(0,LFO3COLOR);



    setSize (1000, 800);
    setResizable(true,true);
}

MySynthAudioProcessorEditor::~MySynthAudioProcessorEditor()
{
}

#define NX 12.5
#define NY 8
#define SX 0.8
#define SY 0.8
#define DELTAX 0.1f
#define DELTAY 0.1f

//========================================  ======================================
void MySynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    static const float border = 0.01;
    float uxb = border*getWidth();
    float uyb = border*getHeight();
    auto ux = (1-2*border)*getWidth()/NX;
    auto uy = (1-2*border)*getHeight()/NY;
    const float step = 1.f;

    auto diagonale = (getLocalBounds().getTopLeft() - getLocalBounds().getBottomRight()).toFloat();
    auto length = diagonale.getDistanceFromOrigin();
    auto perpendicular = diagonale.rotatedAboutOrigin (juce::degreesToRadians (90.0f)) / length;
    auto height = float (getWidth() * getHeight()) / length;
    auto bluegreengrey = juce::Colour::fromFloatRGBA (0.15f, 0.15f, 0.25f, 1.0f);
    juce::ColourGradient grad (bluegreengrey.darker().darker().darker(), perpendicular * height,
                           bluegreengrey, perpendicular * -height, false);
    g.setGradientFill(grad);
    g.fillAll();

    drawBoxWithTitleOnLeft(g,"Sampler",SAMPLERCOLOR,uxb+0.5*ux,uyb,10*ux,uy,0.5*ux);
    drawBoxWithTitleOnLeft(g,"Noise",NOISECOLOR,uxb+0.5*ux,uyb+uy,10*ux,uy,0.5*ux);
    drawBoxWithTitleOnLeft(g,"Cracks",CRACKSCOLOR,uxb+0.5*ux,uyb+2*uy,10*ux,uy,0.5*ux);

    drawBoxWithTitleOnLeft(g,"Exciters",TITLECOLOR,uxb,uyb,0.5*ux,3*uy, 0.5*ux);
    drawBoxWithTitleOnLeft(g,"Waveguides",TITLECOLOR,uxb,uyb+3.25*uy,0.5*ux,3.25*uy, 0.5*ux);

    drawBoxWithTitleOnTop(g,"Properties when note off",STRINGCOLOR,uxb+0.5*ux,uyb+5.25*uy,3*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"Properties when note on",STRINGCOLOR,uxb+3.5*ux,uyb+5.25*uy,3*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"Enveloppe -> Properties",STRINGCOLOR,uxb+6.5*ux,uyb+5.25*uy,4*ux,1.25*uy,0.25*uy);
    
    drawBoxWithTitleOnTop(g,"",STRINGCOLOR,uxb+0.5*ux,uyb+3.25*uy,8*ux,1.85*uy,0.f);
    drawBoxWithTitleOnTop(g,"Waveguide 1",STRINGCOLOR,uxb+0.5*ux,uyb+3.25*uy,4*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"Waveguide 2",STRINGCOLOR,uxb+4.5*ux,uyb+3.25*uy,4*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"In/Out Positions",STRINGCOLOR,uxb+8.5*ux,uyb+3.25*uy,4*ux,1.85*uy,0.25*uy);

    drawBoxWithTitleOnLeft(g,"Velocity sensitivity",TITLECOLOR,uxb+8*ux,uyb,2.5*ux,3*uy,0.5*ux);
    drawBoxWithTitleOnTop(g,"Midi",MASTERCOLOR,uxb+10.5*ux,uyb+1.75*uy,2*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"Master",MASTERCOLOR,uxb+10.5*ux,uyb+5.25*uy,2*ux,3.25*uy,0.25*uy);

    drawBoxWithTitleOnLeft(g," Lfos",TITLECOLOR,uxb,uyb+6.75*uy,0.5*ux,1.25*uy,0.5*ux);
    // drawBoxWithTitleOnLeft(g,"  1",LFO1COLOR,uxb+0.5*ux,uyb+6.75*uy,3.5*ux,1*uy,0.5*ux);
    // drawBoxWithTitleOnLeft(g,"  2",LFO2COLOR,uxb+4*ux,uyb+6.75*uy,3.5*ux,1*uy,0.5*ux);
    // drawBoxWithTitleOnLeft(g,"  3",LFO3COLOR,uxb+7.5*ux,uyb+6.75*uy,3.5*ux,1*uy,0.5*ux);
    drawBoxWithTitleOnTop(g,"  1",LFO1COLOR,uxb+0.5*ux,uyb+6.75*uy,3*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"  2",LFO2COLOR,uxb+3.5*ux,uyb+6.75*uy,3*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"  3",LFO3COLOR,uxb+6.5*ux,uyb+6.75*uy,3*ux,1.25*uy,0.25*uy);

    drawBoxWithTitleOnTop(g,"StrinGO v0.0.2",MASTERCOLOR,uxb+10.5*ux,uyb,2*ux,1.75*uy,0.25*uy);
    // g.drawText("StrinGO v0.0.2", juce::Rectangle<float>(uxb+10.50*ux,0,2*ux,uy),juce::Justification::centred);
    auto r = juce::Rectangle<float>(uxb+10.5*ux,uyb+0.3*uy,2*ux,2*ux*227/269).reduced(10);
    g.drawImage(logo, r);
}

// ----------------------------------------------------------------------------
void MySynthAudioProcessorEditor::resized()
{
    static const float border = 0.01;
    const float uxb = border*getWidth();
    const float uyb = border*getHeight();
    const auto ux = (1-2*border)*getWidth()/NX;
    const auto uy = (1-2*border)*getHeight()/NY;
    const float step = 1.f;
    const float stepy = 1.f;
    const float size = 0.8f;

    float ex = uxb+10.5*ux;
    float ey = uyb+2*uy;
    porta.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    voices.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+10.5*ux;
    ey = uyb+5.5*uy;
    inGain.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    directOut.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+10.5*ux;
    ey = uyb+6.55*uy;
    gain.setBounds(juce::Rectangle<int>(ex,ey,ux,1.5*uy).reduced(DELTAX*ux,DELTAY*uy));
    level.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,1.5*uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+8.5*ux;
    ey = uyb;
    velocitySampleFreq.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    velocitySampleLevel.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    velocityNoiseFreq.setBounds(juce::Rectangle<int>(ex,ey+uy,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    velocityNoiseLevel.setBounds(juce::Rectangle<int>(ex+ux,ey+uy,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    velocityCrackFreq.setBounds(juce::Rectangle<int>(ex,ey+2*uy,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    velocityCrackLevel.setBounds(juce::Rectangle<int>(ex+ux,ey+2*uy,ux,uy).reduced(DELTAX*ux,DELTAY*uy));


    ex = uxb+8.75*ux;
    ey = uyb+3.75*uy;
    stringsInPos.setBounds(ex,ey+0*uy,3.5*ux,0.5*uy);
    stringsOutPos.setBounds(ex,ey+0.75*uy,3.5*ux,0.5*uy);
    lfo1InPos1Button.setBounds(juce::Rectangle<int>(ex+1.5*ux,ey,0.2f*ux,0.1f*uy));
    lfo1OutPos1Button.setBounds(juce::Rectangle<int>(ex+1.5*ux,ey+0.75*uy,0.2f*ux,0.1f*uy));
    lfo1InPos2Button.setBounds(juce::Rectangle<int>(ex+2.5*ux,ey,0.2f*ux,0.1f*uy));
    lfo1OutPos2Button.setBounds(juce::Rectangle<int>(ex+2.5*ux,ey+0.75*uy,0.2f*ux,0.1f*uy));
    lfo2InPos1Button.setBounds(juce::Rectangle<int>(ex+1.75*ux,ey,0.2f*ux,0.1f*uy));
    lfo2OutPos1Button.setBounds(juce::Rectangle<int>(ex+1.75*ux,ey+0.75*uy,0.2f*ux,0.1f*uy));
    lfo2InPos2Button.setBounds(juce::Rectangle<int>(ex+2.75*ux,ey,0.2f*ux,0.1f*uy));
    lfo2OutPos2Button.setBounds(juce::Rectangle<int>(ex+2.75*ux,ey+0.75*uy,0.2f*ux,0.1f*uy));
    lfo3InPos1Button.setBounds(juce::Rectangle<int>(ex+2*ux,ey,0.2f*ux,0.1f*uy));
    lfo3OutPos1Button.setBounds(juce::Rectangle<int>(ex+2*ux,ey+0.75*uy,0.2f*ux,0.1f*uy));
    lfo3InPos2Button.setBounds(juce::Rectangle<int>(ex+3*ux,ey,0.2f*ux,0.1f*uy));
    lfo3OutPos2Button.setBounds(juce::Rectangle<int>(ex+3*ux,ey+0.75*uy,0.2f*ux,0.1f*uy));

    ex = uxb+ux;
    ey = uyb+4.6*uy;
    stringsCoupling.setBounds(ex,ey,7*ux,0.5*uy);

    #define BLFO1POS 0.15f
    #define BLFO2POS 0.40f
    #define BLFO3POS 0.65f
    #define BLFOWIDTH 0.7f

    ex = uxb+0.5*ux;
    ey = uyb+3.5*uy;
    string1Level.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    string1FreqCoarse.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    string1FreqFine.setBounds(juce::Rectangle<int>(ex+3*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    lfo1Level1Button.setBounds(juce::Rectangle<int>(ex+BLFO1POS*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo1Coarse1Button.setBounds(juce::Rectangle<int>(ex+(2+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo1Fine1Button.setBounds(juce::Rectangle<int>(ex+(3+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2Level1Button.setBounds(juce::Rectangle<int>(ex+BLFO2POS*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2Coarse1Button.setBounds(juce::Rectangle<int>(ex+(2+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2Fine1Button.setBounds(juce::Rectangle<int>(ex+(3+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3Level1Button.setBounds(juce::Rectangle<int>(ex+BLFO3POS*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3Coarse1Button.setBounds(juce::Rectangle<int>(ex+(2+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3Fine1Button.setBounds(juce::Rectangle<int>(ex+(3+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));

    ex = uxb+4.5*ux;
    ey = uyb+3.5*uy;
    string2Level.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    string2FreqCoarse.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    string2FreqFine.setBounds(juce::Rectangle<int>(ex+3*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    lfo1Level2Button.setBounds(juce::Rectangle<int>(ex+BLFO1POS*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo1Coarse2Button.setBounds(juce::Rectangle<int>(ex+(2+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo1Fine2Button.setBounds(juce::Rectangle<int>(ex+(3+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2Level2Button.setBounds(juce::Rectangle<int>(ex+BLFO2POS*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2Coarse2Button.setBounds(juce::Rectangle<int>(ex+(2+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2Fine2Button.setBounds(juce::Rectangle<int>(ex+(3+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3Level2Button.setBounds(juce::Rectangle<int>(ex+BLFO3POS*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3Coarse2Button.setBounds(juce::Rectangle<int>(ex+(2+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3Fine2Button.setBounds(juce::Rectangle<int>(ex+(3+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));

    ex = uxb+0.5*ux;
    ey = uyb+5.5*uy;
    stringsFBFreqOff.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsFBGainOff.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsLevelOff.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+3.5*ux;
    ey = uyb+5.5*uy;
    stringsFBFreqOn.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsFBGainOn.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsLevelOn.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+6.5*ux;
    ey = uyb+5.5*uy;
    stringsAttack.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsDecay.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsSustain.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsRelease.setBounds(juce::Rectangle<int>(ex+3*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+ux;
    ey = uyb;
    samplerAttack.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    samplerDecay.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    samplerSustain.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    samplerRelease.setBounds(juce::Rectangle<int>(ex+3*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    samplerLPFreq.setBounds(juce::Rectangle<int>(ex+5*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    samplerLevel.setBounds(juce::Rectangle<int>(ex+6*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    waveComboBox.setBounds(ex+4*ux,ey+0.25*uy,ux,0.25*uy);
    loopButton.setBounds(ex+4.75*ux,ey+0.6*uy,size*ux,0.25*uy);
    lfo1SamplerLevelButton.setBounds(juce::Rectangle<int>(ex+(6+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo1SamplerLPFreqButton.setBounds(juce::Rectangle<int>(ex+(5+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2SamplerLevelButton.setBounds(juce::Rectangle<int>(ex+(6+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2SamplerLPFreqButton.setBounds(juce::Rectangle<int>(ex+(5+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3SamplerLevelButton.setBounds(juce::Rectangle<int>(ex+(6+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3SamplerLPFreqButton.setBounds(juce::Rectangle<int>(ex+(5+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));

    ex = uxb+ux;
    ey = uyb+uy;
    noiseAttack.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseDecay.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseSustain.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseRelease.setBounds(juce::Rectangle<int>(ex+3*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseHPFreq.setBounds(juce::Rectangle<int>(ex+4*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseLPFreq.setBounds(juce::Rectangle<int>(ex+5*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseLevel.setBounds(juce::Rectangle<int>(ex+6*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    lfo1NoiseLevelButton.setBounds(juce::Rectangle<int>(ex+(6+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo1NoiseLPFreqButton.setBounds(juce::Rectangle<int>(ex+(5+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo1NoiseHPFreqButton.setBounds(juce::Rectangle<int>(ex+(4+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2NoiseLevelButton.setBounds(juce::Rectangle<int>(ex+(6+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2NoiseLPFreqButton.setBounds(juce::Rectangle<int>(ex+(5+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2NoiseHPFreqButton.setBounds(juce::Rectangle<int>(ex+(4+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3NoiseLevelButton.setBounds(juce::Rectangle<int>(ex+(6+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3NoiseLPFreqButton.setBounds(juce::Rectangle<int>(ex+(5+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3NoiseHPFreqButton.setBounds(juce::Rectangle<int>(ex+(4+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));

    ex = uxb+ux;
    ey = uyb+2*uy;
    crackAttack.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackDecay.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackSustain.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackRelease.setBounds(juce::Rectangle<int>(ex+3*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackDensity.setBounds(juce::Rectangle<int>(ex+4*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackLPFreq.setBounds(juce::Rectangle<int>(ex+5*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackLevel.setBounds(juce::Rectangle<int>(ex+6*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    lfo1CrackLevelButton.setBounds(juce::Rectangle<int>(ex+(6+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo1CrackLPFreqButton.setBounds(juce::Rectangle<int>(ex+(5+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo1CrackDensityButton.setBounds(juce::Rectangle<int>(ex+(4+BLFO1POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2CrackLevelButton.setBounds(juce::Rectangle<int>(ex+(6+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2CrackLPFreqButton.setBounds(juce::Rectangle<int>(ex+(5+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo2CrackDensityButton.setBounds(juce::Rectangle<int>(ex+(4+BLFO2POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3CrackLevelButton.setBounds(juce::Rectangle<int>(ex+(6+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3CrackLPFreqButton.setBounds(juce::Rectangle<int>(ex+(5+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));
    lfo3CrackDensityButton.setBounds(juce::Rectangle<int>(ex+(4+BLFO3POS)*ux,ey+.87f*uy,0.2f*ux,0.1f*uy));

    ex = uxb+.5*ux;
    ey = uyb+7*uy;
    lfo1Amp.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    lfo1Freq.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+3.5*ux;
    ey = uyb+7*uy;
    lfo2Amp.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    lfo2Freq.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+6.5*ux;
    ey = uyb+7*uy;
    lfo3Amp.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    lfo3Freq.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

}

// -------------------------------------------------------------------------
void MySynthAudioProcessorEditor::addKnob(juce::Slider& slider,
                                                    juce::Label &label,
                                                    juce::Colour fillCol,
                                                    juce::Colour outlineCol)
{
  slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,80,15);
  slider.setTextBoxIsEditable(true);
  slider.setColour(juce::Slider::thumbColourId, fillCol);
  slider.setColour(juce::Slider::rotarySliderFillColourId, fillCol);
  slider.setColour(juce::Slider::trackColourId, outlineCol);
  slider.setColour(juce::Slider::backgroundColourId, outlineCol);
  slider.setColour(juce::Slider::rotarySliderOutlineColourId, outlineCol);
  slider.setLookAndFeel(&fxmeKnobLookAndFeel);
  addAndMakeVisible(slider);

  label.setJustificationType(juce::Justification::centred);
  label.attachToComponent(&slider,false);
  juce::AffineTransform t;
  auto labelArea{label.getLocalBounds().toFloat()};
  label.setTransform(t.translated(0,labelArea.getHeight()*0.45f));
  addAndMakeVisible(label);  
}

// -------------------------------------------------------------------------
void MySynthAudioProcessorEditor::addVSlider(juce::Slider& slider,
                                                    juce::Label &label,
                                                    juce::Colour fillCol,
                                                    juce::Colour outlineCol)
{
  slider.setSliderStyle(juce::Slider::LinearVertical);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,80,15);
  slider.setTextBoxIsEditable(true);
  slider.setColour(juce::Slider::thumbColourId, fillCol);
  slider.setColour(juce::Slider::rotarySliderFillColourId, fillCol);
  slider.setColour(juce::Slider::trackColourId, outlineCol);
  slider.setColour(juce::Slider::backgroundColourId, outlineCol);
  slider.setColour(juce::Slider::rotarySliderOutlineColourId, outlineCol);
  addAndMakeVisible(slider);

  label.setJustificationType(juce::Justification::centred);
  label.attachToComponent(&slider,false);
  // juce::AffineTransform t;
  // auto labelArea{label.getLocalBounds().toFloat()};
  // label.setTransform(t.translated(0,labelArea.getHeight()*0.75f));
  addAndMakeVisible(label);
}


// -------------------------------------------------------------------------
void MySynthAudioProcessorEditor::addHSlider(juce::Slider& slider,
                                                    juce::Label &label,
                                                    juce::Colour fillCol,
                                                    juce::Colour outlineCol)
{
  slider.setSliderStyle(juce::Slider::LinearHorizontal);
  slider.setTextBoxStyle(juce::Slider::NoTextBox,true,120,15);
  slider.setTextBoxIsEditable(true);
  slider.setColour(juce::Slider::thumbColourId, fillCol);
  slider.setColour(juce::Slider::rotarySliderFillColourId, fillCol);
  slider.setColour(juce::Slider::trackColourId, outlineCol);
  slider.setColour(juce::Slider::backgroundColourId, outlineCol);
  slider.setColour(juce::Slider::rotarySliderOutlineColourId, outlineCol);
  addAndMakeVisible(slider);

  label.setJustificationType(juce::Justification::left);
  label.attachToComponent(&slider,false);
  // label.setBoundsRelative(0.2,0.2,0.2,0.2);
  juce::AffineTransform t;
  auto labelArea{label.getLocalBounds().toFloat()};
  label.setTransform(t.translated(-labelArea.getWidth(),labelArea.getHeight()*.75f));
  addAndMakeVisible(label);
}

// -------------------------------------------------------------------------------------
 void MySynthAudioProcessorEditor::drawBoxWithTitleOnTop(juce::Graphics &g,
                                                          const juce::String title,
                                                          const juce::Colour baseColour,
                                                          const float x,
                                                          const float y,
                                                          const float wx,
                                                          const float wy,
                                                          const float bandHeight)
 {
  g.setColour(baseColour.darker(1.5f));
  juce::Rectangle<int> stringRect2(x,y,wx,wy);
  g.drawRect(stringRect2);
  juce::Rectangle<int> stringBand2(x,y,wx,bandHeight);
  g.fillRect(stringBand2);
  g.setColour(juce::Colours::white);    
  g.drawText(title, stringBand2, juce::Justification::centred);
 }


// -------------------------------------------------------------------------------------
void MySynthAudioProcessorEditor::drawBoxWithTitleOnLeft(juce::Graphics &g,
                                                          const juce::String title,
                                                          const juce::Colour baseColour,
                                                          const float x,
                                                          const float y,
                                                          const float wx,
                                                          const float wy,
                                                          const float bandWidth)
 {
  float fontSize=18;
  g.setColour(baseColour.darker(1.5f));
  juce::Rectangle<int> stringRect2(x,y,wx,wy);
  g.drawRect(stringRect2);
  juce::Rectangle<int> stringBand2(x,y,bandWidth,wy);
  g.fillRect(stringBand2);
  g.setColour(juce::Colours::white);
  g.setFont (fontSize);
    
  juce::GlyphArrangement ga;
  ga.addLineOfText(juce::Font(fontSize), title, 0, 0);
  juce::Path p;
  ga.createPath(p);

  auto pathBounds = p.getBounds();
  p.applyTransform(juce::AffineTransform()
                    .translated(x, y+wy*0.5)
                    .rotated(-1.57079632f,x+bandWidth/2,y+wy/2)
                    );

  g.setColour (juce::Colours::white);
  g.fillPath(p);
 }
